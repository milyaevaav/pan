#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include <cstdio>
#include <cmath>

class AltitudeData {
private:
    std::vector<std::pair<double, double>> data;

public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return false;
        }

        std::string line;
        bool firstLine = true;

        while (std::getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }

            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string t_str, h_str;

            if (std::getline(iss, t_str, ',') && std::getline(iss, h_str, ',')) {
                try {
                    double t = std::stod(t_str);
                    double h = std::stod(h_str);
                    data.emplace_back(t, h);
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка парсинга строки: " << line << " - " << e.what() << std::endl;
                    continue;
                }
            } else {
                std::cerr << "Некорректный формат строки: " << line << std::endl;
            }
        }

        file.close();
        std::cout << "Загружено " << data.size() << " точек данных." << std::endl;
        return true;
    }

    std::vector<std::pair<double, double>> filterOutliers() const {
        std::vector<std::pair<double, double>> filtered;

        auto isValidAltitude = [](const std::pair<double, double>& point) {
            double h = point.second;
            return (h > 900.0) && (h < 1100.0);
        };

        std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), isValidAltitude);

        std::cout << "После фильтрации осталось " << filtered.size() << " точек." << std::endl;
        return filtered;
    }

    bool saveFilteredToCSV(const std::vector<std::pair<double, double>>& filtered,
                          const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка при создании файла " << filename << std::endl;
            return false;
        }

        file << "t,H\n";
        for (const auto& point : filtered) {
            file << point.first << "," << point.second << "\n";
        }

        file.close();
        std::cout << "Отфильтрованные данные сохранены в " << filename << std::endl;
        return true;
    }

    void plotComparison(const std::vector<std::pair<double, double>>& filtered) const {
        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            std::cerr << "Ошибка при запуске gnuplot." << std::endl;
            return;
        }

        fprintf(gp, "set terminal qt size 1000,800\n");
        fprintf(gp, "set multiplot layout 2,1 title 'Сравнение исходных и отфильтрованных данных'\n");

        fprintf(gp, "set title 'Исходные данные (высота от времени)'\n");
        fprintf(gp, "set xlabel 'Время t (с)'\n");
        fprintf(gp, "set ylabel 'Высота H (м)'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set yrange [800:1600]\n");
        fprintf(gp, "plot '-' using 1:2 with linespoints title 'Исходные' lw 2 pt 7 lc rgb 'blue'\n");

        for (const auto& point : data) {
            fprintf(gp, "%f %f\n", point.first, point.second);
        }
        fprintf(gp, "e\n");

        fprintf(gp, "set title 'Отфильтрованные данные (900 < H < 1100)'\n");
        fprintf(gp, "set xlabel 'Время t (с)'\n");
        fprintf(gp, "set ylabel 'Высота H (м)'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set yrange [800:1600]\n");
        fprintf(gp, "plot '-' using 1:2 with linespoints title 'Отфильтрованные' lw 2 pt 7 lc rgb 'green'\n");

        for (const auto& point : filtered) {
            fprintf(gp, "%f %f\n", point.first, point.second);
        }
        fprintf(gp, "e\n");

        fprintf(gp, "unset multiplot\n");
        fflush(gp);

        std::cout << "Графики построены. Закройте окно Gnuplot для продолжения." << std::endl;
        pclose(gp);
    }

    void createGnuplotScript(const std::vector<std::pair<double, double>>& filtered) {
        std::ofstream orig("original.csv");
        orig << "t,H\n";
        for (const auto& point : data) {
            orig << point.first << "," << point.second << "\n";
        }
        orig.close();

        saveFilteredToCSV(filtered, "filtered.csv");

        std::ofstream script("comparison.gp");
        script << "set terminal png size 1000,800\n";
        script << "set output 'comparison_plot.png'\n");
        script << "set multiplot layout 2,1 title 'Сравнение высот'\n");

        script << "set title 'Исходные данные'\n";
        script << "set xlabel 'Время t (с)'\n";
        script << "set ylabel 'Высота H (м)'\n");
        script << "set grid\n");
        script << "set datafile separator ','\n");
        script << "plot 'original.csv' using 1:2 with linespoints title 'Исходные' lw 2 pt 7\n");

        script << "set title 'Отфильтрованные данные (900 < H < 1100)'\n";
        script << "set xlabel 'Время t (с)'\n");
        script << "set ylabel 'Высота H (м)'\n");
        script << "set grid\n");
        script << "plot 'filtered.csv' using 1:2 with linespoints title 'Отфильтрованные' lw 2 pt 7 lc rgb 'green'\n");

        script << "unset multiplot\n");
        script.close();

        std::cout << "Скрипт Gnuplot создан: comparison.gp" << std::endl;
        std::cout << "Для построения выполните: gnuplot comparison.gp" << std::endl;
    }

    void printData() const {
        std::cout << "\n=== Исходные данные ===" << std::endl;
        for (const auto& point : data) {
            std::cout << "t=" << point.first << ", H=" << point.second << std::endl;
        }
    }
};

int main() {
    AltitudeData altitudeData;
    if (!altitudeData.loadFromFile("altitude.csv")) {
        std::cerr << "Не удалось загрузить данные из altitude.csv" << std::endl;
        return 1;
    }

    altitudeData.printData();
    std::vector<std::pair<double, double>> filtered = altitudeData.filterOutliers();

    std::cout << "\n=== Отфильтрованные данные ===" << std::endl;
    for (const auto& point : filtered) {
        std::cout << "t=" << point.first << ", H=" << point.second << std::endl;
    }
    altitudeData.saveFilteredToCSV(filtered, "filtered.csv");
    altitudeData.plotComparison(filtered);


    return 0;
}
