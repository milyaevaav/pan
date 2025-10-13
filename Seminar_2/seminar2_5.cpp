//============================================================================
// Name        : seminar2_5.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

struct plane{
	std::string name;
	double weight;
	double s;
	double thrust;
	double Cd;
	double Cl;
	double v;
	double l_fors;
	double res_c;
	double a;
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
		cout<<"Введите площадь крыла самолета:";
		cin>>(p+i)->s;
		cout<<"Введите значение тяги самолета:";
		cin>>(p+i)->thrust;
		cout<<"Введите коэффициент сопротивления:";
		cin>>(p+i)->Cd;
		cout<<"Введите коэффициентом подъемной силы:";
		cin>>(p+i)->Cl;
	}
}

void printPlanes(plane *p, int size){
	cout<<"\nТаблица самолетов\n";
	cout<<"Номер   Название        Вес    S крыла       Тяга    Коэф.Cd    Коэф.Cl"<<endl;
	for (int i=0; i<size; i++){
		cout<<setw(2)<<(i+1)<<setw(14)<<(p+i)->name<<setw(11)<<(p+i)->weight<<setw(11)<<(p+i)->s<<setw(11)<<(p+i)->thrust<<setw(11)<<(p+i)->Cd<<setw(11)<<(p+i)->Cl<<endl;
	}
}

void count_printPlanes(plane *p, int size){
	cout<<"\nТаблица самолетов\n";
	cout<<"Номер   Название        Вес    S крыла       Тяга    Коэф.Cd    Коэф.Cl   Подъемная F   Сопротивление   Ускорение X   Ускорение Y   Время подъема"<<endl;
	for (int i=0; i<size; i++){
		cout<<setw(2)<<(i+1)<<setw(14)<<(p+i)->name<<setw(11)<<(p+i)->weight<<setw(11)<<(p+i)->s<<setw(11)<<(p+i)->thrust<<setw(11)<<(p+i)->Cd<<setw(11)<<(p+i)->Cl<<setw(13)<<(p+i)->l_fors<<setw(16)<<(p+i)->res_c<<setw(14)<<(p+i)->a<<setw(14)<<(p+i)->ay<<setw(16)<<(p+i)->time<<endl;
	}
}

void lifting_fors(plane *pl, int size, double v, double p){
	for (int i=0; i<size; i++){
		(pl+i)->l_fors = 0.5*p*pow(v,2)*((pl+i)->s)*((pl+i)->Cl);
	}
}

void air_res(plane *pl, int size, double v, double p){
	for (int i=0; i<size; i++){
		(pl+i)->res_c = 0.5*p*pow(v,2)*((pl+i)->s)*((pl+i)->Cd);
	}
}

void doost_X_Y(plane *pl, int size, double v, double p){
	double g = 9.8;
	for (int i=0; i<size; i++){
		(pl+i)->a = ((pl+i)->thrust)*((pl+i)->Cd)/((pl+i)->weight);
		(pl+i)->ay = ((pl+i)->l_fors)*((pl+i)->weight)*g/((pl+i)->weight);
	}
}

void time(plane *pl, int size, double h){
	for (int i=0; i<size; i++){
		(pl+i)->time=sqrt(2*h/((pl+i)->ay));
	}
}

void fast(plane *pl, int size,double h){
	double min = pl->time;
	int k=0;
	for(int i=0; i<size; i++){
		if ((pl+i)->time < min){
			k=i;
		}
	}
	cout<<"\nСамолет "<<(pl+k)->name<<"("<<k+1<<")"<<" наберет высоту "<<h<<" быстрее все остальных."<<endl;
}

int main() {
	int n;
	double v,p;
	cout<<"Введите количество самолетов:";
	cin>>n;
	while (n<=0){
		cout<<"Введены некорректные данные. Укажите количество самолетов снова:";
		cin>>n;
	}

	plane *planes = new plane[n];

	inputPlanes(planes,n);
	printPlanes(planes,n);

	cout<<"\nВведите скорость подъема:";
	cin>>v;
	cout<<"Введите значение плотности воздуха:";
	cin>>p;
	lifting_fors(planes,n,v,p);
	air_res(planes,n,v,p);
	doost_X_Y(planes,n,v,p);

	int h;
	cout<<"Введите высоту подъема:";
	cin>>h;

	count_printPlanes(planes,n);
	fast(planes,n,h);

	delete[] planes;
	return 0;
}
