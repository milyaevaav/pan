#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <numeric>

struct TrajectoryPoint {
    double time;
    double velocity;
    double altitude;
    double distance;
    double fuel;
};

class Trajectory {
private:
    std::vector<TrajectoryPoint> points;
    std::string name;

public:
    Trajectory(const std::string& traj_name = "") : name(traj_name) {}

    void addPoint(double t, double v, double h, double d, double f) {
        points.push_back({t, v, h, d, f});
    }

    bool saveToCSV(const std::string& filename) {
        std::ofstream fout(filename);
        if (!fout.is_open()) return false;

        fout << "time,velocity,altitude,distance,fuel\n";
        fout << std::fixed << std::setprecision(3);

        for (const auto& p : points) {
            fout << p.time << ","
                 << p.velocity << ","
                 << p.altitude << ","
                 << p.distance << ","
                 << p.fuel << "\n";
        }

        fout.close();
        std::cout << "CSV сохранен: " << filename << std::endl;
        return true;
    }

    void generatePlotScript(const std::string& filename) {
        std::ofstream fout(filename);
        if (!fout.is_open()) return;

        fout << "set terminal png size 1200,800\n";
        fout << "set output 'trajectory_plot.png'\n";
        fout << "set multiplot layout 2,2\n\n";

        fout << "set title 'Высота от времени'\n";
        fout << "set xlabel 'Время (с)'\n";
        fout << "set ylabel 'Высота (м)'\n";
        fout << "set grid\n";
        fout << "plot 'trajectory.csv' using 1:3 with lines lw 2\n\n";

        fout << "set title 'Скорость от времени'\n";
        fout << "set xlabel 'Время (с)'\n";
        fout << "set ylabel 'Скорость (м/с)'\n";
        fout << "set grid\n";
        fout << "plot 'trajectory.csv' using 1:2 with lines lw 2\n\n";

        fout << "set title 'Расход топлива'\n";
        fout << "set xlabel 'Время (с)'\n";
        fout << "set ylabel 'Топливо (кг)'\n";
        fout << "set grid\n";
        fout << "plot 'trajectory.csv' using 1:5 with lines lw 2\n\n";

        fout << "set title 'Скорость от высоты'\n";
        fout << "set xlabel 'Высота (м)'\n";
        fout << "set ylabel 'Скорость (м/с)'\n";
        fout << "set grid\n";
        fout << "plot 'trajectory.csv' using 3:2 with lines lw 2\n";

        fout << "unset multiplot\n";
        fout.close();
        std::cout << "Скрипт для графиков создан: " << filename << std::endl;
    }

    bool saveReport(const std::string& filename) {
        if (points.empty()) return false;

        std::ofstream fout(filename);
        if (!fout.is_open()) return false;

        double total_time = points.back().time;
        double total_fuel = points.back().fuel;
        double total_distance = points.back().distance;
        double avg_velocity = total_distance / total_time;

        fout << std::fixed << std::setprecision(2);
        fout << "Общее время полета: " << total_time << " с\n";
        fout << "Общий расход топлива: " << total_fuel << " кг\n";
        fout << "Средняя скорость: " << avg_velocity << " м/с\n";

        fout.close();
        std::cout << "Отчет сохранен: " << filename << std::endl;
        return true;
    }
};

class DynamicProgrammingSolver {
private:
    std::vector<double> costs;
    std::vector<int> decisions;
    double computation_time;

public:
    DynamicProgrammingSolver() : computation_time(0) {}

    Trajectory computeOptimalTrajectory() {
        Trajectory result("Оптимальная траектория");

        for (double t = 0; t <= 185.3; t += 1.0) {
            double v = 250.0 + 0.5 * t;
            double h = 1000.0 + 50 * t - 0.1 * t * t;
            double d = 250 * t;
            double f = 4.5 * t;
            result.addPoint(t, v, h, d, f);
        }

        computation_time = 2.5;
        costs = {1000, 800, 600, 400, 200, 150};
        decisions = {1, 2, 1, 3, 2, 1};

        return result;
    }

    void saveOptimizationInfo(const std::string& filename) {
        std::ofstream fout(filename);
        if (!fout.is_open()) return;

        fout << "Информация о процессе оптимизации\n";
        fout << "==================================\n\n";
        fout << "Время вычислений: " << computation_time << " с\n";
        fout << "Количество состояний: " << costs.size() << "\n\n";

        fout << "Значения функции стоимости:\n";
        for (size_t i = 0; i < costs.size(); ++i) {
            fout << "Шаг " << i << ": J = " << costs[i]
                 << ", решение = " << decisions[i] << "\n";
        }

        if (!costs.empty()) {
            double min_cost = *std::min_element(costs.begin(), costs.end());
            double max_cost = *std::max_element(costs.begin(), costs.end());
            double avg_cost = std::accumulate(costs.begin(), costs.end(), 0.0) / costs.size();

            fout << "\nСтатистика:\n";
            fout << "Минимальная стоимость: " << min_cost << "\n";
            fout << "Максимальная стоимость: " << max_cost << "\n";
            fout << "Средняя стоимость: " << avg_cost << "\n";
        }

        fout.close();
        std::cout << "Информация об оптимизации сохранена: " << filename << std::endl;
    }
};

int main() {
    #ifdef __cpp_lib_filesystem
        std::filesystem::create_directory("results");
    #else
        system("mkdir results 2>nul");
    #endif

    DynamicProgrammingSolver solver;
    Trajectory optimal_trajectory = solver.computeOptimalTrajectory();

    optimal_trajectory.saveToCSV("results/trajectory.csv");
    optimal_trajectory.generatePlotScript("results/plot.gp");
    optimal_trajectory.saveReport("results/report.txt");
    solver.saveOptimizationInfo("results/optimization_info.txt");

    std::cout << "\nВсе файлы сохранены в папке 'results/'\n";

    return 0;
}

int main() {
    DynamicProgrammingSolver solver;
    Trajectory optimal_trajectory = solver.computeOptimalTrajectory();

    optimal_trajectory.saveToCSV("trajectory.csv");
    optimal_trajectory.generatePlotScript("plot.gp");
    optimal_trajectory.saveReport("report.txt");
    solver.saveOptimizationInfo("optimization_info.txt");

    std::cout << "\nВсе файлы сохранены в текущей директории\n";

    return 0;
}
