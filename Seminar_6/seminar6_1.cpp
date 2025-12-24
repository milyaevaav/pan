#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

class TrajectoryLogger {
private:
    struct Point {
        double x, y, z, speed, time;
    };

    vector<Point> trajectory;
    string filename;

public:
    TrajectoryLogger(const string& filename) : filename(filename) {}

    void addPoint(double x, double y, double z, double speed, double time) {
        trajectory.push_back({x, y, z, speed, time});
    }
    bool saveToCSV() {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << filename << endl;
            return false;
        }
        fout << "time,x,y,z,speed\n";
        for (const auto& p : trajectory) {
            fout << fixed << setprecision(2)
                 << p.time << ","
                 << p.x << ","
                 << p.y << ","
                 << p.z << ","
                 << p.speed << "\n";
        }

        fout.close();
        cout << "Данные успешно сохранены в " << filename << endl;
        return true;
    }
    bool loadFromCSV() {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Ошибка открытия файла для чтения: " << filename << endl;
            return false;
        }

        trajectory.clear();
        string line;
        if (!getline(fin, line)) {
            cerr << "Файл пустой!" << endl;
            fin.close();
            return false;
        }
        int pointsLoaded = 0;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            Point p;
            if (getline(ss, token, ',')) p.time = stod(token);
            if (getline(ss, token, ',')) p.x = stod(token);
            if (getline(ss, token, ',')) p.y = stod(token);
            if (getline(ss, token, ',')) p.z = stod(token);
            if (getline(ss, token, ',')) p.speed = stod(token);

            trajectory.push_back(p);
            pointsLoaded++;
        }

        fin.close();
        cout << "Загружено " << pointsLoaded << " точек из " << filename << endl;
        return pointsLoaded > 0;
    }
    double calculateTotalDistance() {
        double total = 0.0;
        for (size_t i = 1; i < trajectory.size(); ++i) {
            const auto& p1 = trajectory[i-1];
            const auto& p2 = trajectory[i];
            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;
            double dz = p2.z - p1.z;
            total += sqrt(dx*dx + dy*dy + dz*dz);
        }
        return total;
    }
    double findMaxSpeed() {
        if (trajectory.empty()) return 0.0;

        double maxSpeed = trajectory[0].speed;
        for (const auto& p : trajectory) {
            if (p.speed > maxSpeed) maxSpeed = p.speed;
        }
        return maxSpeed;
    }

    void printStatistics() {
        cout << "Количество точек: " << trajectory.size() << endl;

        if (trajectory.size() >= 2) {
            cout << "Общее расстояние: " << calculateTotalDistance() << " м" << endl;
        }

        if (!trajectory.empty()) {
            cout << "Максимальная скорость: " << findMaxSpeed() << " м/с" << endl;

            double avgSpeed = 0.0;
            for (const auto& p : trajectory) avgSpeed += p.speed;
            avgSpeed /= trajectory.size();
            cout << "Средняя скорость: " << avgSpeed << " м/с" << endl;
        } else {
            cout << "Нет данных для статистики!" << endl;
        }
    }
    void printAllPoints() {
        if (trajectory.empty()) {
            cout << "Траектория пуста." << endl;
            return;
        }

        cout << "\nВсе точки траектории:" << endl;
        cout << "time, x, y, z, speed" << endl;
        for (const auto& p : trajectory) {
            cout << fixed << setprecision(2)
                 << p.time << ", "
                 << p.x << ", "
                 << p.y << ", "
                 << p.z << ", "
                 << p.speed << endl;
        }
    }
};

int main() {
    TrajectoryLogger logger("trajectory.csv");
    TrajectoryLogger logger2("trajectory.csv");
    if (logger2.loadFromCSV()) {
        logger2.printAllPoints();
    }

    return 0;
}
