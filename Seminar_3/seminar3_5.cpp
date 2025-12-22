//============================================================================
// Name        : seminare3_5.cpp
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

struct Point3D {
    double x, y, z;
    Point3D(double x_val=0, double y_val=0, double z_val=0)
        : x(x_val), y(y_val), z(z_val) {}
};

int main() {
    const double vx=30.0, vy=20.0, vz=40.0;
    const double dt=0.1;
    const double t_max=10.0;
    vector<Point3D> trajectory;
    trajectory.reserve(static_cast<int>(t_max / dt)+1);
    double x=0.0, y=0.0, z=0.0;
    for (double t = 0.0; t <= t_max; t += dt) {
        x=vx*t;
        y=vy*t;
        z=vz*t;
        trajectory.push_back(Point3D(x, y, z));
    }
    cout << setw(8) << "t, c" << setw(13) << "x, м" << setw(13) << "y, м" << setw(13) << "z, м" << std::endl;
    cout << string(44, '-') << endl;

    for (size_t i=0; i < trajectory.size(); i++) {
        double t=i*dt;
        cout << setw(8) << fixed << setprecision(2) << t << setw(12) << fixed << setprecision(2) << trajectory[i].x
                  << setw(12) << fixed << setprecision(2) << trajectory[i].y << setw(12) << fixed << setprecision(2) << trajectory[i].z << endl;
    }
    cout << "\nПроверка:" << endl;
    cout << "x = " << vx * t_max << endl;
    cout << "y = " << vy * t_max << endl;
    cout << "z = " << vz * t_max << endl;

    return 0;
}
