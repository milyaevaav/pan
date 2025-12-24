#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Trajectory {
private:
    vector<double> t;
    vector<double> x;

public:
    void createDataFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка создания файла " << filename << endl;
            return;
        }
        file << "t,x\n";
        file << "0,0\n";
        file << "1,2\n";
        file << "2,3.8\n";
        file << "3,6.1\n";
        file << "4,8.0\n";

        file.close();
        cout << "Файл " << filename << " создан с тестовыми данными" << endl;
    }

    bool loadFromFile(const string& filename) {
        ifstream check_file(filename);
        if (!check_file.is_open()) {
            createDataFile(filename);
        }
        check_file.close();
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }

        string line;
        getline(file, line);

        while (getline(file, line)) {
            size_t comma_pos = line.find(',');
            if (comma_pos != string::npos) {
                t.push_back(atof(line.substr(0, comma_pos).c_str()));
                x.push_back(atof(line.substr(comma_pos + 1).c_str()));
            }
        }

        file.close();

        if (t.size() < 2) {
            cerr << "Ошибка: недостаточно данных (нужно минимум 2 точки)" << endl;
            return false;
        }

        cout << "Загружено " << t.size() << " точек траектории" << endl;
        return true;
    }
    vector<double> calculateVelocities() const {
        vector<double> velocities;

        if (t.size() < 2) {
            cerr << "Ошибка: недостаточно точек для вычисления скорости" << endl;
            return velocities;
        }
        auto computeVelocity = [](double x_next, double x_curr, double t_next, double t_curr) {
            return (x_next - x_curr) / (t_next - t_curr);
        };и
        for (size_t i = 0; i < t.size() - 1; ++i) {
            velocities.push_back(computeVelocity(x[i + 1], x[i], t[i + 1], t[i]));
        }

        return velocities;
    }
    void plotGraph() const {
        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            cerr << "Ошибка при запуске gnuplot" << endl;
            return;
        }
        fprintf(gp, "set terminal qt\n");
        fprintf(gp, "set title 'График траектории x(t)'\n");
        fprintf(gp, "set xlabel 'Время t, с'\n");
        fprintf(gp, "set ylabel 'Координата x, м'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set style line 1 lc rgb 'blue' lw 2 pt 7\n");
        fprintf(gp, "plot '-' using 1:2 with linespoints ls 1 title 'x(t)'\n");
        for (size_t i = 0; i < t.size(); i++) {
            fprintf(gp, "%f %f\n", t[i], x[i]);
        }
        fprintf(gp, "e\n");
        fflush(gp);

        cout << "График построен. Нажмите Enter для выхода..." << endl;
        cin.get();

        pclose(gp);
    }
    void printData() const {
        cout << "\n=== Данные траектории ===" << endl;
        cout << "t (с)\tx (м)" << endl;

        for (size_t i = 0; i < t.size(); ++i) {
            cout << t[i] << "\t" << x[i] << endl;
        }
    }
    void printVelocities() const {
        vector<double> velocities = calculateVelocities();

        if (velocities.empty()) {
            cout << "Невозможно вычислить скорости" << endl;
            return;
        }

        cout << "\n      Скорости" << endl;
        for (size_t i = 0; i < velocities.size(); ++i) {
            cout << "Интервал [" << t[i] << ", " << t[i+1]
                 << "]: v = " << velocities[i] << " м/с" << endl;
        }
    }
};

int main() {
    cout << "Анализ траектории" << endl;
    Trajectory traj;
    if (!traj.loadFromFile("traj.csv")) {
        return 1;
    }
    traj.printData();
    traj.printVelocities();
    cout << "\nПостроение графика траектории" << endl;
    traj.plotGraph();

    return 0;
}
