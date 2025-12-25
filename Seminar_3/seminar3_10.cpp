#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point {
    double x, y, z;

    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    void print() const {
        cout << "(" << fixed << setprecision(2) << x << ", " << y << ", " << z << ")";
    }
};

class Rocket {
private:
    double x, y, z;
    double vx, vy, vz;
    double ax, ay, az;
    bool isFalling;
    double fuel;
    string name;

public:
    Rocket(string name = "Rocket",
           double start_x = 0, double start_y = 0, double start_z = 0,
           double start_vx = 0, double start_vy = 0, double start_vz = 0,
           double fuel_amount = 100)
        : name(name), x(start_x), y(start_y), z(start_z),
          vx(start_vx), vy(start_vy), vz(start_vz),
          ax(0), ay(0), az(-9.8),
          isFalling(false), fuel(fuel_amount) {}
    void updatePosition(double dt) {
        if (isFalling) return;
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;

        fuel -= dt * 0.5;
        if (z <= 0) {
            z = 0;
            isFalling = true;
            vx = vy = vz = 0;
            cout << name << " упала на землю!" << endl;
        }
        if (fuel <= 0) {
            ax = ay = 0;
            az = -9.8;
            fuel = 0;
        }
    }
    void changeVelocity(double dvx, double dvy, double dvz) {
        if (!isFalling && fuel > 0) {
            vx += dvx;
            vy += dvy;
            vz += dvz;
        }
    }
    void setAcceleration(double new_ax, double new_ay, double new_az) {
        if (!isFalling && fuel > 0) {
            ax = new_ax;
            ay = new_ay;
            az = new_az;
        }
    }
    Point getPosition() const {
        return Point(x, y, z);
    }

    Point getVelocity() const {
        return Point(vx, vy, vz);
    }

    double getHeight() const {
        return z;
    }

    double getFuel() const {
        return fuel;
    }

    bool isFallingNow() const {
        return isFalling;
    }

    string getName() const {
        return name;
    }

    void printStatus() const {
        cout << name << ": ";
        cout << "Позиция = ";
        Point pos = getPosition();
        pos.print();
        cout << ", Скорость = ";
        Point vel = getVelocity();
        vel.print();
        cout << ", Высота = " << fixed << setprecision(2) << z << " м";
        cout << ", Топливо = " << fixed << setprecision(1) << fuel << " ед.";
        if (isFalling) cout << " (Упала)";
        cout << endl;
    }
};

