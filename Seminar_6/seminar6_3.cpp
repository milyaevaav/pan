#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
using namespace std;

struct TelemetryData {
    double time;
    double altitude;
    double speed;
    double heading;
    double fuel;

    TelemetryData() : time(0.0), altitude(0.0), speed(0.0), heading(0.0), fuel(0.0) {}

    TelemetryData(double t, double alt, double spd, double hdg, double fl)
        : time(t), altitude(alt), speed(spd), heading(hdg), fuel(fl) {}
};

class TelemetryLogger {
private:
    vector<TelemetryData> telemetryBuffer;
    string baseFilename;
    string currentFilename;
    size_t maxBufferSize;
    size_t maxFileSize;
    size_t fileCounter;

    string generateFilename() {
        ostringstream oss;
        oss << baseFilename << "_"
            << setfill('0') << setw(3) << fileCounter
            << ".bin";
        return oss.str();
    }

    size_t getCurrentFileSize(const string& filename) {
        try {
            if (fs::exists(filename)) {
                return fs::file_size(filename);
            }
        } catch (...) {
        }
        return 0;
    }

    bool writeBufferToFile() {
        if (telemetryBuffer.empty()) {
            return true;
        }
        if (currentFilename.empty()) {
            currentFilename = generateFilename();
        }

        size_t currentSize = getCurrentFileSize(currentFilename);
        if (currentSize >= maxFileSize) {
            currentFilename = generateFilename();
            fileCounter++;
            cout << "Создан новый файл: " << currentFilename << endl;
        }
        ofstream fout(currentFilename, ios::binary | ios::app);
        if (!fout.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << currentFilename << endl;
            return false;
        }
        for (const auto& data : telemetryBuffer) {
            fout.write(reinterpret_cast<const char*>(&data), sizeof(TelemetryData));
        }
        if (!fout.good()) {
            cerr << "Ошибка записи в файл: " << currentFilename << endl;
            fout.close();
            return false;
        }

        fout.close();
        telemetryBuffer.clear();

        return true;
    }

public:
    TelemetryLogger(const string& baseName = "telemetry",
                   size_t bufferSize = 10,
                   size_t fileSizeMB = 1)
        : baseFilename(baseName),
          maxBufferSize(bufferSize),
          maxFileSize(fileSizeMB * 1024 * 1024),
          fileCounter(1) {

        currentFilename = generateFilename();
        cout << "Инициализирован логгер телеметрии. Первый файл: "
             << currentFilename << endl;
    }
    ~TelemetryLogger() {
        if (!telemetryBuffer.empty()) {
            cout << "Запись оставшихся данных перед завершением..." << endl;
            writeBufferToFile();
        }
    }
    bool logData(double time, double altitude, double speed,
                double heading, double fuel) {

        if (time < 0 || altitude < 0 || speed < 0 || fuel < 0 || fuel > 100) {
            cerr << "Некорректные данные телеметрии!" << endl;
            return false;
        }

        telemetryBuffer.emplace_back(time, altitude, speed, heading, fuel);

        cout << "Логирование: Time: " << fixed << setprecision(1) << time
             << ", Altitude: " << altitude
             << ", Speed: " << speed
             << ", Heading: " << heading
             << ", Fuel: " << fuel << "%" << endl;

        if (telemetryBuffer.size() >= maxBufferSize) {
            cout << "Буфер заполнен, запись в файл..." << endl;
            return writeBufferToFile();
        }

        return true;
    }
    bool flushBuffer() {
        if (telemetryBuffer.empty()) {
            cout << "Буфер пуст, запись не требуется." << endl;
            return true;
        }

        cout << "Принудительная запись " << telemetryBuffer.size()
             << " записей в файл..." << endl;
        return writeBufferToFile();
    }

    void rotateFileIfNeeded() {
        if (currentFilename.empty()) {
            return;
        }

        size_t currentSize = getCurrentFileSize(currentFilename);

        if (currentSize >= maxFileSize) {
            cout << "Размер файла " << currentFilename
                 << " превышен (" << currentSize << " байт). Ротация..." << endl;

            if (!telemetryBuffer.empty()) {
                writeBufferToFile();
            }

            fileCounter++;
            currentFilename = generateFilename();

            cout << "Создан новый файл: " << currentFilename << endl;
        }
    }

