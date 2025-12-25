#include <iostream>
#include <cmath>

using namespace std;

class Aircraft {
protected:
    double mass;
    double x, y, z;
    double vx, vy, vz;

public:
    Aircraft(double m, double px, double py, double pz,
             double sx, double sy, double sz)
        : mass(m), x(px), y(py), z(pz), vx(sx), vy(sy), vz(sz) {}

    virtual void updatePosition(double dt) {
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
    }

    virtual void printStatus() const {
        cout << "Масса: " << mass << " кг, ";
        cout << "Позиция: (" << x << ", " << y << ", " << z << ") м, ";
        cout << "Скорость: (" << vx << ", " << vy << ", " << vz << ") м/с\n";
    }

    virtual ~Aircraft() {}
};

class JetAircraft : public Aircraft {
private:
    double thrust;
    double Cd;
    double S;
    double rho;
    double fuel;
    const double g = 9.81;

public:
    JetAircraft(double m, double px, double py, double pz,
                double sx, double sy, double sz,
                double T, double cd, double sArea, double density, double f)
        : Aircraft(m, px, py, pz, sx, sy, sz),
          thrust(T), Cd(cd), S(sArea), rho(density), fuel(f) {}

    double computeDrag(double velocity) const {
        return 0.5 * Cd * rho * S * velocity * velocity;
    }

    void simulateStep(double dt) {
        if (fuel <= 0) {
            cout << "Топливо закончилось.\n";
            return;
        }

        double velocity = sqrt(vx*vx + vy*vy + vz*vz);
        double drag = computeDrag(velocity);

        double a_thrust = thrust / mass;
        double a_drag = drag / mass;
        double a_net = a_thrust - a_drag - g;
        vz += a_net * dt;
        double fuel_burned = 0.1 * thrust * dt;
        if (fuel_burned > fuel) fuel_burned = fuel;
        fuel -= fuel_burned;

        updatePosition(dt);
    }

    void printStatus() const override {
        Aircraft::printStatus();
        cout << "Топливо: " << fuel << " кг, ";
        cout << "Тяга: " << thrust << " Н, ";
        cout << "Высота: " << z << " м\n";
    }

    double getFuel() const { return fuel; }
    double getVelocityZ() const { return vz; }
};

int main() {
    JetAircraft jet(20000, 0, 0, 0, 0, 0, 0,
                    50000, 0.02, 50, 1.225, 1000);

    double dt = 0.5;
    double time = 0;

    cout << "Начало моделирования полёта реактивного самолёта:\n";
    jet.printStatus();

    while (jet.getFuel() > 0 && jet.getVelocityZ() > 0) {
        jet.simulateStep(dt);
        time += dt;
        cout << "\nВремя: " << time << " с\n";
        jet.printStatus();
    }
    return 0;
}
