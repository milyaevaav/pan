//============================================================================
// Name        : seminar3_7.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

int main() {
    const double v0 = 50.0, g = 9.8, dt = 1.0;
    vector<double> times;
    vector<double> heights;
    double h;
    if (v0 <= 0) {
        cout << "Начальная скорость должна быть положительной" << endl;
    } else {
        double t_total = 2.0 * v0 / g;
        for (double t = 0.0; t <= 1000.0; t += dt) {
            h = v0 * t - 0.5 * g * t * t;
            if (h < 0) {
                times.push_back(t);
                heights.push_back(h);
                break;
            }
            times.push_back(t);
            heights.push_back(h);
        }
        auto it = find_if(heights.begin() + 1, heights.end(),
                         [](double h) { return h <= 0.0; });

        if (it == heights.end()) {
            cout << "Падения не было." << endl;
        } else {
            size_t idx = distance(heights.begin(), it);
            double t_fall = times[idx];
            double h_fall = heights[idx];
            cout << "Первый момент времени когда высота стала <= 0: t = " << fixed << setprecision(2)
                 << t_fall << " с" << endl;
        }
        cout << "\nТраектория:" << endl;
        for (size_t i = 0; i < times.size(); i++) {
            cout << "t = " << setw(5) << times[i]
                 << " с, h = " << heights[i] << " м" << endl;
        }
    }

    return 0;
}
