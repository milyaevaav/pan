#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <memory>
#include <cstdio>

const double TU154_MASS = 98000.0;                      // Масса самолета, кг
const double TU154_WING_AREA = 201.0;                   // Площадь крыла, м²
const double b = 37.55;                                 // Размах крыла, м
const double TU154_NOMINAL_THRUST = 3 * 105000.0;       // Суммарная тяга, Н
const double Cy0 = 0.2;                                 // Коэффициент подъемной силы при нулевом угле атаки
const double MAX_THRUST_PERCENT = 0.9;                  // Реалистичный процент тяги
const double MAX_Theta = 9.0;                           // Максимальный угол набора высоты в градусах

const double INITIAL_ALTITUDE = 400.0;                  // Начальная высота, м
const double FINAL_ALTITUDE = 6500.0;                   // Конечная высота, м

const double INITIAL_VELOCITY = 320.0 / 3.6;            // Начальная скорость, м/с
const double FINAL_VELOCITY = 900.0 / 3.6;              // Конечная скорость, м/с
const double MAX_V_y = 12.0;                            // Максимальная скорость набора высоты, м/с
const double MIN_V_y = INITIAL_VELOCITY;                // Минимальная скорость для набора высоты, м/с

const double g = 9.81;                            // Ускорение свободного падения, м/с²

const double h = 100;                                   // Разбиение высоты на участки для расчета
const double INF = 1e9;
const double ZERO_COST = 0.0;

class CSVWriter {
private:
    std::ofstream file;

public:
    CSVWriter(const std::string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
    }

    ~CSVWriter() {
        if (file.is_open()) file.close();
    }

    void writeRow(const std::vector<double>& row) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << std::setprecision(6) << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    void writeHeader(const std::vector<std::string>& headers) {
        for (size_t i = 0; i < headers.size(); ++i) {
            file << headers[i];
            if (i < headers.size() - 1) file << ",";
        }
        file << "\n";
    }
};

class Interpolator {
private:
    std::vector<double> altitudes;
    std::vector<double> temperatures;
    std::vector<double> pressures;
    std::vector<double> densities;
    std::vector<double> sound_speeds;

    void initializeTableData() {
        altitudes = {0, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
        temperatures = {288.15, 284.9, 281.651, 275.154, 268.659, 262.166, 255.676,
                        249.187, 242.7, 236.215, 229.733, 223.252};
        pressures = {1.01325e5, 9.54613e4, 8.98763e4, 7.95014e4, 7.01212e4, 6.16604e4,
                     5.40483e4, 4.72176e4, 4.11051e4, 3.56516e4, 3.08007e4, 2.64999e4};
        densities = {1.22500, 1.16727, 1.11166, 1.00655, 0.909254, 0.819347, 0.736429,
                     0.660111, 0.590018, 0.526783, 0.467063, 0.413510};
        sound_speeds = {340.294, 338.37, 336.435, 332.532, 328.584, 324.589, 320.545,
                        316.452, 312.306, 308.105, 303.848, 299.532};
    }

public:
    Interpolator() {
        initializeTableData();
    }

    double get_linear_interpolation(double cur_alt, const std::vector<double>& alt, const std::vector<double>& parametrs) const {
        if (cur_alt <= alt[0]){
        	return parametrs[0];
        }
        if(cur_alt >= alt.back()){
        	return parametrs.back();
        }

        for (size_t i = 0; i < alt.size()-1; i++){
            if (cur_alt >= alt[i] && cur_alt <= alt[i+1]){
                double delta_value;
                delta_value = (cur_alt - alt[i]) / (alt[i+1]-alt[i]);
                return parametrs[i] + delta_value * (parametrs[i+1] - parametrs[i]);
            }
        }
        return 0.1;
    }

    double get_Temperature(double cur_alt) const { return get_linear_interpolation(cur_alt, altitudes, temperatures); }
    double get_Pressure(double cur_alt) const { return get_linear_interpolation(cur_alt, altitudes, pressures); }
    double get_Air_Density(double cur_alt) const { return get_linear_interpolation(cur_alt, altitudes, densities); }
    double get_Sound_Speed(double cur_alt) const { return get_linear_interpolation(cur_alt, altitudes, sound_speeds); }
    double get_Mach(double V, double cur_alt) const { return V / get_Sound_Speed(cur_alt); }
};

class TrajectoryPoint {
public:
    double time;            // Время, с
    double height;          // Высота, м
    double V_res;           // Скорость, м/с


    TrajectoryPoint(double t = 0, double hght = 0, double vel = 0)
        : time(t), height(hght), V_res(vel) {}

