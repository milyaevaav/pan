//============================================================================
// Name        : seminar3_8.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

int main() {
    vector<double> times1, heights1;
    vector<double> times2, heights2;

    double v01 = 50.0, v02 = 40.0, g = 9.8, dt = 0.5;
    for (double t = 0.0; t <= 15.0; t += dt) {
        double h1 = v01 * t - 0.5 * g * t * t;
        double h2 = v02 * t - 0.5 * g * t * t;

        times1.push_back(t);
        heights1.push_back(h1);

        times2.push_back(t);
        heights2.push_back(h2);
    }
    auto max_it1 = std::max_element(heights1.begin(), heights1.end());
    double max_height1 = *max_it1;
    double time_max1 = times1[distance(heights1.begin(), max_it1)];

    auto max_it2 = std::max_element(heights2.begin(), heights2.end());
    double max_height2 = *max_it2;
    double time_max2 = times2[distance(heights2.begin(), max_it2)];

    auto fall_it1 = std::find_if(heights1.begin() + 1, heights1.end(), [](double h) { return h < 0.0; });

    auto fall_it2 = std::find_if(heights2.begin() + 1, heights2.end(), [](double h) { return h < 0.0; });

    double time_fall1 = (fall_it1 != heights1.end()) ?
                       times1[distance(heights1.begin(), fall_it1)] : -1;

    double time_fall2 = (fall_it2 != heights2.end()) ?
                       times2[distance(heights2.begin(), fall_it2)] : -1;

    cout << "Параметр" << std::setw(29) << "Ракета1"
              << std::setw(19) << "Ракета2" << endl;
    cout << string(44, '-') << endl;
    cout << setw(20) << "Макс. высота (м)"
              << setw(15) << fixed << setprecision(2) << max_height1
			  << setw(13) << max_height2 << endl;
    cout << setw(15) << "Время макс. высоты (с)"
              << setw(9) << time_max1 << setw(13) << time_max2 << endl;
    cout << setw(20) << "Время падения (с)"
              << setw(14) << time_fall1 << setw(13) << time_fall2 << endl;

    return 0;
}
