//============================================================================
// Name        : seminar3_2.cpp
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
    const double v0=50.0, g=9.8, dt=1.0;
    vector<double> times;
    vector<double> heights;
    double h;
    if (v0 <= 0) {
    		cout << "Начальная скорость должна быть положительной!";
    }else{
    for (double t = 0.0; t <= 1000.0; t += dt) {
        h = v0*t-0.5*g*t*t;
        if (h < 0) break;
        times.push_back(t);
        heights.push_back(h);
    }
    if (heights.empty()) {
        cout << "Данных нет!" <<endl;
        return 1;
    }

    size_t idx_max=0;
    for (size_t i = 1; i < heights.size(); ++i) {
        if (heights[i] > heights[idx_max]) {
            idx_max=i;
        }
    }
    cout << "Максимальная высота: " << endl;
    cout << "Время: " << fixed << setprecision(2) << times[idx_max] << " с" << endl;
    cout << "Высота: " << heights[idx_max] << " м" << endl;
    }
    return 0;
}
