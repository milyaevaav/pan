#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

class DroneFlight {
private:
    vector<double> x;
    vector<double> y;
    double totalDistance;

public:
    DroneFlight() : totalDistance(0) {
           x.push_back(0);
           y.push_back(0);
     }

    void addPoint(double newX, double newY) {
        double X, Y, dx, dy, d;
        X = x.back();
        Y = y.back();
        dx = newX - X;
        dy = newY - Y;
        d = sqrt(dx*dx + dy*dy);

        totalDistance += d;
        x.push_back(newX);
        y.push_back(newY);
    }

    double getTotalDistance() const {
        return totalDistance;
    }

    void printPath() const {
        cout << "Точки маршрута:\n";
        for (size_t i = 0; i < x.size(); i++) {
            cout << "(" << x[i] << ", " << y[i] << ")" << endl;
        }
    }
};

int main() {
    DroneFlight d;
    d.addPoint(3, 4);
    d.addPoint(6, 8);

    d.printPath();
    cout << "Пройдено: " << d.getTotalDistance() << " м" << endl;

    return 0;
}
