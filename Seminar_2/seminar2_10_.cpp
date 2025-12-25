#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    double Tmin, Tmax, dT, h, m, g = 9.81;

    cout << "Введите минимальную тягу Tmin: ";
    cin >> Tmin;

    cout << "Введите максимальную тягу Tmax: ";
    cin >> Tmax;

    cout << "Введите шаг изменения тяги dT: ";
    cin >> dT;

    cout << "Введите высоту h: ";
    cin >> h;

    cout << "Введите массу самолета m: ";
    cin >> m;

    double min_time = 1e9;
    double optimal_T = Tmin;

    cout << fixed << setprecision(2);
    cout << "\n   Тяга  |Ускорение | Время" << endl;
    cout << "---------------------------------------" << endl;

    for (double T = Tmin; T <= Tmax; T += dT) {
        double D = 0;
        double ay = (T - D) / m;

        if (ay > 0) {
            double t = sqrt(2 * h / ay);

            cout << setw(8) << T << " | "
                 << setw(8) << ay << " | "
                 << setw(7) << t << endl;
            if (t < min_time) {
                min_time = t;
                optimal_T = T;
            }
        } else {
            cout << setw(8) << T << " | "
                 << "неположительное ускорение" << endl;
        }
    }
    cout << "\nОптимальная тяга: " << optimal_T << " Н" << endl;
    cout << "Минимальное время набора высоты " << h << " м: "
         << min_time << " с" << endl;

    return 0;
}
