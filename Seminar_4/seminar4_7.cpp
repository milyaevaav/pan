#include <iostream>
#include <iomanip>
using namespace std;

class FlightComputer {
private:
    double altitude;
    double velocity;
    double fuel;
    double thrust;

public:
    FlightComputer(double alt, double vel, double f, double thr)
            : altitude(alt), velocity(vel), fuel(f), thrust(thr) {}

    void simulateStep(double dt) {
        fuel -= dt * 2;
        if (fuel < 0) {
            fuel = 0;
        }

        velocity += (thrust / 1000 - 9.8) * dt;
        altitude += velocity * dt;

        if (altitude < 0) {
            altitude = 0;
        }
    }

    double t = 0.5;
    void printStatus() {
        cout << std::fixed << std::setprecision(1);
        cout << "t = " << t << "с: " << "высота = " << altitude << "м, " << "скорость = " << velocity << "м/с, "
             << "топливо = " << fuel << endl;

        t += 0.5;
    }
};

int main() {
    FlightComputer fc(0, 0, 100, 12000);

    for (int i = 0; i < 5; i++) {
        fc.simulateStep(0.5);
        fc.printStatus();
    }

    return 0;
}
