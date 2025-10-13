//============================================================================
// Name        : seminar2_6.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
	int n,s,cl;
	cout<<"Введите площадь крыла самолета:";
	cin>>s;
	cout<<"Введите значение коэффициента подъемной силы:";
	cin>>cl;
	cout<<"Введите кол-во эллементов в массиве скоростей: ";
	cin>>n;
	while (s<=0 || n<=0 || cl<=0){
		cout<<"Введеные данные некорректны! Введите параметры повторно."<<endl;
		if (s<=0){
			cout<<"Площадь крыла самолета:";
			cin>>s;
		}
		if(cl<=0){
			cout<<"Введите значение коэффициента подъемной силы:";
			cin>>cl;
		}
		if(n<=0){
			cout<<"Введите кол-во эллементов в массиве скоростей: ";
			cin>>n;
		}
	}
	double mass[n][2];
	for (int i=0; i<n; i++){
		cout<<"Шаг "<<i+1<<" :";
		for (int j=0; j<2; j++){
					cin>>mass[i][j];
				}
		mass[i][2]=0.5*mass[i][1]*pow(mass[i][0],2)*s*cl;
	}
	cout<<"| Шаг | Скорость | Плотность | Поъемная сила |"<<endl;
	for (int i=0; i<n; i++){
		cout<<"|"<<setw(4)<<i+1<<" |";
		cout<<setw(9)<<mass[i][0]<<" |"<<setw(10)<<mass[i][1]<<" |"<<setw(14)<<mass[i][2]<<" |"<<endl;
	}
	return 0;
}