    // Вывод текуущей точки траектроии
    void print() const {
      std::cout << std::fixed << std::setprecision(1);
      std::cout << "t=" << time << "с, H=" << height << "м, V=" << V_res*3.6 << "км/ч, ";
    }
};

class Trajectory {
private:
    std::vector<TrajectoryPoint> points;

public:
    Trajectory() {}

    void addPoint(const TrajectoryPoint& point) {
    	points.push_back(point);
    }
    const std::vector<TrajectoryPoint>& getPoints() const { return points; }

    void saveToCSV(const std::string& filename) {
        CSVWriter csv(filename);
        csv.writeHeader({"time_s", "altitude_m", "velocity_ms"});

        for (const auto& point : points) {
            std::vector<double> row_data = {
                point.time, point.height, point.V_res,
            };
            csv.writeRow(row_data);
        }

        std::cout << "Траектория сохранена в файл: " << filename << std::endl;
    }
    // Вывод траектори по точкам
    void TrajectoryPrint()const {
    	        std::cout << "\n----- Траектория Полета ТУ-154 -----" << std::endl;
    	        std::cout << "Всего точек в траеткирии: " << points.size() << std::endl;
    	        std::cout << std::string(38, '-') << std::endl;

    	        int c = 0;
    	        for (size_t i = points.size(); i > 0; --i) {
    	            std::cout << "Точка " << c + 1 << ": ";
    	            points[i-1].print();
    	            std::cout<<std::endl;
    	            c++;
    	        }
    	    }

    void plotTrajectory() const {
        FILE* gp = _popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist", "w");
        if (!gp) {
            std::cerr << "Ошибка" << std::endl;
            return;
        }

        fprintf(gp, "set terminal wxt size 1200,800 font 'Arial,12'\n");
        fprintf(gp, "set multiplot layout 2,2 title 'Параметры полета ТУ-154'\n");

        // Высота от времени
        fprintf(gp, "set title 'Высота от времени'\n");
        fprintf(gp, "set xlabel 'Время, с'\n");
        fprintf(gp, "set ylabel 'Высота, м'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot '-' with lines lw 2 lc 'blue' title 'Высота'\n");
        for (const auto& point : points) {
            fprintf(gp, "%f %f\n", point.time, point.height);
        }
        fprintf(gp, "e\n");
        // Скорость от времени
        fprintf(gp, "set title 'Скорость от времени'\n");
        fprintf(gp, "set xlabel 'Время, с'\n");
        fprintf(gp, "set ylabel 'Скорость, км/ч'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot '-' with lines lw 2 lc 'red' title 'Скорость'\n");
        for (const auto& point : points) {
            fprintf(gp, "%f %f\n", point.time, point.V_res * 3.6);
        }
        fprintf(gp, "e\n");
        fprintf(gp, "unset multiplot\n");
        fflush(gp);
        std::cout << "Закройте окно Gnuplot чтобы продолжить" << std::endl;
        std::cin.get();
        _pclose(gp);
    }
};

class Aircraft {
private:
    double wing_area;
    double initial_mass;
    Interpolator env;

public:
    double mass;
    double thrust;      // Тяга, Н
    double Cx0;         // Коэффициент сопротивления при нулевой подъемной силе
    double K;           // Коэффициент индуктивного сопротивления
    double Cy_max;      // Максимальный коэффициент подъемной силы

    Aircraft(double m = TU154_MASS, double wing = TU154_WING_AREA)
        : wing_area(wing), initial_mass(m), env(), mass(m) {

        thrust = TU154_NOMINAL_THRUST * MAX_THRUST_PERCENT;
        Cx0 = 0.023;
        K = 0.012;
        Cy_max = 1.92;
    }

    double Attack_Angle(double cur_alt, double current_velocity) {
    	double current_Thrust = total_thrust(cur_alt);
        double ro = env.get_Air_Density(cur_alt);
        double grad_Cy = grad_Cy_a();
        double q = 0.5 * ro * current_velocity * current_velocity;

        double alpha = (TU154_MASS * g - q * TU154_WING_AREA * Cy0) / (q * TU154_WING_AREA * grad_Cy + current_Thrust);
        return alpha; // Угол атаки в градусах

    }

    // Расчет градиента коэффициента подъема
    double grad_Cy_a() const{
        double AR = b*b/TU154_WING_AREA;
        return ((2 * 3.14159 * AR) / (2 + pow(4+AR*AR, 0.5))) * 3.14159/180; // перевод в градусы
    }

    // Коэффициент подъемной силы в зависимости от угла атаки
    double get_LiftingForceCoefficient(double alpha) const{
        if (alpha < 0){
        	alpha = 0;
        }
        if (alpha > 12){
        	alpha = 12;
        }

    // Расчета через линейной зависимостью
        double grad_Cy = grad_Cy_a();
        double Cy = 0.25 + grad_Cy * alpha;
        if (Cy > 1.1)
            return 1.1;
        return Cy;

    }

