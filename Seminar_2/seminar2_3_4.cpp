//============================================================================
// Name        : seminar2_3_4.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
using namespace std;

int main() {
	double m,L,D,T,g=9.8,h;
	cout<<"Введите массу самолета:";
	cin>>m;
	cout<<"Введите подъемной силы самолета:";
	cin>>L;
	cout<<"Введите сопротивление:";
	cin>>D;
	cout<<"Введите тягу двигателя самолета:";
	cin>>T;
	double ay=(L-m*g)/m;
	while (ay<0){
		int x;
		cout<<"Необходимо заменить параметры m(1) или L(2), или оба(3):";
		cin>>x;
		switch(x){
		case 1:
			cout<<"Введите массу самолета:";
			cin>>m;
			ay=(L-m*g)/m;
			break;
		case 2:
			cout<<"Введите подъемной силы самолета:";
			cin>>L;
			ay=(L-m*g)/m;
			break;
		case 3:
			cout<<"Введите массу самолета:";
			cin>>m;
			cout<<"Введите подъемной силы самолета:";
			cin>>L;
			ay=(L-m*g)/m;
			break;
		}
	}
	double a=(T-D)/m;
	cout<<"Ускорение по направлеию движения:"<<a<<endl;
	cout<<"Вертикальное ускорение:"<<ay<<endl;
	cout<<"Введите высоту, которую должен набрать самолет:";
	cin>>h;
	while (h < 0){
		cout<<"Введены некоректные данные. Введите высоту, которую должен набрать самолет повторно:";
	}
	double t = sqrt(2*h/ay);
	cout<<"Время необходимое самолету для набора высоты "<<h<<" равно "<<t<< endl;
	return 0;
}