int main() {
    vector<Rocket> squad;
    squad.reserve(5);
    cout << "Инициализация отряда ракет:" << endl;
    cout << "------------------------------------------------------" << endl;
    squad.emplace_back("Ракета-1", 0, 0, 0, 0, 0, 50, 120);
    squad.back().setAcceleration(0, 0, 15);
    squad.emplace_back("Ракета-2", 0, 0, 100, 30, 0, 40, 100);
    squad.back().setAcceleration(5, 0, 10);
    squad.emplace_back("Ракета-3", -50, 0, 50, 20, 10, 45, 80);
    squad.back().setAcceleration(3, 2, 12);
    squad.emplace_back("Ракета-4", 0, -30, 80, 25, -5, 35, 90);
    squad.back().setAcceleration(4, 1, 8);
    squad.emplace_back("Ракета-5", 30, 20, 0, 5, 3, 55, 110);
    squad.back().setAcceleration(1, 1, 18);
    for (size_t i = 0; i < squad.size(); i++) {
        squad[i].printStatus();
    }

    vector<vector<Point>> trajectories(5);
    for (size_t i = 0; i < squad.size(); i++) {
        trajectories[i].push_back(squad[i].getPosition());
    }
    const double T = 10.0;
    const double dt = 0.1;
    int steps = static_cast<int>(T / dt);

    cout << endl << "Начало симуляции:" << endl;
    cout << "Время: " << T << " секунд, шаг: " << dt << " секунд" << endl;
    cout << "Количество шагов: " << steps << endl << endl;

    for (int step = 0; step <= steps; step++) {
        double current_time = step * dt;

        for (size_t i = 0; i < squad.size(); i++) {
            if (!squad[i].isFallingNow()) {
                squad[i].updatePosition(dt);

                if (step % 5 == 0) {
                    trajectories[i].push_back(squad[i].getPosition());
                }

                if (step == 20) {
                    if (i == 1) squad[i].changeVelocity(10, 0, 0);
                    if (i == 2) squad[i].setAcceleration(0, 5, 5);
                }

                if (step == 40) {
                    if (i == 3) squad[i].changeVelocity(0, 8, 10);
                }
            }
        }

        if (step % 20 == 0 && step > 0) {
            cout << "Время: " << fixed << setprecision(1) << current_time << " с" << endl;
            for (size_t i = 0; i < squad.size(); i++) {
                if (!squad[i].isFallingNow()) {
                    cout << "  " << squad[i].getName() << ": высота = "
                         << fixed << setprecision(2) << squad[i].getHeight()
                         << " м, топливо = " << squad[i].getFuel() << " ед." << endl;
                }
            }
            cout << endl;
        }
    }

    cout << setw(12) << "Ракета"
         << setw(20) << "Позиция (x,y,z)"
         << setw(15) << "Высота"
         << setw(15) << "Топливо"
         << setw(15) << "Состояние" << endl;
    cout << string(80, '-') << endl;

    for (size_t i = 0; i < squad.size(); i++) {
        Point pos = squad[i].getPosition();

        cout << setw(12) << squad[i].getName()
             << setw(10) << "(" << fixed << setprecision(2)
             << setw(6) << pos.x << ", "
             << setw(6) << pos.y << ", "
             << setw(6) << pos.z << ")"
             << setw(15) << fixed << setprecision(2) << pos.z << " м"
             << setw(15) << fixed << setprecision(1) << squad[i].getFuel() << " ед.";

        if (squad[i].isFallingNow()) {
            cout << setw(15) << "Упала";
        } else {
            cout << setw(15) << "В полете";
        }
        cout << endl;
    }


    double max_height = 0;
    string max_height_rocket;

    for (size_t i = 0; i < squad.size(); i++) {
        double height = squad[i].getHeight();
        if (height > max_height) {
            max_height = height;
            max_height_rocket = squad[i].getName();
        }
    }

    cout << "Максимальная высота: " << fixed << setprecision(2)
         << max_height << " м (достигнута " << max_height_rocket << ")" << endl;

    int fallen_count = 0;
    for (size_t i = 0; i < squad.size(); i++) {
        if (squad[i].isFallingNow()) {
            fallen_count++;
        }
    }

    cout << "Упавших ракет: " << fallen_count << " из " << squad.size() << endl;

    cout << endl << "Информация о траекториях" << endl;
    for (size_t i = 0; i < trajectories.size(); i++) {
        cout << squad[i].getName() << ": "
             << trajectories[i].size() << " точек траектории" << endl;

        if (!trajectories[i].empty()) {
            cout << "  Начальная точка: ";
            trajectories[i].front().print();
            cout << endl;

            cout << "  Конечная точка: ";
            trajectories[i].back().print();
            cout << endl;
            if (trajectories[i].size() > 1) {
                Point max_point = trajectories[i][0];
                for (size_t j = 1; j < trajectories[i].size(); j++) {
                    if (trajectories[i][j].z > max_point.z) {
                        max_point = trajectories[i][j];
                    }
                }
                cout << "  Макс. высота: ";
                max_point.print();
                cout << " (высота = " << fixed << setprecision(2) << max_point.z << " м)" << endl;
            }
        }
        cout << endl;
    }

    for (size_t i = 0; i < trajectories.size(); i++) {
        if (trajectories[i].size() > 1) {
            double total_distance = 0;
            for (size_t j = 1; j < trajectories[i].size(); j++) {
                double dx = trajectories[i][j].x - trajectories[i][j-1].x;
                double dy = trajectories[i][j].y - trajectories[i][j-1].y;
                total_distance += sqrt(dx*dx + dy*dy);
            }
            cout << squad[i].getName() << ": общее горизонтальное расстояние = "
                 << fixed << setprecision(2) << total_distance << " м" << endl;
        }
    }

    cout << endl << "Симуляция завершена успешно!" << endl;

    return 0;
}