    // Подъемная сила
    double computeLiftForce(double current_velocity, double cur_alt, double alpha) const {
        double ro = env.get_Air_Density(cur_alt);
        double Cy = get_LiftingForceCoefficient(alpha);
        double Y = 0.5 * ro * current_velocity * current_velocity * TU154_WING_AREA * Cy;
        return Y;
     }

    // Коэффициент сопротивления вычисляется через параболическую поляру
    double getDragCoefficient(double Cy) const {return Cx0 + K * Cy * Cy;}

    // Сила лобового сопротивления
    double Drag_Force(double current_velocity, double cur_alt, double alpha) const {
        double ro = env.get_Air_Density(cur_alt);
        double Cx = getDragCoefficient(get_LiftingForceCoefficient(alpha));
        double X = 0.5 * ro * current_velocity * current_velocity * TU154_WING_AREA * Cx;
        return X;
    }
    // Расчет тяги в зависимости от параметров среды полета
    double total_thrust(double cur_alt){
        double p_0 = env.get_Pressure(0);
        double current_Pressure = env.get_Pressure(cur_alt);
        return TU154_NOMINAL_THRUST*MAX_THRUST_PERCENT * pow(current_Pressure/p_0, 0.7); // коррекция тяги по плотности и давлению
    }
};


class DynamicProgrammingSolver {
    private:
        Interpolator env;
public:
    DynamicProgrammingSolver() {}
    // Расчет этапа подъема
    double calculate_podiem(double initial_alt, double final_alt, double velocity, Aircraft& Aircraft){
        double avg_alt = (final_alt - initial_alt) / 2;
        double current_Thrust = Aircraft.total_thrust(avg_alt);
        double alpha_degree = Aircraft.Attack_Angle(avg_alt, velocity);

        double Cx = Aircraft.getDragCoefficient(Aircraft.get_LiftingForceCoefficient(alpha_degree));
        double ro = env.get_Air_Density(avg_alt);
        double q = 0.5 * velocity*velocity*ro;
        double X = q * TU154_WING_AREA * Cx;

        double sin_tetha = std::min((current_Thrust-X) / (TU154_MASS* g), MAX_Theta/57.3);
        double velocity_y = velocity * sin_tetha;

        if (velocity_y > MAX_V_y) {
        	velocity_y = MAX_V_y;
        }

        double dt = (final_alt-initial_alt) / velocity_y;
        return dt;
     }

    // Расчет этапа разгона
    double calculate_razgon(double altitude, double initial_velocity, double final_velocity, Aircraft& Aircraft){
        double avg_Vel = (initial_velocity+final_velocity) / 2;
        if (avg_Vel < MIN_V_y) avg_Vel = MIN_V_y;
        double current_Thrust = Aircraft.total_thrust(altitude);
        double alpha_degree = Aircraft.Attack_Angle(altitude, avg_Vel);

        double Cx = Aircraft.getDragCoefficient(Aircraft.get_LiftingForceCoefficient(alpha_degree));
        double ro = env.get_Air_Density(altitude);
        double q = 0.5 * avg_Vel* avg_Vel * ro;
        double alpha_rad = alpha_degree/57.3;
        double a_x = ((current_Thrust * cos(alpha_rad)) - q * Cx * TU154_WING_AREA) / TU154_MASS;
        return (final_velocity - initial_velocity) / a_x;
    }

