//============================================================================
// Name        : seminar3_1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <numeric>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
	const double v0=50.0, g=9.8, dt=1.0;
	vector<double> times;
	vector<double> heights;
	double h;
	if (v0 <= 0) {
		cout << "Начальная скорость должна быть положительной!" << endl;
	}else{
		for (double t=0.0;; t+=dt){
			h = v0*t-0.5*g*t*t;
			if (h<0){
				break;
			} else{
				times.push_back(t);
				heights.push_back(h);
			}
		}
		double t = 0.5*9.8/v0;
		times.push_back(t+times.size());
	    heights.push_back(0);
		cout << setw(10) << "t" << setw(15) << "h" << endl;
		cout << string(25, '-')<< endl;
		for (size_t i = 0; i < times.size(); i++){
			cout << setw(10) << fixed << setprecision(2) << times[i]
			<< setw(15) << fixed << setprecision(2) << heights[i]
			<< endl;
			}

	}

	return 0;
}
