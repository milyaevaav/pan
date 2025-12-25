#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

// из задачи 10
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
    cout << "\nРакета" << rocketIndex + 1 << ":" << endl;

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
             << max_height << " м" << endl;
        cout << "Момент времени: " << fixed << setprecision(2)
             << time_at_max << " с" << endl;
        if (max_index == heights.size() - 1 && max_height == 0) {
            cout << "Ракета не взлетела." << endl;
        }
    }
}

// задача 2
void countZeroAccelerationsDetailed() {
    vector<double> acceleration_data = {2.5, 0.0, 3.1, 2.8, 0.0, 0.0, 2.9, 3.0, 0.0, 2.7, 3.2, 0.0};

    cout << "Данные датчика ускорения (" << acceleration_data.size() << " измерений):" << endl;
    for (size_t i = 0; i < acceleration_data.size(); i++) {
        cout << fixed << setprecision(1) << acceleration_data[i];
        if (i < acceleration_data.size() - 1) cout << ", ";
    }
    cout << endl;

    int zero_count = count(acceleration_data.begin(), acceleration_data.end(), 0.0);
    cout << "Количество нулевых показаний: " << zero_count << endl;
    cout << "Процент нулевых показаний: " << fixed << setprecision(1)
         << (static_cast<double>(zero_count) / acceleration_data.size() * 100.0) << "%" << endl;

    vector<vector<double>> sensor_data = {
        {2.5, 3.1, 0.0, 2.8, 0.0, 2.9, 3.0, 0.0, 2.7, 3.2},
        {1.2, 0.0, 1.3, 1.4, 1.1, 0.0, 0.0, 1.5, 1.6, 1.2},
        {9.8, 9.7, 0.0, 9.6, 9.8, 9.7, 0.0, 0.0, 9.8, 9.6}
    };

    cout << "\nПодсчет нулевых показаний для каждого датчика:" << endl;
    for (size_t sensor_idx = 0; sensor_idx < sensor_data.size(); sensor_idx++) {
        const vector<double>& sensor_readings = sensor_data[sensor_idx];
        int zero_count_sensor = count(sensor_readings.begin(), sensor_readings.end(), 0.0);
        double percentage = (static_cast<double>(zero_count_sensor) / sensor_readings.size()) * 100;

        cout << "Датчик " << sensor_idx + 1 << ": " << zero_count_sensor
             << " нулевых показаний (" << fixed << setprecision(1) << percentage << "%)" << endl;
    }
}

// задача 3
void cleanTemperatureData() {
    vector<double> temperatures = {85.5, 92.3, 105.7, 150.2, -150.0, 180.5, 250.0, 95.8,
                                   98.2, -50.3, 110.5, 210.5, 88.9, -200.0, 195.8};

    cout << "Исходные данные температуры (" << temperatures.size() << " измерений):" << endl;
    for (size_t i = 0; i < temperatures.size(); i++) {
        cout << fixed << setprecision(1) << temperatures[i];
        if (i < temperatures.size() - 1) cout << ", ";
    }
    cout << endl;

    auto new_end = remove_if(temperatures.begin(), temperatures.end(),
        [](double temp) {
            return temp < -100.0 || temp > 200.0;
        });

    temperatures.erase(new_end, temperatures.end());

    cout << "Очищенные данные температуры (" << temperatures.size() << " измерений):" << endl;
    for (size_t i = 0; i < temperatures.size(); i++) {
        cout << fixed << setprecision(1) << temperatures[i];
        if (i < temperatures.size() - 1) cout << ", ";
    }
    cout << endl;

    if (!temperatures.empty()) {
        double avg_temp = accumulate(temperatures.begin(), temperatures.end(), 0.0) / temperatures.size();
        cout << "Средняя температура после очистки: " << fixed << setprecision(1) << avg_temp << " °C" << endl;
    }
}

