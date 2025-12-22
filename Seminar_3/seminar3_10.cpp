#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
using namespace std;

class Rocket {
private:
    double x, y, z;
    double v_x, v_y, v_z;
    bool fall;

public:
    Rocket(double start_x, double start_y, double start_z,
           double start_v_x, double start_v_y, double start_v_z)
        : x(start_x), y(start_y), z(start_z),
          v_x(start_v_x), v_y(start_v_y), v_z(start_v_z),
          fall(false) {}

    void updatePosition(double dt) {
        if (fall) return;
        x += v_x * dt;
        y += v_y * dt;
        z += v_z * dt;
        if (z < 0) {
            z = 0;
            fall = true;
            v_x = 0;
            v_y = 0;
            v_z = 0;
        }
    }

    void changeVelocity(double dv_x, double dv_y, double dv_z) {
        if (!fall) {
            v_x += dv_x;
            v_y += dv_y;
            v_z += dv_z;
        }
    }

    double get_X() const { return x; }
    double get_Y() const { return y; }
    double get_Z() const { return z; }
    bool getfall() const { return fall; }

    void printPosition() const {
        std::cout << "(" << fixed << setprecision(2) << x << ", " << y << ", " << z << ")";
        if (fall) {
            cout << " (упала)";
        }
    }
};

double calculateTotalPath(const vector<double>& coordinates) {
    if (coordinates.size() < 2) {
        return 0.0;
    }

    vector<double> differences(coordinates.size());
    adjacent_difference(coordinates.begin(), coordinates.end(), differences.begin());

    double total_path = accumulate(differences.begin() + 1, differences.end(), 0.0,
        [](double sum, double diff) {
            return sum + abs(diff);
        });

    return total_path;
}

double calculateTotalPath3D(const vector<double>& x_coords,
                           const vector<double>& y_coords,
                           const vector<double>& z_coords) {
    if (x_coords.size() < 2 || y_coords.size() < 2 || z_coords.size() < 2) {
        return 0.0;
    }

    double total_path_3d = 0.0;
    for (size_t i = 1; i < x_coords.size(); i++) {
        double dx = x_coords[i] - x_coords[i-1];
        double dy = y_coords[i] - y_coords[i-1];
        double dz = z_coords[i] - z_coords[i-1];
        double distance = sqrt(dx*dx + dy*dy + dz*dz);
        total_path_3d += distance;
    }

    return total_path_3d;
}

void findMaxHeightForRocket(int rocketIndex, const vector<double>& times, const vector<double>& heights) {
    cout << "\nРакета" << rocketIndex + 1 << endl;

    if (heights.empty()) {
        cout << "Нет данных о высотах." << endl;
        return;
    }
    auto max_it = max_element(heights.begin(), heights.end());

    if (max_it != heights.end()) {
        size_t max_index = distance(heights.begin(), max_it);
        double max_height = *max_it;
        double time_at_max = times[max_index];

        cout << "Максимальная высота: " << fixed << setprecision(2)
             << max_height << endl;
        cout << "Момент времени: " << fixed << setprecision(2)
             << time_at_max << endl;
        if (max_index == heights.size() - 1 && max_height == 0) {
            cout << "Ракета не взлетела." << endl;
        }
    }
}

void countZeroAccelerations(const vector<vector<double>>& acceleration_data) {
    vector<vector<double>> sensor_data = {
        {2.5, 3.1, 0.0, 2.8, 0.0, 2.9, 3.0, 0.0, 2.7, 3.2},
        {1.2, 0.0, 1.3, 1.4, 1.1, 0.0, 0.0, 1.5, 1.6, 1.2},
        {9.8, 9.7, 0.0, 9.6, 9.8, 9.7, 0.0, 0.0, 9.8, 9.6},
        {2.1, 2.3, 2.0, 0.0, 2.2, 2.4, 0.0, 2.1, 2.3, 2.0},
        {0.0, 0.0, 0.0, 0.0, 1.1, 1.2, 1.0, 1.3, 1.4, 1.2}
    };

    for (size_t sensor_idx = 0; sensor_idx < sensor_data.size(); sensor_idx++) {
        const vector<double>& sensor_readings = sensor_data[sensor_idx];

        int zero_count = count(sensor_readings.begin(), sensor_readings.end(), 0.0);
        double percentage = (static_cast<double>(zero_count) / sensor_readings.size()) * 100;
    }
}

