#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

struct Waypoint {
    int id;
    double x, y, z;
    double speed;
    string description;
    bool reached;

    Waypoint(int id = 0, double x = 0.0, double y = 0.0, double z = 0.0,
            double speed = 0.0, const string& desc = "", bool reached = false)
        : id(id), x(x), y(y), z(z), speed(speed),
          description(desc), reached(reached) {}
};

class WaypointManager {
private:
    vector<Waypoint> waypoints;
    string filename;
    int currentWaypointIndex;

    double calculateDistance(const Waypoint& a, const Waypoint& b) const {
        double dx = b.x - a.x;
        double dy = b.y - a.y;
        double dz = b.z - a.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }

public:
    WaypointManager(const string& filename = "waypoints.txt")
        : filename(filename), currentWaypointIndex(0) {}

    void addWaypoint(int id, double x, double y, double z,
                    double speed, const string& desc) {
        for (const auto& wp : waypoints) {
            if (wp.id == id) {
                cout << "Точка маршрута с ID " << id << " уже существует!" << endl;
                return;
            }
        }

        waypoints.emplace_back(id, x, y, z, speed, desc, false);
        cout << "Точка маршрута '" << desc << "' (ID: " << id << ") добавлена." << endl;
    }

    bool saveRoute() {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << filename << endl;
            return false;
        }

        for (const auto& wp : waypoints) {
            fout << wp.id << ","
                 << fixed << setprecision(2)
                 << wp.x << ","
                 << wp.y << ","
                 << wp.z << ","
                 << wp.speed << ","
                 << wp.description << "\n";
        }

        fout.close();
        cout << "Маршрут сохранен в файл: " << filename << endl;
        return true;
    }

    bool loadRoute() {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Ошибка открытия файла для чтения: " << filename << endl;
            return false;
        }

        waypoints.clear();
        currentWaypointIndex = 0;
        string line;
        int loadedCount = 0;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            vector<string> tokens;

            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() == 6) {
                try {
                    int id = stoi(tokens[0]);
                    double x = stod(tokens[1]);
                    double y = stod(tokens[2]);
                    double z = stod(tokens[3]);
                    double speed = stod(tokens[4]);
                    string desc = tokens[5];

                    waypoints.emplace_back(id, x, y, z, speed, desc, false);
                    loadedCount++;
                } catch (const exception& e) {
                    cerr << "Ошибка преобразования данных: " << line << endl;
                }
            } else {
                cerr << "Некорректная строка: " << line << endl;
            }
        }

        fin.close();
        return loadedCount > 0;
    }

    double calculateTotalDistance() {
        double totalDistance = 0.0;

        for (size_t i = 1; i < waypoints.size(); ++i) {
            totalDistance += calculateDistance(waypoints[i-1], waypoints[i]);
        }

        return totalDistance;
    }

    Waypoint getNextWaypoint() {
        if (waypoints.empty()) {
            return Waypoint(0, 0, 0, 0, 0, "Нет точек ");
        }

        for (size_t i = currentWaypointIndex; i < waypoints.size(); ++i) {
            if (!waypoints[i].reached) {
                currentWaypointIndex = i;
                return waypoints[i];
            }
        }

        return waypoints.back();
    }

    bool checkWaypointReached(double x, double y, double z) {
        if (waypoints.empty() || currentWaypointIndex >= waypoints.size()) {
            return false;
        }

        Waypoint& current = waypoints[currentWaypointIndex];

        double distance = sqrt(pow(x - current.x, 2) +
                              pow(y - current.y, 2) +
                              pow(z - current.z, 2));

        if (distance < 10.0 && !current.reached) {
            current.reached = true;
            cout << "Точка маршрута '" << current.description
                 << "' достигнута!" << endl;

            currentWaypointIndex++;
            if (currentWaypointIndex >= waypoints.size()) {
                cout << "Маршрут завершен!" << endl;
            }

            return true;
        }

        return false;
    }

    void printRouteInfo() {
        if (waypoints.empty()) {
            cout << "Маршрут пуст." << endl;
            return;
        }

        cout << "\nИнформация о маршруте" << endl;
        cout << "Всего точек: " << waypoints.size() << endl;
        cout << "Общее расстояние: " << fixed << setprecision(1)
             << calculateTotalDistance() << " метров" << endl;

        if (currentWaypointIndex < waypoints.size()) {
            const auto& current = waypoints[currentWaypointIndex];
            cout << "Текущая точка: " << current.description
                 << " (" << fixed << setprecision(1)
                 << current.x << ", " << current.y << ", " << current.z << ")" << endl;
        }

        if (currentWaypointIndex + 1 < waypoints.size()) {
            const auto& next = waypoints[currentWaypointIndex + 1];
            cout << "Следующая точка: " << next.description
                 << " (" << fixed << setprecision(1)
                 << next.x << ", " << next.y << ", " << next.z << ")" << endl;
        } else if (currentWaypointIndex < waypoints.size()) {
            const auto& current = waypoints[currentWaypointIndex];
            if (!current.reached) {
                cout << "Следующая точка: " << current.description
                     << " (" << fixed << setprecision(1)
                     << current.x << ", " << current.y << ", " << current.z << ")" << endl;
            } else {
                cout << "Маршрут завершен!" << endl;
            }
        }
    }

    void printAllWaypoints() {
        if (waypoints.empty()) {
            cout << "Нет точек маршрута." << endl;
            return;
        }

        cout << "\ Все точки маршрута" << endl;
        for (const auto& wp : waypoints) {
            cout << "ID: " << wp.id
                 << ", Описание: " << wp.description
                 << ", Позиция: (" << fixed << setprecision(2)
                 << wp.x << ", " << wp.y << ", " << wp.z << ")"
                 << ", Скорость: " << wp.speed
                 << ", Достигнута: " << (wp.reached ? "Да" : "Нет") << endl;
        }
    }

    void resetRoute() {
        for (auto& wp : waypoints) {
            wp.reached = false;
        }
        currentWaypointIndex = 0;
        cout << "Маршрут сброшен. Все точки отмечены как недостигнутые." << endl;
    }
    int getWaypointCount() const {
        return waypoints.size();
    }
    int getCurrentIndex() const {
        return currentWaypointIndex;
    }
    const vector<Waypoint>& getAllWaypoints() const {
        return waypoints;
    }
};

