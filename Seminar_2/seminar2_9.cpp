//============================================================================
// Name        : seminar2_9.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

struct plane{
	std::string name;
	double weight;
	double s;
	double thrust;
	double Cd;
	double Cl;
	double l_fors;
	double res_c;
	double a;
	double ay;
	double skal_a;
};

void inputPlanes(plane *p, int size){
	for (int i=0; i<size; i++){
		cout<<(i+1)<<"---------------------------------------"<<endl;
		cout<<"Введите название самолета:";
		cin>>(p+i)->name;
		cout<<"Введите вес самолета:";
		cin>>(p+i)->weight;
		cout<<"Введите площадь крыла самолета самолета:";
		cin>>(p+i)->s;
		cout<<"Введите значение тяги самолета:";
		cin>>(p+i)->thrust;
		cout<<"Введите коэффициент сопротивления:";
		cin>>(p+i)->Cd;
		cout<<"Введите коэффициентом подъемной силы:";
		cin>>(p+i)->Cl;
	}
}

void counts(plane *p, int size){
	cout<<"\n";
	double g=9.8, pv,v;
	cout<<"Введите значение плотности воздуха:";
	cin>>pv;
	cout<<"\n";
	for (int i=0; i<size; i++){
		cout<<"Введите скорость самолета "<<i+1<<":";
		cin>>v;
		(p+i)->l_fors = 0.5*pv*pow(v,2)*(p+i)->s*(p+i)->Cl;
		(p+i)->res_c = 0.5*pv*pow(v,2)*(p+i)->s*(p+i)->Cd;
		(p+i)->ay = ((p+i)->l_fors-(p+i)->weight*g)/(p+i)->weight;
		(p+i)->a = ((p+i)->thrust-(p+i)->Cd)/(p+i)->weight;
		(p+i)->skal_a = abs(sqrt(pow((p+i)->ay,2)+pow((p+i)->a,2)));
	}
}

void printPlanes(plane *p, int size){
	cout<<"\nТаблица самолетов\n";
	cout<<"Номер   Название   Вес   Площадь   Тяга   Коэф.Cd   Коэф.Cl   Подъемная F   Сопротивление   Ускорение по Y   Ускорение по X   Модуль ускорения"<<endl;
	for (int i=0; i<size; i++){
		cout<<setw(2)<<(i+1)<<setw(14)<<(p+i)->name<<setw(6)<<(p+i)->weight<<setw(10)<<(p+i)->s<<setw(7)<<(p+i)->thrust<<setw(10)<<(p+i)->Cd<<setw(10)<<(p+i)->Cl<<setw(14)<<(p+i)->l_fors<<setw(16)<<(p+i)->res_c<<setw(17)<<(p+i)->ay<<setw(17)<<(p+i)->a<<setw(19)<<(p+i)->skal_a<<endl;
	}
}

void a_max(plane *p, int size){
	plane k = *p;
	for (int i=0; i<size; i++){
		if((p+i)->skal_a > k.skal_a){
			k = *(p+i);
		}
	}
	cout<<"\nСамолет "<<k.name<<" имеет наибольший модуль ускорения ускорения "<<k.skal_a<<endl;
}

int main() {
	int n;
	cout<<"Введите количество самолетов:";
	cin>>n;
	plane *Aircraft = new  plane[n];
	while (n<=0){
			cout<<"Введены некорректные данные. Укажите количество самолетов снова:";
			cin>>n;
	}
	inputPlanes(Aircraft,n);
	counts(Aircraft,n);
	printPlanes(Aircraft,n);
	a_max(Aircraft,n);

	delete[] Aircraft;
	return 0;
}
