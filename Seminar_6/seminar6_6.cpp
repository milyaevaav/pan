#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

class TelemetryFilter {
private:
    std::vector<std::vector<std::string>> data;
    std::vector<std::vector<std::string>> filteredData;

public:
    bool loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return false;
        }

        std::string line;
        bool firstLine = true;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            if (firstLine) {
                data.push_back(row);
                filteredData.push_back(row);
                firstLine = false;
            } else {
                data.push_back(row);
            }
        }

        file.close();

        if (data.size() <= 1) {
            std::cerr << "Файл не содержит данных или имеет неверный формат" << std::endl;
            return false;
        }

        std::cout << "Загружено " << data.size() - 1 << " строк телеметрии" << std::endl;
        return true;
    }

    void filterData() {
        auto isValidAltitude = [](const std::string& altStr) -> bool {
            try {
                double alt = std::stod(altStr);
                return alt >= 0.0 && alt <= 20000.0;
            } catch (...) {
                return false;
            }
        };

        auto isValidSpeed = [](const std::string& speedStr) -> bool {
            try {
                double speed = std::stod(speedStr);
                return speed >= 0.0 && speed <= 500.0;
            } catch (...) {
                return false;
            }
        };

        if (data.size() <= 1) return;

        filteredData.clear();
        filteredData.push_back(data[0]);
        for (size_t i = 1; i < data.size(); ++i) {
            const auto& row = data[i];

            if (row.size() < 5) {
                std::cerr << "Предупреждение: строка " << i << " имеет некорректное количество столбцов" << std::endl;
                continue;
            }

            std::string altitudeStr = row[1];
            std::string speedStr = row[2];
            if (isValidAltitude(altitudeStr) && isValidSpeed(speedStr)) {
                filteredData.push_back(row);
            } else {
                std::cout << "Отфильтрована строка " << i << ": ";
                std::cout << "time=" << row[0] << ", altitude=" << altitudeStr;
                std::cout << ", speed=" << speedStr << std::endl;
            }
        }

        std::cout << "После фильтрации осталось " << filteredData.size() - 1 << " корректных строк" << std::endl;
    }

    bool saveToCSV(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
            return false;
        }

        for (const auto& row : filteredData) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }

        file.close();

        if (!file.good()) {
            std::cerr << "ошибка при записи файла" << std::endl;
            return false;
        }

        std::cout << "Отфильтрованные данные сохранены в " << filename << std::endl;
        return true;
    }
    void printFilteredStats() {
        if (filteredData.size() <= 1) {
            std::cout << "Нет данных для анализа" << std::endl;
            return;
        }

        std::cout << "\nОтфильтрованные данные" << std::endl;
        int totalRows = filteredData.size() - 1;
        double minAltitude = 1e9, maxAltitude = -1e9, sumAltitude = 0;
        double minSpeed = 1e9, maxSpeed = -1e9, sumSpeed = 0;
        double minFuel = 1e9, maxFuel = -1e9, sumFuel = 0;
        for (size_t i = 1; i < filteredData.size(); ++i) {
            const auto& row = filteredData[i];

            if (row.size() >= 5) {
                try {
                    double altitude = std::stod(row[1]);
                    double speed = std::stod(row[2]);
                    double fuel = std::stod(row[4]);
                    if (altitude < minAltitude) minAltitude = altitude;
                    if (altitude > maxAltitude) maxAltitude = altitude;
                    sumAltitude += altitude;
                    if (speed < minSpeed) minSpeed = speed;
                    if (speed > maxSpeed) maxSpeed = speed;
                    sumSpeed += speed;
                    if (fuel < minFuel) minFuel = fuel;
                    if (fuel > maxFuel) maxFuel = fuel;
                    sumFuel += fuel;

                } catch (const std::exception& e) {
                    std::cerr << "Ошибка при обработке строки " << i << ": " << e.what() << std::endl;
                }
            }
        }

        std::cout << "Количество корректных записей: " << totalRows << std::endl;

        if (totalRows > 0) {
            std::cout << "\nВысота (altitude):" << std::endl;
            std::cout << "  Минимальная: " << std::fixed << std::setprecision(2) << minAltitude << " м" << std::endl;
            std::cout << "  Максимальная: " << maxAltitude << " м" << std::endl;
            std::cout << "  Средняя: " << sumAltitude / totalRows << " м" << std::endl;

            std::cout << "\nСкорость (speed):" << std::endl;
            std::cout << "  Минимальная: " << minSpeed << " м/с" << std::endl;
            std::cout << "  Максимальная: " << maxSpeed << " м/с" << std::endl;
            std::cout << "  Средняя: " << sumSpeed / totalRows << " м/с" << std::endl;

            std::cout << "\nТопливо (fuel):" << std::endl;
            std::cout << "  Минимальное: " << minFuel << " %" << std::endl;
            std::cout << "  Максимальное: " << maxFuel << " %" << std::endl;
            std::cout << "  Среднее: " << sumFuel / totalRows << " %" << std::endl;
        }

        int originalRows = data.size() - 1;
        if (originalRows > 0) {
            double filteredPercentage = 100.0 * (originalRows - totalRows) / originalRows;
            std::cout << "\nФильтрация: удалено " << (originalRows - totalRows)
                     << " из " << originalRows << " строк ("
                     << std::fixed << std::setprecision(1) << filteredPercentage << "%)" << std::endl;
        }
    }

    void printData(const std::vector<std::vector<std::string>>& dataset, const std::string& title) {
        std::cout << "\n=== " << title << " ===" << std::endl;

        if (dataset.empty()) {
            std::cout << "Данные отсутствуют" << std::endl;
            return;
        }

        std::vector<size_t> colWidths(dataset[0].size(), 0);
        for (const auto& row : dataset) {
            for (size_t i = 0; i < row.size(); ++i) {
                if (row[i].length() > colWidths[i]) {
                    colWidths[i] = row[i].length();
                }
            }
        }
        for (const auto& row : dataset) {
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << std::setw(colWidths[i] + 2) << std::left << row[i];
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    TelemetryFilter filter;
    std::cout << "Загрузка данных из telemetry.csv..." << std::endl;
    if (!filter.loadFromCSV("telemetry.csv")) {
        std::cerr << "Не удалось загрузить данные" << std::endl;
        return 1;
    }
    std::cout << "\nФильтрация данных..." << std::endl;
    filter.filterData();
    std::cout << "\nСохранение отфильтрованных данных..." << std::endl;
    if (!filter.saveToCSV("filtered_telemetry.csv")) {
        std::cerr << "Не удалось сохранить отфильтрованные данные" << std::endl;
        return 1;
    }
    filter.printFilteredStats();

    return 0;
}
