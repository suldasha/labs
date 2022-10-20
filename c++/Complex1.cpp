#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

class Complex         // класс "Комплексное число"
{
private:
    double re, im;
public:
    
  // конструктор 
    Complex (double r=0, double i = 0){
	    re=r;
		im=i;
	};//Complex a(2)
	
  // остальные функции
  // Ввод
    friend istream & operator >> (istream& in, Complex& x);
  // Модуль комплексного числа
    double Mod ();
  // Сумма комплексных чисел
    Complex operator + (const Complex& x);
  // Разность комплексных чисел
    Complex operator - (const Complex& x); 
  // Произведение комплексных чисел
    Complex operator * (const Complex& x);
  // Равенство
    bool operator == (const Complex& x); 
  //Неравенство
	bool operator != (const Complex& x);
  // Вывод
    friend ostream & operator << (ostream& out, const Complex& x);

};

// Ввод
    inline istream& operator >> (istream& in, Complex& x){
		cout << "input re, im:";
		return (in >> x.re >> x.im);
	}
	
// Вывод
    inline ostream& operator << (ostream& out, const Complex& x){
		if (x.re!=0 || x.im==0)
		    out << x.re ;
		if (x.im!=0){
		    if (x.im > 0 && x.re != 0)
			    out << "+";
		    out << x.im << 'i'; 
		};
		return out;
	}
	 
// Модуль
	inline double Complex:: Mod (){
		return sqrt(re*re + im*im);
	}
	
// Сложение 
    inline Complex Complex:: operator +	(const Complex& x){
		Complex res;
		res.re = re + x.re;
        res.im = im + x.im;
		return res;
	}
// Умножение 
    inline Complex Complex:: operator * (const Complex& x){
		Complex res;
		res.re = re*x.re - im*x.im;
        res.im = re*x.im + x.re*im;
		return res;
	}
	
// Сравнение
	inline bool Complex:: operator == (const Complex& x){
		return (re == x.re && im == x.im);
	}
	
// Неравенство
	inline bool Complex:: operator != (const Complex& x){
		return (re != x.re || im != x.im);
	}
	
// Вычитание
    inline Complex Complex:: operator - (const Complex& x){
		Complex res;
		res.re = re - x.re;
        res.im = im - x.im;
		return res;
	}//c=a-b
	
	
int main (){
    Complex a, b, sum, sub, mul;
	double mod;
    cin >> a;
	cout << a <<endl;
	cin >> b;
	cout << b << endl;
	mod = a.Mod();
	cout << "mod = " << mod << endl;
	sum = a+b;
	cout << "sum = " << sum << endl;
	mul = a*b;
	cout << "mul = " << mul << endl;
	sub = a-b;
	cout << "sub = " << sub << endl;
	if (a==b)
	    cout << "equal" << endl;
	if (a!=b)
		cout << "not equal" << endl;
    return 0;
}