//задача 4
void calculateTotalPathForRocket() {
    cout << "\n=== Задача 4: Вычисление суммарного пути ракеты ===" << endl;

    vector<double> y_coordinates = {0.0, 10.5, 25.3, 42.8, 60.1, 75.0, 85.2, 90.5, 88.3, 80.1};

    cout << "Координаты ракеты по оси Y:" << endl;
    for (size_t i = 0; i < y_coordinates.size(); i++) {
        cout << "t=" << i << ": " << fixed << setprecision(1) << y_coordinates[i] << " м" << endl;
    }

    if (y_coordinates.size() < 2) {
        cout << "Недостаточно данных для вычисления пути!" << endl;
        return;
    }

    vector<double> differences(y_coordinates.size());
    adjacent_difference(y_coordinates.begin(), y_coordinates.end(), differences.begin());

    cout << "\nРазности соседних координат:" << endl;
    for (size_t i = 0; i < differences.size(); i++) {
        cout << "Δt" << i << ": " << fixed << setprecision(1) << differences[i] << " м" << endl;
    }

    double total_path = accumulate(differences.begin() + 1, differences.end(), 0.0,
        [](double sum, double diff) {
            return sum + abs(diff);
        });

    cout << "\nСуммарный путь ракеты по оси Y: " << fixed << setprecision(1)
         << total_path << " м" << endl;

    double net_displacement = y_coordinates.back() - y_coordinates.front();
    cout << "Чистое смещение (начальная - конечная точка): " << fixed << setprecision(1)
         << net_displacement << " м" << endl;
    cout << "Разница между общим путем и чистым смещением: " << fixed << setprecision(1)
         << (total_path - abs(net_displacement)) << " м" << endl;
}

// задача 5
void sortAndRemoveDuplicates() {
    cout << "\n=== Задача 5: Сортировка и удаление дубликатов траекторий ===" << endl;

    vector<double> x_coordinates = {15.3, 8.7, 15.3, 42.1, 8.7, 25.6, 42.1, 15.3,
                                    30.8, 8.7, 10.5, 42.1, 25.6, 15.3, 5.2};

    cout << "Исходные координаты по X (" << x_coordinates.size() << " значений):" << endl;
    for (size_t i = 0; i < x_coordinates.size(); i++) {
        cout << fixed << setprecision(1) << x_coordinates[i];
        if (i < x_coordinates.size() - 1) cout << ", ";
    }
    cout << endl;

    // 1. Сортировка
    sort(x_coordinates.begin(), x_coordinates.end());

    cout << "\nПосле сортировки:" << endl;
    for (size_t i = 0; i < x_coordinates.size(); i++) {
        cout << fixed << setprecision(1) << x_coordinates[i];
        if (i < x_coordinates.size() - 1) cout << ", ";
    }
    cout << endl;

    auto new_end = unique(x_coordinates.begin(), x_coordinates.end());
    x_coordinates.erase(new_end, x_coordinates.end());

    cout << "\nПосле удаления дубликатов (" << x_coordinates.size() << " уникальных значений):" << endl;
    for (size_t i = 0; i < x_coordinates.size(); i++) {
        cout << fixed << setprecision(1) << x_coordinates[i];
        if (i < x_coordinates.size() - 1) cout << ", ";
    }
    cout << endl;

    if (!x_coordinates.empty()) {
        double min_val = *min_element(x_coordinates.begin(), x_coordinates.end());
        double max_val = *max_element(x_coordinates.begin(), x_coordinates.end());
        double avg_val = accumulate(x_coordinates.begin(), x_coordinates.end(), 0.0) / x_coordinates.size();

        cout << "\nСтатистика уникальных координат:" << endl;
        cout << "Минимальное значение: " << fixed << setprecision(1) << min_val << endl;
        cout << "Максимальное значение: " << fixed << setprecision(1) << max_val << endl;
        cout << "Среднее значение: " << fixed << setprecision(1) << avg_val << endl;
    }
}

int main() {
    srand(time(0));

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
                     << setprecision(2) << fallTime << " с" << endl;
            }
        }
    }
    cout << setw(10) << "Ракета" << setw(15) << "X" << setw(15) << "Y"
         << setw(15) << "Z" << setw(20) << "Состояние" << endl;
    cout << string(75, '-') << endl;

    for (size_t i = 0; i < squad.size(); i++) {
        cout << setw(10) << i+1 << setw(15) << fixed << setprecision(2) << squad[i].get_X()
             << setw(15) << squad[i].get_Y() << setw(15) << squad[i].get_Z();
        if (squad[i].getfall()) {
            cout << setw(20) << "Упала";
        } else {
            cout << setw(20) << "В полете";
        }
        cout << endl;
    }

    // задача 1
    for (size_t i = 0; i < squad.size(); i++) {
        findMaxHeightForRocket(i, all_times[i], all_heights[i]);
    }

    // Поиск глобального максимума среди всех ракет
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

    if (global_max_rocket != -1) {
        cout << "\nГлобальный максимум среди всех ракет:" << endl;
        cout << "Ракета " << global_max_rocket + 1 << " достигла максимальной высоты "
             << fixed << setprecision(2) << global_max_height << " м в момент времени "
             << global_max_time << " с" << endl;
    }

    countZeroAccelerationsDetailed();  // Задача 2
    cleanTemperatureData();            // Задача 3
    calculateTotalPathForRocket();     // Задача 4
    sortAndRemoveDuplicates();         // Задача 5


    return 0;
}
