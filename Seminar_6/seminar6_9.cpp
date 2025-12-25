#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

struct AircraftParams {
    double mass;
    double max_thrust;
    double wingspan;
    double fuel_capacity;
    double cruise_speed;
};

struct AtmospherePoint {
    double altitude;
    double density;
    double temperature;
    double pressure;
};

class Aircraft {
private:
    AircraftParams params;

public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream fin(filename);
        if (!fin.is_open()) {
            std::cerr << "Не удалось открыть файл с параметрами самолета: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(fin, line);

        if (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string value;

            std::getline(ss, value, ','); params.mass = std::stod(value);
            std::getline(ss, value, ','); params.max_thrust = std::stod(value);
            std::getline(ss, value, ','); params.wingspan = std::stod(value);
            std::getline(ss, value, ','); params.fuel_capacity = std::stod(value);
            std::getline(ss, value, ','); params.cruise_speed = std::stod(value);

            std::cout << "Параметры самолета загружены: масса=" << params.mass
                      << " кг, тяга=" << params.max_thrust << " Н" << std::endl;
            return true;
        }
        return false;
    }

    const AircraftParams& getParams() const { return params; }
};

class Environment {
private:
    std::vector<AtmospherePoint> atmosphere_table;

public:
    bool loadAtmosphereTable(const std::string& filename) {
        std::ifstream fin(filename);
        if (!fin.is_open()) {
            std::cerr << "Не удалось открыть файл с таблицей атмосферы: " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(fin, line);

        while (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string value;
            AtmospherePoint point;

            std::getline(ss, value, ','); point.altitude = std::stod(value);
            std::getline(ss, value, ','); point.density = std::stod(value);
            std::getline(ss, value, ','); point.temperature = std::stod(value);
            std::getline(ss, value, ','); point.pressure = std::stod(value);

            atmosphere_table.push_back(point);
        }

        std::cout << "Таблица атмосферы загружена: " << atmosphere_table.size() << " точек" << std::endl;
        return true;
    }

    double getDensity(double altitude) const {
        if (atmosphere_table.empty()) {
            std::cerr << "Таблица атмосферы не загружена!" << std::endl;
            return 1.225;
        }

        for (size_t i = 0; i < atmosphere_table.size() - 1; ++i) {
            double h1 = atmosphere_table[i].altitude;
            double d1 = atmosphere_table[i].density;
            double h2 = atmosphere_table[i + 1].altitude;
            double d2 = atmosphere_table[i + 1].density;

            if (altitude >= h1 && altitude <= h2) {
                return d1 + (d2 - d1) * (altitude - h1) / (h2 - h1);
            }
        }

        if (altitude < atmosphere_table.front().altitude)
            return atmosphere_table.front().density;
        else
            return atmosphere_table.back().density;
    }
};

class TrajectoryOptimizer {
private:
    std::vector<std::vector<double>> grid;
    std::vector<std::vector<int>> policy;
    int steps_h = 100;
    int steps_v = 100;
    double dt = 1.0;

public:
    void initializeGrid(double max_h, double max_v) {
        grid.resize(steps_h, std::vector<double>(steps_v, 1e9));
        policy.resize(steps_h, std::vector<int>(steps_v, -1));
        grid[0][0] = 0;
    }

    double computeCost(double h1, double v1, double h2, double v2,
                       const AircraftParams& params, double density) {
        double dh = h2 - h1;
        double dv = v2 - v1;
        double thrust_needed = params.mass * dv / dt + 0.5 * density * v1 * v1 * 0.1;
        double fuel_cost = thrust_needed / (params.max_thrust * 0.1);
        return fuel_cost + 0.01 * std::abs(dh);
    }

    void solveDP(const Aircraft& aircraft, const Environment& env) {
        const AircraftParams& params = aircraft.getParams();

        for (int i = 0; i < steps_h - 1; ++i) {
            for (int j = 0; j < steps_v - 1; ++j) {
                double h1 = i * 100.0;
                double v1 = j * 10.0;
                double density = env.getDensity(h1);

                for (int di = 0; di <= 2; ++di) {
                    for (int dj = 0; dj <= 2; ++dj) {
                        int i2 = i + di;
                        int j2 = j + dj;

                        if (i2 < steps_h && j2 < steps_v) {
                            double h2 = i2 * 100.0;
                            double v2 = j2 * 10.0;

                            double cost = computeCost(h1, v1, h2, v2, params, density);
                            double new_cost = grid[i][j] + cost;

                            if (new_cost < grid[i2][j2]) {
                                grid[i2][j2] = new_cost;
                                policy[i2][j2] = i * steps_v + j;
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<std::pair<double, double>> getOptimalPath() {
        std::vector<std::pair<double, double>> path;

        int i = steps_h - 1;
        int j = steps_v - 1;

        while (i >= 0 && j >= 0 && policy[i][j] != -1) {
            path.push_back({i * 100.0, j * 10.0});
            int prev = policy[i][j];
            i = prev / steps_v;
            j = prev % steps_v;
        }

        path.push_back({0.0, 0.0});
        std::reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    Aircraft aircraft;
    if (!aircraft.loadFromFile("aircraft_params.csv")) {
        std::cerr << "Не удалось загрузить параметры самолета" << std::endl;
        return 1;
    }

    Environment env;
    if (!env.loadAtmosphereTable("atmosphere.csv")) {
        std::cerr << "Не удалось загрузить таблицу атмосферы" << std::endl;
        return 1;
    }

    TrajectoryOptimizer optimizer;
    optimizer.initializeGrid(10000, 1000);
    optimizer.solveDP(aircraft, env);

    auto optimal_path = optimizer.getOptimalPath();

    std::cout << "\nТочки оптимальной траектории:" << std::endl;
    for (size_t i = 0; i < std::min(optimal_path.size(), size_t(10)); ++i) {
        std::cout << "Точка " << i << ": высота=" << optimal_path[i].first
                  << " м, скорость=" << optimal_path[i].second << " м/с" << std::endl;
    }
    std::cout << "Всего точек: " << optimal_path.size() << std::endl;

    std::cout << "\nПлотность на высоте 5000 м: " << env.getDensity(5000) << " кг/м³" << std::endl;

    return 0;
}
