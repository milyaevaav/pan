#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cmath>

class Trajectory {
private:
    std::vector<double> t;
    std::vector<double> x;

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
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() != 2) {
                std::cerr << "Предупреждение: некорректная строка — пропущена: " << line << std::endl;
                continue;
            }

            try {
                double time_val = std::stod(tokens[0]);
                double x_val = std::stod(tokens[1]);

                t.push_back(time_val);
                x.push_back(x_val);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при парсинге строки: " << line << " — " << e.what() << std::endl;
                continue;
            }
        }

        file.close();
        std::cout << "Загружено " << t.size() << " точек траектории." << std::endl;
        return true;
    }

    std::vector<double> calculateVelocities() const {
        if (t.size() < 2) {
            std::cerr << "Ошибка: недостаточно точек для вычисления скорости." << std::endl;
            return {};
        }

        std::vector<double> velocities;
        velocities.reserve(t.size() - 1);

        for (size_t i = 0; i < t.size() - 1; ++i) {
            double dt = t[i + 1] - t[i];
            if (dt == 0) {
                std::cerr << "Ошибка: нулевой интервал времени между точками " << i << " и " << i + 1 << std::endl;
                continue;
            }
            double v = (x[i + 1] - x[i]) / dt;
            velocities.push_back(v);
        }

        std::cout << "Вычислено " << velocities.size() << " значений скорости." << std::endl;
        return velocities;
    }

    void plotWithGnuplot() const {
        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            std::cerr << "Ошибка при запуске gnuplot." << std::endl;
            return;
        }
        fprintf(gp, "set terminal qt size 800,600\n");
        fprintf(gp, "set title 'Траектория x(t) и скорость v(t)'\n");
        fprintf(gp, "set xlabel 'Время t (с)'\n");
        fprintf(gp, "set ylabel 'Координата x (м)'\n");
        fprintf(gp, "set y2label 'Скорость v (м/с)'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set key right top\n");
        fprintf(gp, "set ytics nomirror\n");
        fprintf(gp, "set y2tics\n");

        fprintf(gp, "plot '-' using 1:2 with linespoints title 'x(t)' lw 2 pt 7, \\\n");
        fprintf(gp, "     '-' using 1:2 with lines title 'v(t)' axes x1y2 lw 2 dt 2\n");

        for (size_t i = 0; i < t.size(); ++i) {
            fprintf(gp, "%f %f\n", t[i], x[i]);
        }
        fprintf(gp, "e\n");
        std::vector<double> v = calculateVelocities();
        std::vector<double> t_mid;
        for (size_t i = 0; i < v.size(); ++i) {
            t_mid.push_back((t[i] + t[i + 1]) / 2.0);
        }

        for (size_t i = 0; i < v.size(); ++i) {
            fprintf(gp, "%f %f\n", t_mid[i], v[i]);
        }
        fprintf(gp, "e\n");

        fflush(gp);
        std::cout << "График построен. Закройте окно Gnuplot." << std::endl;
        pclose(gp);
    }

    void saveToCSVAndScript(const std::string& csv_filename, const std::string& script_filename) const {
        std::ofstream csv(csv_filename);
        if (!csv.is_open()) {
            std::cerr << "Ошибка при создании CSV файла." << std::endl;
            return;
        }
        csv << "t,x\n";
        for (size_t i = 0; i < t.size(); ++i) {
            csv << t[i] << "," << x[i] << "\n";
        }
        csv.close();
        std::ofstream script(script_filename);
        if (!script.is_open()) {
            std::cerr << "Ошибка при создании скрипта Gnuplot." << std::endl;
            return;
        }

        script << "set terminal png size 800,600\n";
        script << "set output 'trajectory_plot.png'\n";
        script << "set grid\n";
        script << "set xlabel 'Время t (с)'\n";
        script << "set ylabel 'Координата x (м)'\n";
        script << "set title 'Траектория x(t)'\n";
        script << "set datafile separator ','\n";
        script << "plot '" << csv_filename << "' using 1:2 with linespoints title 'x(t)' lw 2 pt 7\n";
        script.close();
    }

    void printData() const {
        std::cout << "\nДанные о траектории" << std::endl;
        for (size_t i = 0; i < t.size(); ++i) {
            std::cout << "t=" << t[i] << ", x=" << x[i] << std::endl;
        }
    }

    const std::vector<double>& getT() const { return t; }
    const std::vector<double>& getX() const { return x; }
};

int main() {
    Trajectory traj;
    if (!traj.loadFromFile("traj.csv")) {
        std::cerr << "Не удалось загрузить данные." << std::endl;
        return 1;
    }

    traj.printData();
    std::vector<double> velocities = traj.calculateVelocities();
    std::cout << "\nСкорости" << std::endl;
    for (size_t i = 0; i < velocities.size(); ++i) {
        std::cout << "v_" << i << " = " << velocities[i] << " м/с" << std::endl;
    }

    traj.plotWithGnuplot();

    return 0;
}
