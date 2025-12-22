#include <iostream>
#include <iomanip>
using namespace std;

class RocketStage {
private:
    double thrust;
    double burnTime;
    double mass;

public:
    RocketStage(double T, double time, double m)
            : thrust(T), burnTime(time), mass(m) {}

    double getDeltaV() {
        if (mass <= 0){
        	return 0;
        }
        return (thrust * burnTime) / mass;
    }

    void printInfo() {
        cout << fixed << setprecision(2);
        cout << "Ступень: ΔV = " << getDeltaV() << " м/с; m = " << mass <<" кг; T = "
        		<< thrust << " H; t = " << burnTime << " c" << endl;
    }
};

int main() {
    RocketStage s1(5000, 10, 1000);
    RocketStage s2(7000, 8, 900);
    RocketStage s3(9000, 6, 800);

    cout << "Данные ступеней:" << endl;
    s1.printInfo();
    s2.printInfo();
    s3.printInfo();

    double totalV = s1.getDeltaV() + s2.getDeltaV() + s3.getDeltaV();

    cout << fixed << setprecision(2);
    cout << "\nИтоговая скорость ракеты: " << totalV << " м/с" << endl;

    return 0;
}
