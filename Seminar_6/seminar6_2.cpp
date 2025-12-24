#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;

struct Target {
    int id;
    string name;
    double x, y, z;
    double priority;
    double distance;

    Target(int id = 0, const string& name = "",
           double x = 0.0, double y = 0.0, double z = 0.0,
           double priority = 0.0, double distance = 0.0)
        : id(id), name(name), x(x), y(y), z(z),
          priority(priority), distance(distance) {}
};

class TargetManager {
private:
    vector<Target> targets;
    string filename;

public:
    TargetManager(const string& filename = "targets.txt") : filename(filename) {}

    void addTarget(int id, const string& name, double x, double y, double z,
                  double priority, double distance) {
        for (const auto& target : targets) {
            if (target.id == id) {
                cout << "Цель с ID " << id << " уже существует!" << endl;
                return;
            }
        }

        targets.emplace_back(id, name, x, y, z, priority, distance);
        cout << "Цель '" << name << "' (ID: " << id << ") добавлена." << endl;
    }

    bool removeTarget(int target_id) {
        auto it = find_if(targets.begin(), targets.end(),
                         [target_id](const Target& t) { return t.id == target_id; });

        if (it != targets.end()) {
            cout << "Цель с ID " << target_id << " удалена." << endl;
            targets.erase(it);
            return true;
        }

        cout << "Цель с ID " << target_id << " не найдена." << endl;
        return false;
    }

    void saveTargetsToFile() {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << filename << endl;
            return;
        }
        for (const auto& target : targets) {
            fout << target.id << ","
                 << target.name << ","
                 << fixed << setprecision(2)
                 << target.x << ","
                 << target.y << ","
                 << target.z << ","
                 << target.priority << ","
                 << target.distance << "\n";
        }

        fout.close();
        cout << "Цели сохранены в файл: " << filename << endl;
    }

    void loadTargetsFromFile() {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Ошибка открытия файла для чтения: " << filename << endl;
            return;
        }

        targets.clear();
        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() == 7) {
                try {
                    Target target;
                    target.id = stoi(tokens[0]);
                    target.name = tokens[1];
                    target.x = stod(tokens[2]);
                    target.y = stod(tokens[3]);
                    target.z = stod(tokens[4]);
                    target.priority = stod(tokens[5]);
                    target.distance = stod(tokens[6]);

                    targets.push_back(target);
                } catch (const exception& e) {
                    cerr << "Ошибка преобразования данных: " << line << endl;
                }
            } else {
                cerr << "Некорректная строка: " << line << endl;
            }
        }

        fin.close();
        cout << "Загружено " << targets.size() << " целей из файла: " << filename << endl;
    }

    vector<Target> getHighPriorityTargets(double min_priority) {
        vector<Target> highPriorityTargets;

        copy_if(targets.begin(), targets.end(),
                back_inserter(highPriorityTargets),
                [min_priority](const Target& t) {
                    return t.priority >= min_priority;
                });

        return highPriorityTargets;
    }
    void sortByDistance() {
        sort(targets.begin(), targets.end(),
             [](const Target& a, const Target& b) {
                 return a.distance < b.distance;
             });
        cout << "Цели отсортированы по расстоянию." << endl;
    }

    void printAllTargets() {
        if (targets.empty()) {
            cout << "Нет целей в системе." << endl;
            return;
        }

        cout << "\nЦели в системе:" << endl;
        for (const auto& target : targets) {
            cout << "ID: " << target.id
                 << ", " << target.name
                 << ", Position: (" << fixed << setprecision(2)
                 << target.x << ", " << target.y << ", " << target.z << ")"
                 << ", Priority: " << target.priority
                 << ", Distance: " << target.distance << endl;
        }
        cout << "Всего целей: " << targets.size() << endl;
    }

    Target* findTargetById(int id) {
        for (auto& target : targets) {
            if (target.id == id) {
                return &target;
            }
        }
        return nullptr;
    }

    const vector<Target>& getAllTargets() const {
        return targets;
    }

    void clearAllTargets() {
        targets.clear();
        cout << "Все цели удалены." << endl;
    }

    int countTargets() const {
        return targets.size();
    }
};

void createInitialTargetsFile() {
    ofstream fout("targets.txt");
    if (fout.is_open()) {
        fout << "1,Name1,100.5,200.3,50.0,0.8,1500.0\n";
        fout << "2,Name2,150.2,180.7,300.0,0.9,2000.0\n";
        fout << "3,Name3,50.0,50.0,10.0,0.1,500.0\n";
        fout.close();
        cout << "Файл targets.txt создан с начальными данными." << endl;
    }
}

int main() {
    createInitialTargetsFile();
    TargetManager manager("targets.txt");
    manager.loadTargetsFromFile();
    manager.printAllTargets();

    cout << "\nДобавление цели" << endl;
    manager.addTarget(4, "Name4", 200.0, 100.0, 20.0, 0.7, 1800.0);
    cout << "\nСортировка целей по расстоянию..." << endl;
    manager.sortByDistance();

    cout << "\nНовый список целей (отсортированный по расстоянию):" << endl;
    manager.printAllTargets();
    cout << "\nПоиск целей с высоким приоритетом (>= 0.7):" << endl;
    auto highPriority = manager.getHighPriorityTargets(0.7);
    for (const auto& target : highPriority) {
        cout << "ID: " << target.id << ", " << target.name
             << ", Priority: " << target.priority
             << ", Distance: " << target.distance << endl;
    }

    manager.saveTargetsToFile();

    return 0;
}
