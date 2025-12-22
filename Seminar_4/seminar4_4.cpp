#include <iostream>
#include <iomanip>
using namespace std;

class Gyroscope {
private:
    double angularVelocity;
    bool calibrationRequired;

public:

    Gyroscope(double velocity, bool needCalibration)
    : angularVelocity(velocity), calibrationRequired(needCalibration) {}

    void calibrate() {
        calibrationRequired = false;
    }

    void printStatus() const {
        if (calibrationRequired) {
        	cout << fixed << setprecision(1);
        	cout << "Скорость: " << angularVelocity << " °/с | " << "Калибровка требуется\n";
        } else {
        	cout << fixed << setprecision(1);
        	cout << "Скорость: " << angularVelocity << " °/с | " << "Калибровка не требуется\n";
        }
    }
};

int main() {
    Gyroscope g1(120.5, true);
    g1.printStatus();
    g1.calibrate();
    g1.printStatus();

    return 0;
}
