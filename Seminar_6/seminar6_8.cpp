#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

class FuelAnalyzer {
private:
    std::vector<double> time_data;
    std::vector<double> fuel_data;
    std::vector<double> rpm_data;
    std::vector<bool> anomalies;

public:
    bool loadData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(file, line);

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() != 3) continue;

            try {
                time_data.push_back(std::stod(tokens[0]));
                fuel_data.push_back(std::stod(tokens[1]));
                rpm_data.push_back(std::stod(tokens[2]));
            } catch (...) {
                continue;
            }
        }

        file.close();
        anomalies.resize(fuel_data.size(), false);
        std::cout << "Загружено " << time_data.size() << " записей" << std::endl;
        return true;
    }

    void detectAnomalies() {
        if (fuel_data.empty()) return;

        double avg = calculateAverageConsumption();
        double threshold = 1.5;

        auto isAnomaly = [avg, threshold](double consumption) {
            return consumption > avg * threshold;
        };

        for (size_t i = 0; i < fuel_data.size(); ++i) {
            anomalies[i] = isAnomaly(fuel_data[i]);
        }

        int anomaly_count = std::count(anomalies.begin(), anomalies.end(), true);
        std::cout << "Обнаружено " << anomaly_count << " аномалий" << std::endl;
    }

    double calculateAverageConsumption() {
        if (fuel_data.empty()) return 0.0;

        double sum = 0.0;
        for (double f : fuel_data) {
            sum += f;
        }
        return sum / fuel_data.size();
    }

    void generateReport(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка создания файла " << filename << std::endl;
            return;
        }

        file << std::fixed << std::setprecision(2);
        file << "Анализ расхода топлива\n";
        file << "-----------------\n\n";

        double avg_consumption = calculateAverageConsumption();
        file << "Средний расход топлива: " << avg_consumption << " л/ч\n\n";

        file << "Время,Расход топлива,Обороты двигателя,Аномалия\n";
        for (size_t i = 0; i < time_data.size(); ++i) {
            file << time_data[i] << ","
                 << fuel_data[i] << ","
                 << rpm_data[i] << ","
                 << (anomalies[i] ? "ДА" : "нет") << "\n";
        }

        file << "\nДетектированные аномалии:\n";
        file << "Время\tРасход\tОбороты\n";
        for (size_t i = 0; i < time_data.size(); ++i) {
            if (anomalies[i]) {
                file << time_data[i] << "\t"
                     << fuel_data[i] << "\t"
                     << rpm_data[i] << "\n";
            }
        }

        file.close();
        std::cout << "Отчет сохранен в " << filename << std::endl;
    }

    void printStatistics() {
        if (fuel_data.empty()) return;

        double avg = calculateAverageConsumption();
        double min_fuel = *std::min_element(fuel_data.begin(), fuel_data.end());
        double max_fuel = *std::max_element(fuel_data.begin(), fuel_data.end());

        std::cout << "\nСтатистика расхода топлива:\n";
        std::cout << "Минимальный: " << min_fuel << " л/ч\n";
        std::cout << "Максимальный: " << max_fuel << " л/ч\n";
        std::cout << "Средний: " << avg << " л/ч\n";

        double sum_sq_diff = 0.0;
        for (double f : fuel_data) {
            sum_sq_diff += (f - avg) * (f - avg);
        }
        double std_dev = std::sqrt(sum_sq_diff / fuel_data.size());
        std::cout << "Стандартное отклонение: " << std_dev << " л/ч\n";
    }
};

int main() {
    FuelAnalyzer analyzer;

    if (!analyzer.loadData("fuel_data.csv")) {
        return 1;
    }

    analyzer.detectAnomalies();
    analyzer.printStatistics();
    analyzer.generateReport("fuel_report.txt");

    return 0;
}