    vector<TelemetryData> readLogFile(const string& filename) {
        vector<TelemetryData> data;

        ifstream fin(filename, ios::binary | ios::ate);
        if (!fin.is_open()) {
            cerr << "Ошибка открытия файла для чтения: " << filename << endl;
            return data;
        }

        streamsize fileSize = fin.tellg();
        fin.seekg(0, ios::beg);
        if (fileSize % sizeof(TelemetryData) != 0) {
            cerr << "Некорректный размер файла: " << fileSize
                 << " байт (размер структуры: " << sizeof(TelemetryData)
                 << " байт)!" << endl;
            fin.close();
            return data;
        }
        size_t recordCount = fileSize / sizeof(TelemetryData);
        data.resize(recordCount);
        fin.read(reinterpret_cast<char*>(data.data()), fileSize);

        if (!fin.good()) {
            cerr << "Ошибка чтения файла!" << endl;
            data.clear();
        } else {
            cout << "Прочитано " << recordCount << " записей из файла "
                 << filename << endl;
        }

        fin.close();
        return data;
    }

    void printLogSummary() {
        if (!telemetryBuffer.empty()) {
            cout << "В буфере " << telemetryBuffer.size()
                 << " несохраненных записей." << endl;
        }

        vector<string> logFiles;
        try {
            for (const auto& entry : fs::directory_iterator(".")) {
                if (entry.path().extension() == ".bin" &&
                    entry.path().filename().string().find(baseFilename) == 0) {
                    logFiles.push_back(entry.path().string());
                }
            }
        } catch (...) {
            cerr << "Ошибка доступа к директории!" << endl;
        }

        sort(logFiles.begin(), logFiles.end());

        cout << "\n=== Сводка по логам телеметрии ===" << endl;
        cout << "Базовое имя файлов: " << baseFilename << endl;
        cout << "Текущий файл: " << currentFilename << endl;
        cout << "Следующий номер файла: " << fileCounter << endl;
        cout << "Всего файлов логов: " << logFiles.size() << endl;

        if (!logFiles.empty()) {
            cout << "\nСписок файлов логов:" << endl;
            for (const auto& file : logFiles) {
                try {
                    size_t fileSize = fs::file_size(file);
                    cout << "  " << fs::path(file).filename().string()
                         << " - " << fileSize << " байт ("
                         << fileSize / sizeof(TelemetryData) << " записей)" << endl;
                } catch (...) {
                    cout << "  " << file << " - ошибка доступа" << endl;
                }
            }

            if (!logFiles[0].empty()) {
                auto data = readLogFile(logFiles[0]);
                if (!data.empty()) {
                    cout << "\nАнализ первого файла (" << logFiles[0] << "):" << endl;
                    cout << "Первая запись: Time: " << fixed << setprecision(1) << data[0].time
                         << ", Altitude: " << data[0].altitude
                         << ", Speed: " << data[0].speed << endl;
                    cout << "Последняя запись: Time: " << fixed << setprecision(1) << data.back().time
                         << ", Altitude: " << data.back().altitude
                         << ", Speed: " << data.back().speed << endl;

                    double minAlt = data[0].altitude;
                    double maxAlt = data[0].altitude;
                    double totalFuel = 0.0;

                    for (const auto& record : data) {
                        if (record.altitude < minAlt) minAlt = record.altitude;
                        if (record.altitude > maxAlt) maxAlt = record.altitude;
                        totalFuel += record.fuel;
                    }

                    cout << "Минимальная высота: " << minAlt << " м" << endl;
                    cout << "Максимальная высота: " << maxAlt << " м" << endl;
                    cout << "Средний уровень топлива: " << fixed << setprecision(1)
                         << totalFuel / data.size() << "%" << endl;
                }
            }
        }
    }

    string getCurrentFilename() const {
        return currentFilename;
    }
    size_t getBufferSize() const {
        return telemetryBuffer.size();
    }
    size_t getMaxBufferSize() const {
        return maxBufferSize;
    }
    void setMaxBufferSize(size_t newSize) {
        maxBufferSize = newSize;
        cout << "Максимальный размер буфера установлен в " << newSize << " записей." << endl;
    }
};

void demonstrateTelemetryLogger() {
    cout << "Система логирования телеметрии " << endl;

    TelemetryLogger logger("telemetry", 3, 0.1);
    cout << "\nЛогирование данных телеметрии " << endl;
    logger.logData(0.0, 100.0, 25.0, 45.0, 80.0);
    logger.logData(1.0, 105.0, 27.0, 46.0, 79.5);
    logger.logData(2.0, 110.0, 29.0, 47.0, 79.0);
    logger.flushBuffer();
}



int main() {
    demonstrateTelemetryLogger();
    return 0;
}
