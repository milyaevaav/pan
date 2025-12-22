#include <iostream>
#include <iomanip>
using namespace std;

class AutonomousControl {
private:
    double altitude;
    double thrust;
    double targetAltitude;

public:
    AutonomousControl(double alt, double thr, double target)
            : altitude(alt), thrust(thr), targetAltitude(target) {}

    void updateControl() {
        if (altitude < targetAltitude) {
            thrust += 100;
        } else {
            thrust -= 100;
            if (thrust < 0) {
                thrust = 0;
            }
        }
    }

    void simulateStep(double dt) {
        altitude += (thrust / 500) * dt;
        if (altitude < 0) {
            altitude = 0;
        }
    }

    void printStatus() const {
        cout << fixed << setprecision(0);
        cout << "Высота: " << altitude << "м, тяга: " << thrust << endl;
    }
};

int main() {
    AutonomousControl ac(0, 2000, 5000);

    for (int i = 0; i < 5; ++i) {
        ac.updateControl();
        ac.simulateStep(1.0);
        ac.printStatus();
    }

    return 0;
}