    // Расчет этапа подъем разгон
    double calculate_podiem_razgon(double initial_alt, double final_alt, double initial_vel, double final_vel, Aircraft& Air_craft){
        double avg_alt = 0.5 * (final_alt - initial_alt);
        double avg_vel = 0.5 * (final_vel - initial_vel);

        double dH = final_alt - initial_alt;
        double dV = final_vel - initial_vel;

        double time_for_climb = calculate_podiem(initial_alt, final_alt, avg_vel, Air_craft);
        double time_for_Air_craftc = calculate_razgon(avg_alt, initial_vel, final_vel, Air_craft);

        double dt = std::max(time_for_climb, time_for_Air_craftc);

        double Vy = dH/dt;
        if(Vy > MAX_V_y){
            double new_min_dt = dH / MAX_V_y;
            dt = new_min_dt;
        }
        return dt;
    }
    Trajectory computeOptimalTrajectory(Aircraft& Aircraft) {


        int N = (FINAL_ALTITUDE - INITIAL_ALTITUDE) / h;
        double DELTA_V = (FINAL_VELOCITY - INITIAL_VELOCITY) / N;
        std::vector <double> height_grid(N+1);
        std::vector <double> velocity_grid(N+1);

        for (size_t i = 0; i <= N; i++){
            height_grid[i] = INITIAL_ALTITUDE + i * h;
            velocity_grid[i] = INITIAL_VELOCITY + i * DELTA_V;
        }
        std::cout << "Критерий минимизации времени" << "\n";
        std::cout << "Текущее количество N: " << N << "\n";
        std::vector<std::vector<double>> cost_table(N + 1, std::vector<double>(N + 1, INF));
        std::vector<std::vector<double>> time_table(N + 1, std::vector<double>(N + 1, ZERO_COST));
        std::vector<std::vector<int>> previous_i(N + 1, std::vector<int>(N + 1, -1));
        std::vector<std::vector<int>> previous_j(N + 1, std::vector<int>(N + 1, -1));
        cost_table[0][0] = ZERO_COST;
        for (int i = 0; i <= N; i++){
            for (int j = 0; j <= N; j++){
                if (cost_table[i][j] >= INF) continue;
                double cur_alt = height_grid[i];
                double current_velocity = velocity_grid[j];
                if (j < N){
                    double final_velocity = velocity_grid[j+1];
                    double time_razgon = calculate_razgon(cur_alt, current_velocity, final_velocity, Aircraft);
                    double new_cost = cost_table[i][j]+time_razgon;

                    if(new_cost < cost_table[i][j+1]){
                        cost_table[i][j+1] = new_cost;
                        time_table[i][j+1] = time_table[i][j]+time_razgon;
                        previous_i[i][j+1] = i;
                        previous_j[i][j+1] = j;

                    }
                }

                if(i < N){
                    double final_altitude = height_grid[i+1];
                    double time_podiem = calculate_podiem(cur_alt, final_altitude, current_velocity, Aircraft);
                    double new_cost = cost_table[i][j]+time_podiem;

                    if (new_cost < cost_table[i+1][j]){
                        cost_table[i+1][j] = new_cost;
                        time_table[i+1][j] = time_table[i][j] + time_podiem;
                        previous_i[i+1][j] = i;
                        previous_j[i+1][j] = j;
                    }
                }

                if (i < N && j < N){
                    double final_altitude = height_grid[i+1];
                    double final_velocity = velocity_grid[j+1];
                    double time_podiem_razgon = calculate_podiem_razgon(cur_alt, final_altitude, current_velocity, final_velocity, Aircraft);
                    double new_cost = cost_table[i][j]+time_podiem_razgon;

                    if(new_cost < cost_table[i+1][j+1]){
                        cost_table[i+1][j+1] = new_cost;
                        time_table[i+1][j+1] = time_table[i][j] + time_podiem_razgon;
                        previous_i[i+1][j+1] = i;
                        previous_j[i+1][j+1] = j;
                    }
                }
            }
        }
        // Вывод матрицы времени
        std::cout << "Матрица времени (s):\n";
        std::cout << " V м/с |";
        for (int j = 0; j <= N; j++) {
            std::cout << std::setw(5) << (int)velocity_grid[j]<< " |";
        }
        std::cout << "\n  H м  |" << std::string(433, '-') << "|"<<std::endl;
        for (int i = 0; i <= N; i++) {
            std::cout << std::setw(6) << (int)height_grid[i]<<" |";
            for (int j = 0; j <= N; j++) {
                if (time_table[i][j] < 1e8) {
                    std::cout << std::setw(5) << (int)time_table[i][j]<<" |";
                }
                else {
                    std::cout << std::setw(7) << "---";
                }
            }
            std::cout << "\n";
        }

        Trajectory trajectory;
        TrajectoryPoint point;
        std::vector<std::pair<double, double> > path;
        int current_i = N, current_j = N;
        size_t i = 0;
        while (current_i >= 0 && current_j >= 0) {
            path.push_back(std::make_pair(height_grid[current_i], velocity_grid[current_j]));
            point.time = time_table[current_i][current_j];
            point.height = height_grid[current_i];
            point.V_res = velocity_grid[current_j];
            trajectory.addPoint(point);
            int prev_i = previous_i[current_i][current_j];
            int prev_j = previous_j[current_i][current_j];
            if (prev_i == -1) break;
            current_i = prev_i;
            current_j = prev_j;
            i++;
        }
    return trajectory;
}
};

int main() {

    try {
        Aircraft TU154;
        DynamicProgrammingSolver solver;
        Trajectory trajectory = solver.computeOptimalTrajectory(TU154);
        trajectory.TrajectoryPrint();
        trajectory.saveToCSV("tu-154_trajectory_realistic.csv");
        trajectory.plotTrajectory();
    } catch (const std::exception& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
