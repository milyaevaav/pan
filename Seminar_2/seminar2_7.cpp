//============================================================================
// Name        : seminar2_7.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
using namespace std;

int main() {
	double m,L,D,T,g=9.8;
	cout<<"Введите массу самолета:";
	cin>>m;
	cout<<"Введите значение подъемной силы самолета:";
	cin>>L;
	cout<<"Введите сопротивление:";
	cin>>D;
	cout<<"Введите тягу двигателя самолета:";
	cin>>T;
	double ay=(L-m*g)/m;
	double ax=(T-D)/m;
	cout<<"ax = "<<ax<< endl;
	cout<<"ay = "<<ay<< endl;
	if (ay<0){
		cout<<"Режим 'снижение'";
	}else if(0<ay and ay<0.5){
		cout<<"Режим 'горизонтальный полет'";
	}else if(ay>0.5){
		cout<<"Режим 'набор высоты'";
	}

	return 0;
}
