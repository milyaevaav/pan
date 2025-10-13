//============================================================================
// Name        : seminar2_8.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

struct plane{
	std::string name;
	double weight;
	double thrust;
	double Cd;
	double Cl;
	double ay;
	double time;
};

void inputPlanes(plane *p, int size){
	for (int i=0; i<size; i++){
		cout<<(i+1)<<"---------------------------------------"<<endl;
		cout<<"Введите название самолета:";
		cin>>(p+i)->name;
		cout<<"Введите вес самолета:";
		cin>>(p+i)->weight;
		cout<<"Введите значение тяги самолета:";
		cin>>(p+i)->thrust;
		cout<<"Введите коэффициент сопротивления:";
		cin>>(p+i)->Cd;
		cout<<"Введите коэффициентом подъемной силы:";
		cin>>(p+i)->Cl;
	}
}

void counts(plane *p, int size){
	double g=9.8, h;
	cout<<"---------------------------------------\nВведите высоту подъема:";
	cin>>h;
	for (int i=0; i<size; i++){
		(p+i)->ay = ((p+i)->thrust-(p+i)->weight*g)/(p+i)->weight;
		(p+i)->time = sqrt(2*h/(p+i)->ay);
	}
}

void sort(plane *p, int size){
	for (int i=0; i<size; i++){
		for (int j=0; j<size-i-1; j++){
			if ((p+j)->time > (p+j+1)->time){
				plane k;
				k = *(p+j);
				*(p+j)=*(p+j+1);
				*(p+j+1)=k;
			}
		}
	}
}

void printPlanes(plane *p, int size){
	cout<<"\nТаблица самолетов\n";
	cout<<"Номер   Название        Вес       Тяга    Коэф.Cd    Коэф.Cl   Вертикальное ускорение   Время подъема"<<endl;
	for (int i=0; i<size; i++){
		cout<<setw(2)<<(i+1)<<setw(14)<<(p+i)->name<<setw(11)<<(p+i)->weight<<setw(11)<<(p+i)->thrust<<setw(11)<<(p+i)->Cd<<setw(11)<<(p+i)->Cl<<setw(25)<<(p+i)->ay<<setw(16)<<(p+i)->time<<endl;
	}
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
	sort(Aircraft,n);
	printPlanes(Aircraft,n);

	delete[] Aircraft;
	return 0;
}
