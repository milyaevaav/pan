#include <iostream>
using namespace std;

class Trajectory {
private:
    double startX, startY;
    double angle;

public:
    Trajectory() : startX(0), startY(0), angle(0) {}

    Trajectory(double x, double y, double ang)
    : startX(x), startY(y), angle(ang) {}

    void printTrajectory() {
        cout << "Старт: (" << startX << ", " << startY << "), угол: " << angle << "°"<<endl;
    }
};

int main() {
    Trajectory t1;

    Trajectory t2(100, 200, 45);

    t1.printTrajectory();
    t2.printTrajectory();

    return 0;
}

