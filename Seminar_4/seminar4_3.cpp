#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class Engine {
private:
    double thrust;
    double fuelFlow;

public:
    Engine(double T, double F) : thrust(T), fuelFlow(F) {}

    double getSpecificImpulse() const {
        if (fuelFlow == 0) {
            return 0;
        }
        return thrust / (fuelFlow * 9.81);
    }

    void printInfo() const {
    	cout << std::fixed << std::setprecision(2);
    	cout << "Тяга: " << thrust << " H | " << "Расход: " << fuelFlow << " кг/с | " << "Удельный импульс: " << getSpecificImpulse() << " с" << endl;
    }
};

int main() {
    Engine e1(5000, 2.5);
    e1.printInfo();

    return 0;
}
