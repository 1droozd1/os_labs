#include <stdio.h>
#include <iostream>
#include <dlfcn.h>
#include "../src/types.h"

using namespace std; 

const char* lib1 = "./liblib1.so";
const char* lib2 = "./liblib2.so";

int main(int argc, char const *argv[])
{
	cout << "Введите: [key] [arg1] ... [argN]\n";
	cout << "Если вы хотите поменять метод вычислений, введите 0\n";
	cout << "Если вы хотите найти наибольший общий делитель для двух натуральных чисел: 1 [first number] [second number]\n";
	cout << "Если вы хотите отсортировать массив: 2 [size] [array[0]] [array[1]] ... [array[size-1]]\n";
	int key;
	int checker = 0;
	void* CurrentLib = dlopen(lib1, RTLD_LAZY);
	cout << "Библиотека: " << checker << "\n";
	os_int (*GCD)(os_int A, os_int B);
	os_int* (*Sort)(os_int *array, os_int size);
	GCD =(os_int (*)(os_int,os_int)) dlsym(CurrentLib, "GCD");
	Sort =(os_int* (*)(os_int*, os_int)) dlsym(CurrentLib, "Sort");
	while(cin >> key){
		if (key == 0){
			dlclose(CurrentLib);
			if (checker == 0){
				CurrentLib = dlopen(lib2, RTLD_LAZY);
			} else {
				CurrentLib = dlopen(lib1, RTLD_LAZY);
			}
			checker = !checker;
       			GCD = (os_int (*)(os_int, os_int)) dlsym(CurrentLib, "GCD");
        		Sort =(os_int* (*)(os_int*, os_int)) dlsym(CurrentLib, "Sort");
		}
		if (key == 1){
			os_int A,B;
			cin >> A >> B;
			cout << GCD(A,B) << "\n"; 
		} 
		if (key == 2){
			os_int size;
			cin >> size;
			os_int a[size], *b;
			for (int i=0; i<size; i++)
				cin >> a[i];
			b = Sort(a, size);
			for (int i=0; i<size; i++)
				cout << b[i] << " ";
			cout << "\n";
		}
		if (key != 0 and key != 1 and key != 2){
			cout << "\nНеправильный ключ\n";
		}
		cout << "\nВведите: [key] [arg1] ... [argN]\n";
        	cout << "Если вы хотите поменять метод выислений, введите 0\n";
        	cout << "Если вы хотите найти наибольший общий делитель для двух натуральных чисел: 1 [first number] [second number]\n";
        	cout << "Если вы хотите отсортировать список: 2 [size] [array[0]] [array[1]] ... [array[size-1]]\n";
		cout << "Библиотека: " << checker << "\n";
	}
}
