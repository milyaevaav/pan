//============================================================================
// Name        : seminar3_3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
    const double v0=50.0, g=9.8, dt=1.0, t_max=v0/g*2;
    vector<double> times;
    vector<double> velocities;
    double v;
    for (double t=0.0; t<=t_max; t+=dt) {
        v=v0-g*t;
        if (v < -v0) break;
        times.push_back(t);
        velocities.push_back(v);
    }
    cout << setw(10) << "t, c" << setw(17) << "v, м/с" << endl;
    cout << string(25, '-') << endl;
    for (size_t i = 0; i < times.size(); i++) {
    	cout << setw(10) << fixed << setprecision(2) << times[i] << setw(15) << fixed << setprecision(2) << velocities[i] << endl;
    }
    double t0=v0/g;
    cout << "\nМомент смены знака v: t = " << fixed << setprecision(2) << t0 << " с" << endl;
    return 0;
}
