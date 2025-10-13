//============================================================================
// Name        : seminar2_1_2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
using namespace std;

void air_res (double s,double v,double p,double cd){
	double L2;
	L2=(0.5*p*pow(v,2)*s*cd);
	cout<<"Значение аэродинамического сопротивления: L2="<<L2<<endl;
}


int main() {
	double s,v,p,cl,cd,L1;
	cout<<"Введите площадь крыла самолета:";
	cin>>s;
	cout<<"Введите скорость полета:";
	cin>>v;
	cout<<"Введите значение плотности воздуха:";
	cin>>p;
	cout<<"Введите значение коэффициента подъемной силы:";
	cin>>cl;
	cout<<"Введите значение коэффициента сопротивления:";
	cin>>cd;
	if (s<=0 || v<=0 || p<=0 || cl<=0 || cd<=0){
		cout<<"Введеные данные некорректны!"<<endl;
	}else{
		L1=0.5*p*pow(v,2)*s*cl;
		cout<<"Значение подъемной силы: L1="<<L1<<endl;
		air_res(s,v,p,cd);
	}
	return 0;
}
