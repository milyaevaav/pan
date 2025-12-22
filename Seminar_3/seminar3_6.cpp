//============================================================================
// Name        : seminar3_6.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    std::vector<double> heights = {12.5, 8.2, 18.1, 5.7, 2.0};
    auto new_end = std::remove_if(heights.begin(), heights.end(), [](double h) {
        return h < 10.0;
    });
    heights.erase(new_end, heights.end());
    cout << "Обновлённые высоты: ";
    if (heights.empty()) {
        cout << "(вектор пуст)";
    } else {
        for (double h : heights) {
            cout << h << " ";
        }
    }
    cout << std::endl;
    cout << "Количество элементов в новом векторе: " << heights.size() << std::endl;

    return 0;
}
