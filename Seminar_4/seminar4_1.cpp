#include <iostream>
using namespace std;

class Sensor {
private:
    double signalStrength;
    bool isActive;

public:
    Sensor(double strength)
    : signalStrength(strength), isActive(true) {}

    void activate() {
        isActive = true;
    }

    void deactivate() {
        isActive = false;
    }

    void setSignalStrength(double s) {
        if (s < 0) {
            signalStrength = 0;
        } else {
            signalStrength = s;
        }
    }

    double getSignalStrength() const {
        return signalStrength;
    }

    void printStatus() const {
        if (isActive) {
            cout << "Датчик активен | Сила сигнала: " << signalStrength << " дБ\n";
        } else {
            cout << "Датчик выключен | Сила сигнала: " << signalStrength << " дБ\n";
        }
    }
};

int main() {
    Sensor sensor(45.5);
    sensor.printStatus();

    sensor.setSignalStrength(-10);
    sensor.printStatus();

    sensor.deactivate();
    sensor.printStatus();

    return 0;
}
