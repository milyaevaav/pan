#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

class SensorData {
private:
    vector<double> t;
    vector<double> h1;
    vector<double> h2;
    vector<double> dh;

public:
    void createTestFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка создания файла " << filename << endl;
            return;
        }

        file << "t,h1,h2\n";
        file << "0,100.0,102.5\n";
        file << "1,105.2,104.8\n";
        file << "2,110.1,109.9\n";
        file << "3,115.3,116.0\n";
        file << "4,120.5,121.2\n";
        file << "5,125.0,124.5\n";

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
        int line_num = 0;
        while (getline(file, line)) {
            line_num++;

            vector<string> tokens;
            size_t start = 0, end = 0;

            while ((end = line.find(',', start)) != string::npos) {
                tokens.push_back(line.substr(start, end - start));
                start = end + 1;
            }
            tokens.push_back(line.substr(start));

            if (tokens.size() == 3) {
                t.push_back(atof(tokens[0].c_str()));
                h1.push_back(atof(tokens[1].c_str()));
                h2.push_back(atof(tokens[2].c_str()));
            } else {
                cerr << "Предупреждение: строка " << line_num << " пропущена: " << line << endl;
            }
        }

        file.close();

        if (t.size() < 2) {
            cerr << "Ошибка: недостаточно данных (нужно минимум 2 точки)" << endl;
            return false;
        }

        cout << "Загружено " << t.size() << " точек данных" << endl;
        return true;
    }
    void calculateDifference() {
        if (h1.size() != h2.size()) {
            cerr << "Ошибка: размеры массивов h1 и h2 не совпадают" << endl;
            return;
        }

        dh.clear();
        dh.reserve(h1.size());

        auto computeDelta = [](double a, double b) -> double {
            return fabs(a - b);
        };

        for (size_t i = 0; i < h1.size(); ++i) {
            dh.push_back(computeDelta(h1[i], h2[i]));
        }

        cout << "Вычислено " << dh.size() << " значений разницы Δh" << endl;
    }

    bool saveDifferenceToFile() {
        if (t.size() != dh.size()) {
            cerr << "Ошибка: данные не готовы для сохранения" << endl;
            return false;
        }

        ofstream file("diff.csv");
        if (!file.is_open()) {
            cerr << "Ошибка создания файла diff.csv" << endl;
            return false;
        }

        file << "t,dh\n";
        for (size_t i = 0; i < t.size(); ++i) {
            file << t[i] << "," << dh[i] << "\n";
        }

        file.close();
        cout << "Разница сохранена в файл diff.csv" << endl;
        return true;
    }
    void plotComparisonGraph() {
        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            cerr << "Ошибка при запуске gnuplot" << endl;
            return;
        }
        fprintf(gp, "set terminal qt\n");
        fprintf(gp, "set title 'Сравнение показаний датчиков'\n");
        fprintf(gp, "set xlabel 'Время t, с'\n");
        fprintf(gp, "set ylabel 'Значение'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set style line 1 lc rgb 'blue' lw 2 pt 7\n");
        fprintf(gp, "set style line 2 lc rgb 'red' lw 2 pt 5\n");
        fprintf(gp, "set style line 3 lc rgb 'green' lw 2 pt 9\n");
        fprintf(gp, "plot '-' using 1:2 with linespoints ls 1 title 'h1(t)', ");
        fprintf(gp, "'-' using 1:2 with linespoints ls 2 title 'h2(t)', ");
        fprintf(gp, "'diff.csv' using 1:2 with linespoints ls 3 title 'Δh(t)'\n");
        for (size_t i = 0; i < t.size(); i++) {
            fprintf(gp, "%f %f\n", t[i], h1[i]);
        }
        fprintf(gp, "e\n");
        for (size_t i = 0; i < t.size(); i++) {
            fprintf(gp, "%f %f\n", t[i], h2[i]);
        }
        fprintf(gp, "e\n");

        fflush(gp);

        cout << "График построен. Нажмите Enter для выхода..." << endl;
        cin.get();

        pclose(gp);
    }
    void plotDifferenceGraph() {
        FILE* gp = popen("gnuplot -persistent", "w");
        if (!gp) {
            cerr << "Ошибка при запуске gnuplot" << endl;
            return;
        }
        fprintf(gp, "set terminal qt\n");
        fprintf(gp, "set title 'Разница показаний датчиков Δh(t)'\n");
        fprintf(gp, "set xlabel 'Время t, с'\n");
        fprintf(gp, "set ylabel 'Δh = |h1 - h2|'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "set style line 1 lc rgb 'green' lw 2 pt 9\n");
        fprintf(gp, "plot 'diff.csv' using 1:2 with linespoints ls 1 title 'Δh(t)'\n");

        fflush(gp);

        cout << "График разницы построен. Нажмите Enter для выхода..." << endl;
        cin.get();

        pclose(gp);
    }
    void printData() const {
        cout << "\nДанные датчиков" << endl;
        cout << "t (с)\th1\th2\tΔh" << endl;

        for (size_t i = 0; i < t.size(); ++i) {
            cout << t[i] << "\t" << h1[i] << "\t" << h2[i] << "\t";
            if (i < dh.size()) {
                cout << dh[i];
            } else {
                cout << "N/A";
            }
            cout << endl;
        }
    }
};

int main() {
    SensorData sensor;
    cout << "1. Загрузка данных из sensors.csv..." << endl;
    if (!sensor.loadFromFile("sensors.csv")) {
        return 1;
    }
    cout << "2. Вычисление разницы Δh " << endl;
    sensor.calculateDifference();
    cout << "3. Сохранение разницы в файл" << endl;
    if (!sensor.saveDifferenceToFile()) {
        return 1;
    }

    sensor.printData();
    cout << "\n4. Построение графиков..." << endl;
    cout << "Выберите вариант:" << endl;
    cout << "1 - График сравнения (h1, h2, Δh)" << endl;
    cout << "2 - График разницы (только Δh)" << endl;
    cout << "3 - Оба графика" << endl;
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            sensor.plotComparisonGraph();
            break;
        case 2:
            sensor.plotDifferenceGraph();
            break;
        case 3:
            sensor.plotComparisonGraph();
            sensor.plotDifferenceGraph();
            break;
        default:
            cout << "------------" << endl;
            break;
    }

    return 0;
}