void createInitialWaypointsFile() {
    ofstream fout("waypoints.txt");
    if (fout.is_open()) {
        fout << "1,0.0,0.0,100.0,25.0,Start\n";
        fout << "2,100.0,50.0,150.0,30.0,CheckpointA\n";
        fout << "3,200.0,100.0,200.0,35.0,CheckpointB\n";
        fout.close();
    }
}

int main() {
    createInitialWaypointsFile();
    WaypointManager manager("waypoints.txt");
    manager.loadRoute();
    manager.printRouteInfo();
    manager.printAllWaypoints();
    cout << "\nДобавление новой точки " << endl;
    manager.addWaypoint(4, 300.0, 150.0, 180.0, 40.0, "CheckpointC");
    manager.saveRoute();
    cout << "\nОбновление маршрута" << endl;
    manager.loadRoute();
    manager.printRouteInfo();
    Waypoint next = manager.getNextWaypoint();
    cout << "Следующая точка для достижения: " << next.description << endl;
    cout << "\nПроверка достижения точек:" << endl;
    cout << "Позиция (5.0, 5.0, 105.0) - близко к Start: ";
    if (manager.checkWaypointReached(5.0, 5.0, 105.0)) {
        cout << "Точка достигнута!" << endl;
    } else {
        cout << "Не достигнута" << endl;
    }

    manager.printRouteInfo();

    cout << "\nДвижения по маршруту " << endl;
    manager.resetRoute();
    vector<tuple<double, double, double>> testPositions = {
        {0.0, 0.0, 100.0},
        {100.0, 50.0, 150.0},
        {200.0, 100.0, 200.0}
    };

    return 0;
}