int main() {
    std::vector<Rocket> squad;
    squad.reserve(5);
    squad.emplace_back(-5, 0, 50, 100, 10, 50);
    squad.emplace_back(0, 0, 0, 90, 15, 40);
    squad.emplace_back(0, 0, -10, 110, -5, 60);
    squad.emplace_back(0, 0, 0, 95, 12, -45);
    squad.emplace_back(0, 0, 0, -105, -8, 55);

    vector<vector<double>> all_times(5);
    vector<vector<double>> all_heights(5);
    vector<vector<double>> all_x_coordinates(5);
    vector<vector<double>> all_y_coordinates(5);
    vector<vector<double>> all_z_coordinates(5);
    vector<vector<double>> acceleration_data(5);

    const double dt = 0.1;
    const double T = 10.0;

    for (double t = 0; t <= T; t += dt) {
        for (size_t i = 0; i < squad.size(); i++) {
            all_times[i].push_back(t);
            all_heights[i].push_back(squad[i].get_Z());
            all_x_coordinates[i].push_back(squad[i].get_X());
            all_y_coordinates[i].push_back(squad[i].get_Y());
            all_z_coordinates[i].push_back(squad[i].get_Z());
            double acceleration = 9.8;
            if (static_cast<int>(t * 10) % 7 == 0) {
                acceleration = 0.0;
            }
            acceleration_data[i].push_back(acceleration);
            squad[i].updatePosition(dt);
            if (squad[i].getfall() && all_heights[i].back() > 0) {
                double fallTime = t - dt + (all_heights[i].back() /
                              (all_heights[i].back() - squad[i].get_Z())) * dt;
                cout << "Время " << fixed << setprecision(1) << t
                     << " с: Ракета " << i+1 << " упала в момент ~"
                     << setprecision(2) << fallTime << " с\n";
            }
        }
    }

    cout << setw(19) << "X" << setw(15) << "Y"
         << setw(15) << "Z" << setw(25) << "Состояние" << endl;
    cout << string(75, '-') << endl;
    for (size_t i = 0; i < squad.size(); i++) {
        cout << setw(10) << "Ракета " << i+1 << setw(15) << fixed << setprecision(2) << squad[i].get_X()
             << setw(15) << squad[i].get_Y() << setw(15) << squad[i].get_Z();
        if (squad[i].getfall()) {
            cout << setw(18) << "Упала";
        } else {
            cout << setw(20) << "В полете";
        }
        cout << endl;
    }

    cout << "\n\nМаксимальная высота ракеты (доп задача 1)" << endl;
    for (size_t i = 0; i < squad.size(); i++) {
        findMaxHeightForRocket(i, all_times[i], all_heights[i]);
    }

    double global_max_height = 0;
    int global_max_rocket = -1;
    double global_max_time = 0;

    for (size_t i = 0; i < all_heights.size(); i++) {
        if (!all_heights[i].empty()) {
            auto max_it = max_element(all_heights[i].begin(), all_heights[i].end());
            if (max_it != all_heights[i].end()) {
                double max_height = *max_it;
                if (max_height > global_max_height) {
                    global_max_height = max_height;
                    global_max_rocket = i;
                    size_t max_index = distance(all_heights[i].begin(), max_it);
                    global_max_time = all_times[i][max_index];
                }
            }
        }
    }

    countZeroAccelerations(acceleration_data);
    cout << "\nПодсчет «нулевых» измерений датчика (доп задача 2)" << endl;
    for (size_t i = 0; i < all_heights.size(); i++) {
        if (!all_heights[i].empty()) {
            int zero_height_count = count(all_heights[i].begin(), all_heights[i].end(), 0.0);

            cout << "Ракета " << i+1 << ": ";
            cout << zero_height_count << endl;
        }
    }

    for (size_t i = 0; i < all_x_coordinates.size(); i++) {
        if (!all_x_coordinates[i].empty() &&
            !all_y_coordinates[i].empty() &&
            !all_z_coordinates[i].empty()) {
            double total_path_x = calculateTotalPath(all_x_coordinates[i]);
            double total_path_y = calculateTotalPath(all_y_coordinates[i]);
            double total_path_z = calculateTotalPath(all_z_coordinates[i]);
            double total_path_3d = calculateTotalPath3D(
                all_x_coordinates[i],
                all_y_coordinates[i],
                all_z_coordinates[i]
            );
        }
    }

    cout << "\nСуммарный путь ракеты (доп задача 4):" << endl;
    vector<double> total_paths_3d;
    vector<double> total_paths_sum_axes;
    vector<double> efficiencies_3d;

    for (size_t i = 0; i < all_x_coordinates.size(); i++) {
        if (!all_x_coordinates[i].empty()) {
            double path_3d = calculateTotalPath3D(
                all_x_coordinates[i],
                all_y_coordinates[i],
                all_z_coordinates[i]
            );
            double path_x = calculateTotalPath(all_x_coordinates[i]);
            double path_y = calculateTotalPath(all_y_coordinates[i]);
            double path_z = calculateTotalPath(all_z_coordinates[i]);
            double path_sum = path_x + path_y + path_z;

            total_paths_3d.push_back(path_3d);
            total_paths_sum_axes.push_back(path_sum);

            cout << "Ракета " << i+1 << ": "
                 << "Суммарный путь ракеты = " << fixed << setprecision(1) << path_3d;

            if (squad[i].getfall()) {
                cout << " (упала)";
            }
            cout << endl;
        }
    }

    if (!total_paths_3d.empty()) {
        auto max_3d_it = max_element(total_paths_3d.begin(), total_paths_3d.end());
        auto min_3d_it = min_element(total_paths_3d.begin(), total_paths_3d.end());

        if (!efficiencies_3d.empty()) {
            auto max_eff_it = max_element(efficiencies_3d.begin(), efficiencies_3d.end());
            auto min_eff_it = min_element(efficiencies_3d.begin(), efficiencies_3d.end());

            size_t max_eff_idx = distance(efficiencies_3d.begin(), max_eff_it);
            size_t min_eff_idx = distance(efficiencies_3d.begin(), min_eff_it);
        }

        size_t max_3d_idx = distance(total_paths_3d.begin(), max_3d_it);
        size_t min_3d_idx = distance(total_paths_3d.begin(), min_3d_it);
    }

    return 0;
}
