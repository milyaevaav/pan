#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

class Navigator {
public:
    vector<double> t;
    vector<double> x;
    vector<double> y;
    void createTestFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка создания файла " << filename << endl;
            return;
        }
        file << "t,x,y\n";
        file << "0,0,0\n";
        file << "1,2,1\n";
        file << "2,3.8,1.5\n";
        file << "3,6.1,2.2\n";
        file << "4,8.0,3.0\n";
        file << "5,10.5,4.1\n";

        file.close();
        cout << "Файл " << filename << " создан с тестовыми данными" << endl;
    }

    bool loadFromFile(const string& filename) {
        ifstream check_file(filename);
        if (!check_file.is_open()) {
            createTestFile(filename);
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
            size_t comma1 = line.find(',');
            size_t comma2 = line.find(',', comma1 + 1);

            if (comma1 != string::npos && comma2 != string::npos) {
                t.push_back(atof(line.substr(0, comma1).c_str()));
                x.push_back(atof(line.substr(comma1 + 1, comma2 - comma1 - 1).c_str()));
                y.push_back(atof(line.substr(comma2 + 1).c_str()));
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
    vector<double> computeVelocityX() const {
        vector<double> vx;

        if (t.size() < 2) return vx;

        auto calcVx = [](double x1, double x2, double t1, double t2) -> double {
            return (t2 != t1) ? (x2 - x1) / (t2 - t1) : 0.0;
        };

        for (size_t i = 1; i < t.size(); ++i) {
            vx.push_back(calcVx(x[i-1], x[i], t[i-1], t[i]));
        }

        return vx;
    }

    vector<double> computeVelocityY() const {
        vector<double> vy;

        if (t.size() < 2) return vy;

        auto calcVy = [](double y1, double y2, double t1, double t2) -> double {
            return (t2 != t1) ? (y2 - y1) / (t2 - t1) : 0.0;
        };

        for (size_t i = 1; i < t.size(); ++i) {
            vy.push_back(calcVy(y[i-1], y[i], t[i-1], t[i]));
        }

        return vy;
    }

    vector<double> computeTotalVelocity() const {
        vector<double> vx = computeVelocityX();
        vector<double> vy = computeVelocityY();
        vector<double> v;

        if (vx.empty() || vy.empty()) return v;

        auto calcV = [](double vx_val, double vy_val) -> double {
            return sqrt(vx_val * vx_val + vy_val * vy_val);
        };

        size_t n = min(vx.size(), vy.size());
        for (size_t i = 0; i < n; ++i) {
            v.push_back(calcV(vx[i], vy[i]));
        }

        return v;
    }

    bool saveVelocityToFile(const string& filename = "velocity.csv") {
        vector<double> v = computeTotalVelocity();

        if (v.empty()) {
            cerr << "Ошибка: не удалось вычислить скорость" << endl;
            return false;
        }

        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка создания файла " << filename << endl;
            return false;
        }

        file << "t,v\n";
        for (size_t i = 0; i < v.size(); ++i) {
            double time_mid = (t[i] + t[i+1]) / 2.0;
            file << time_mid << "," << v[i] << "\n";
        }

        file.close();
        cout << "Скорость сохранена в файл " << filename << endl;
        return true;
    }

    void plotVelocityGraph() {
        if (!saveVelocityToFile()) {
            return;
        }

        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            cerr << "Ошибка при запуске gnuplot" << endl;
            return;
        }
        fprintf(gp, "set terminal qt\n");
        fprintf(gp, "set title 'График скорости v(t)'\n");
        fprintf(gp, "set xlabel 'Время t, с'\n");
        fprintf(gp, "set ylabel 'Скорость v, м/с'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set style line 1 lc rgb 'red' lw 2 pt 7\n");
        fprintf(gp, "set datafile separator ','\n");
        fprintf(gp, "plot 'velocity.csv' using 1:2 with linespoints ls 1 title 'v(t)'\n");

        fflush(gp);

        cout << "График скорости построен. Нажмите Enter для выхода..." << endl;
        cin.get();

        pclose(gp);
    }
    void plotTrajectoryGraph() {
        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            cerr << "Ошибка при запуске gnuplot" << endl;
            return;
        }
        fprintf(gp, "set terminal qt\n");
        fprintf(gp, "set title 'Траектория движения (x,y)'\n");
        fprintf(gp, "set xlabel 'Координата X, м'\n");
        fprintf(gp, "set ylabel 'Координата Y, м'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set style line 1 lc rgb 'blue' lw 2 pt 7\n");

        fprintf(gp, "plot '-' using 1:2 with linespoints ls 1 title 'Траектория'\n");

        for (size_t i = 0; i < x.size(); i++) {
            fprintf(gp, "%f %f\n", x[i], y[i]);
        }

        fprintf(gp, "e\n");
        fflush(gp);

        cout << "График траектории построен. Нажмите Enter для выхода..." << endl;
        cin.get();

        pclose(gp);
    }

    void printInfo() const {
        cout << "\nИноформация о положении" << endl;
        cout << "Количество точек: " << t.size() << endl;

        if (!t.empty()) {
            cout << "Время: от " << t[0] << " до " << t.back() << " с" << endl;
            cout << "X: от " << x[0] << " до " << x.back() << " м" << endl;
            cout << "Y: от " << y[0] << " до " << y.back() << " м" << endl;
        }

        vector<double> v = computeTotalVelocity();
        if (!v.empty()) {
            double v_avg = 0, v_max = v[0], v_min = v[0];
            for (double vi : v) {
                v_avg += vi;
                if (vi > v_max) v_max = vi;
                if (vi < v_min) v_min = vi;
            }
            v_avg /= v.size();
            cout << "Скорость: min=" << v_min << ", max=" << v_max
                 << ", avg=" << v_avg << " м/с" << endl;
        }
    }
};

int main() {
    Navigator nav;
    if (!nav.loadFromFile("navigation.csv")) {
        return 1;
    }
    nav.printInfo();
    cout << "\n2. Вычисление скорости..." << endl;
    nav.saveVelocityToFile();
    cout << "\n3. Построение графиков..." << endl;
    cout << "Выберите график:" << endl;
    cout << "1 - График скорости v(t)" << endl;
    cout << "2 - График траектории (x,y)" << endl;
    cout << "3 - Оба графика" << endl;
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            nav.plotVelocityGraph();
            break;
        case 2:
            nav.plotTrajectoryGraph();
            break;
        case 3:
            nav.plotVelocityGraph();
            nav.plotTrajectoryGraph();
            break;
        default:
            cout << "-----------" << endl;
            break;
    }

    return 0;
}
