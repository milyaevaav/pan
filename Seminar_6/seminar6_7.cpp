#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

class WaypointSorter {
private:
    struct Waypoint {
        int id;
        double x, y, z;
        std::string name;
        double distance;

        Waypoint(int i, double x_val, double y_val, double z_val, std::string n)
            : id(i), x(x_val), y(y_val), z(z_val), name(n), distance(0.0) {}
    };

    std::vector<Waypoint> waypoints;

public:
    bool loadWaypoints(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() != 5) {
                std::cerr << "Предупреждение: некорректная строка - " << line << std::endl;
                continue;
            }

            try {
                int id = std::stoi(tokens[0]);
                double x = std::stod(tokens[1]);
                double y = std::stod(tokens[2]);
                double z = std::stod(tokens[3]);
                std::string name = tokens[4];

                waypoints.emplace_back(id, x, y, z, name);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при парсинге строки: " << line << " - " << e.what() << std::endl;
                continue;
            }
        }

        file.close();

        if (waypoints.empty()) {
            std::cerr << "Файл не содержит данных или имеет неверный формат" << std::endl;
            return false;
        }

        std::cout << "Загружено " << waypoints.size() << " точек маршрута" << std::endl;
        return true;
    }
    void calculateDistances(double current_x, double current_y, double current_z) {
        std::cout << "Вычисление расстояний от текущей позиции ("
                  << current_x << ", " << current_y << ", " << current_z << ")..." << std::endl;

        for (auto& wp : waypoints) {
            double dx = wp.x - current_x;
            double dy = wp.y - current_y;
            double dz = wp.z - current_z;

            wp.distance = std::sqrt(dx*dx + dy*dy + dz*dz);
        }
    }

    void sortByDistance() {
        auto compareByDistance = [](const Waypoint& a, const Waypoint& b) {
            return a.distance < b.distance;
        };

        std::sort(waypoints.begin(), waypoints.end(), compareByDistance);

        std::cout << "Точки маршрута отсортированы по расстоянию" << std::endl;
    }

    bool saveSortedWaypoints(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
            return false;
        }
        file << "ID,X,Y,Z,Name,Distance" << std::endl;

        file << std::fixed << std::setprecision(2);

        for (const auto& wp : waypoints) {
            file << wp.id << ","
                 << wp.x << ","
                 << wp.y << ","
                 << wp.z << ","
                 << wp.name << ","
                 << wp.distance << std::endl;
        }

        file.close();

        if (!file.good()) {
            std::cerr << "Предупреждение: возможная ошибка при записи файла" << std::endl;
            return false;
        }

        std::cout << "Отсортированные точки сохранены в " << filename << std::endl;
        return true;
    }

    void printWaypoints(const std::string& title = "Точки маршрута") {
        std::cout << "\n=== " << title << " ===" << std::endl;

        if (waypoints.empty()) {
            std::cout << "Нет данных для отображения" << std::endl;
            return;
        }
        std::cout << std::left
                  << std::setw(5) << "ID"
                  << std::setw(10) << "X"
                  << std::setw(10) << "Y"
                  << std::setw(10) << "Z"
                  << std::setw(15) << "Название"
                  << std::setw(12) << "Расстояние"
                  << std::endl;

        std::cout << std::string(62, '-') << std::endl;

        std::cout << std::fixed << std::setprecision(2);
        for (const auto& wp : waypoints) {
            std::cout << std::left
                      << std::setw(5) << wp.id
                      << std::setw(10) << wp.x
                      << std::setw(10) << wp.y
                      << std::setw(10) << wp.z
                      << std::setw(15) << wp.name
                      << std::setw(12) << wp.distance
                      << std::endl;
        }
    }

    Waypoint* findNearestWaypoint() {
        if (waypoints.empty()) {
            return nullptr;
        }

        auto minIt = std::min_element(waypoints.begin(), waypoints.end(),
            [](const Waypoint& a, const Waypoint& b) {
                return a.distance < b.distance;
            });

        return &(*minIt);
    }

    Waypoint* findFarthestWaypoint() {
        if (waypoints.empty()) {
            return nullptr;
        }

        auto maxIt = std::max_element(waypoints.begin(), waypoints.end(),
            [](const Waypoint& a, const Waypoint& b) {
                return a.distance < b.distance;
            });

        return &(*maxIt);
    }
    double getAverageDistance() {
        if (waypoints.empty()) {
            return 0.0;
        }

        double sum = 0.0;
        for (const auto& wp : waypoints) {
            sum += wp.distance;
        }

        return sum / waypoints.size();
    }
};

int main() {
    WaypointSorter sorter;
    std::cout << "Загрузка точек маршрута из waypoints.txt..." << std::endl;
    if (!sorter.loadWaypoints("waypoints.txt")) {
        std::cerr << "Не удалось загрузить точки маршрута" << std::endl;
        return 1;
    }

    sorter.printWaypoints("ИСХОДНЫЕ ТОЧКИ МАРШРУТА");
    double current_x = 800.0, current_y = 600.0, current_z = 1800.0;
    sorter.calculateDistances(current_x, current_y, current_z);
    sorter.sortByDistance();
    sorter.printWaypoints("ОТСОРТИРОВАННЫЕ ТОЧКИ МАРШРУТА");\
    std::cout << "\nСохранение отсортированных точек в файл..." << std::endl;
    if (!sorter.saveSortedWaypoints("sorted_waypoints.csv")) {
        std::cerr << "Не удалось сохранить отсортированные точки" << std::endl;
        return 1;
    }

    std::cout << "\nСТАТИСТИКА" << std::endl;

    auto* nearest = sorter.findNearestWaypoint();
    if (nearest) {
        std::cout << "Ближайшая точка: " << nearest->name
                  << " (расстояние: " << std::fixed << std::setprecision(2) << nearest->distance << ")" << std::endl;
    }

    auto* farthest = sorter.findFarthestWaypoint();
    if (farthest) {
        std::cout << "Дальняя точка: " << farthest->name
                  << " (расстояние: " << farthest->distance << ")" << std::endl;
    }

    double avgDistance = sorter.getAverageDistance();
    std::cout << "Среднее расстояние: " << avgDistance << std::endl;
    std::cout << "\n=== ПЛАН ПОЛЕТА (последовательность посещения) ===" << std::endl;
    std::cout << "Текущая позиция -> ";

    std::cout << "Текущая позиция (" << current_x << ", " << current_y << ", " << current_z << ")" << std::endl;
    return 0;
}
