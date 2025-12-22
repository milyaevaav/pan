#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class Engine {
private:
    double thrust;
    double fuelFlow;
    double fuel;

public:
    Engine(double thr, double fF, double f)
        : thrust(thr), fuelFlow(fF), fuel(f) {}

    double getThrust() {
        return thrust;
    }

    bool hasFuel(){
        return fuel > 0;
    }

    void burn(double dt) {
        if (hasFuel()) {
            double fuelm = fuelFlow * dt;
            if (fuelm > fuel) {
                fuelm = fuel;
            }
            fuel -= fuelm;
            if (fuel <= 0) {
                thrust = 0;
            }
        }
    }

    double getFuel(){
        return fuel;
    }
};

class Navigation {
private:
    double altitude;
    double velocity;
    double acceleration;
    double mass;

public:
    Navigation(double alt, double v, double m)
        : altitude(alt), velocity(v), mass(m) {
        acceleration = 0;
    }

    void update(double thrust, double dt){
        acceleration = thrust / mass - 9.81;
        velocity += acceleration * dt;
        altitude += velocity * dt;
        if (altitude < 0) {
            altitude = 0;
            velocity = 0;
        }
    }

    void printStatus(double time) {
       cout << fixed << setprecision(2);
       cout << "t=" << time << "с | h=" << altitude << "м | v=" << velocity << "м/с | a=" << acceleration << "м/с²" << endl;
    }

    double getAltitude(){
        return altitude;
    }

    double getVelocity(){
        return velocity;
    }
};

class AutonomousFlightSystem {
private:
    Engine engine;
    Navigation nav;
    double time;

public:
    AutonomousFlightSystem(const Engine& eng, const Navigation& navigation)
        : engine(eng), nav(navigation), time(0) {
        nav.update(engine.getThrust(), 0);
    }

    void simulate(double dt, double totalTime) {
        nav.printStatus(time);
        while (time < totalTime && engine.hasFuel()) {
            engine.burn(dt);
            nav.update(engine.getThrust(), dt);
            time += dt;
            nav.printStatus(time);
        }
    }

    void printSummary() {
        cout << "--- Полет завершен ---\n" << "Оставшееся топливо: " << engine.getFuel() << " кг" << endl;
    }
};

int main() {
    Engine e(40000, 3, 30);
    Navigation n(0, 10.19, 2000);
    AutonomousFlightSystem af(e, n);
    af.simulate(1.0, 10.0);
    af.printSummary();

    return 0;
}
