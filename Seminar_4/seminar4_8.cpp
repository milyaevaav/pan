#include <iostream>
#include <iomanip>
using namespace std;

class NavigationSystem {
private:
    double x, y;
    double velocityX, velocityY;
    bool gpsAvailable;

public:
    NavigationSystem(double initX, double initY, double vX, double vY, bool gps)
            : x(initX), y(initY), velocityX(vX), velocityY(vY), gpsAvailable(gps) {}

    void integratePosition(double dt) {
        x += velocityX * dt;
        y += velocityY * dt;
    }

    void correctGPS(double realX, double realY) {
        cout << "Текущие координаты: (" << x << ", " << y << ")" << endl;

        if (gpsAvailable == true) {
            x = (x + realX) / 2;
            y = (y + realY) / 2;
            cout << "Реальные координаты: (" << realX << ", " << realY << ")" << endl;
        } else {
            cout << "Коррекция не выполнима." << endl;
        }
    }

    void printPosition() {
    	if (gpsAvailable == true) {
        cout << fixed << setprecision(1);
        cout << "\nСкорректированные координаты: (" << x << ", " << y << ")" << endl;
    	}
    }
};

int main() {
    NavigationSystem nav(0, 0, 100, 50, true);
    nav.integratePosition(1);
    nav.correctGPS(110, 60);
    nav.printPosition();

    return 0;
}
