//============================================================================
// Name        : seminar3_4.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
using namespace std;

int main() {
    const double v0=50.0, g=9.8, dt=1.0;
    vector<double> velocities;
    for (double t=0.0; t <= v0/g*2; t+=dt) {
        double v=v0-g*t;
        if (v < -v0) break;
        velocities.push_back(v);
    }
    if (velocities.empty()) {
    	cout << "Вектор пуст!" << endl;
        return 1;
    }
    double sum, avg;
    sum = accumulate(velocities.begin(), velocities.end(), 0.0);
    avg = sum/velocities.size();
    cout << "Интегральня средняя скорость: " << fixed << setprecision(2) << avg << " м/с" << endl;
    double time = velocities.size() * dt, integral=0.0, physical_avg;
    if (time > 0) {
    	for (size_t i=0; i < velocities.size()-1; i++) {
    		integral += (velocities[i] + velocities[i+1])*dt/2.0;
    	}
    	physical_avg=integral/time;
        cout << "Физическая средняя скорость: " << fixed << setprecision(2) << physical_avg << " м/с" << endl;
    }
    return 0;
}
