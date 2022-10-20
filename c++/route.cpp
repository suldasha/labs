#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Класс маршрут
class route {
private:
    int *r, n; // r – массив, представляющий маршрут; n – количество городов
public:
    
    // создается первый маршрут
    route(int x){
		n = x;
		int f = 0;
	    cout << "Enter first town ";
	    cin >> f;
		cout << endl;
		f--;
		r = new int [x]; 
		r[0] = f;
		for (int i = 1; i<=f; i++)
			r[i] = i-1;
		for (int i = f+1; i<n; i++)
			r[i] = i;
	}; 
	// конструктор копирования
    route(const route& x){
		n = x.n;
		r = new int [x.n];
		for (int i = 0; i < x.n; i++)
			r[i] = x.r[i];
	}; 
	//деструктор
    ~route() {
	    if(r != NULL) 
		    delete [] r; 
	    r = NULL; 
    }
	// операция присваивания
    route & operator = (const route&); 
	int & operator [] (int);
	// вычисляется стоимость маршрута по матрице стоимости
    int route_price(int **); 
	// вычисляется следующий маршрут, используя алгоритм Дейкстры
    bool next_route(); 
	// вывод маршрута
    friend ostream & operator << (ostream&, const route&); 
};
// операция присваивания
inline route& route:: operator = (const route& x){
	if (this != &x){
		delete [] r;
		r = NULL;
		n = x.n;
		r = new int [n];
		for (int i = 0; i < n; i++)
			r[i] = x.r[i];
	};
	return *this;
}
inline int& route:: operator [] (int i){
	if (i<0 || i>=n)
		exit (1);
	return r[i];
}
// вычисляется стоимость маршрута по матрице стоимости
inline int route:: route_price(int **Matrix){
	int price = 0;
	for (int i = 0; i<n-1; i++)
		price = price + Matrix[r[i]][r[i+1]];
	price = price + Matrix[r[n-1]][r[0]];
	return price;
}
// вычисляется следующий маршрут, используя алгоритм Дейкстры
inline bool route:: next_route(){
	int i = n-2;
	while (r[i]>r[i+1]){
		if (i==1)
			return false;
		i--;
	};
	int j = n-1;
	while (r[i]>r[j])
		j--;
	int l = r[i];
	r[i] = r[j];
	r[j] = l;
	int p;
	for (int k = i+1; k <= (i+(n-i-1)/2); k++){
        p = r[k];
        r[k] = r[n-(k-i)];
        r[n-(k-i)] = p;
    };
	return true;
}; 
//Вывод
inline ostream& operator << (ostream& out, const route& x){
	int k =x.n-1;
	for (int i = 0; i<k; i++)
		out << x.r[i]+1 << ", ";
	out << x.r[k]+1;
	out << endl;
	return out;
}
int main(){
	int k = 0;
	cout << "Enter number of cities: ";
    cin >> k;
	int **matrix = new int* [k];
	for (int count = 0; count < k; count++)
		matrix[count] = new int [k];
	for (int i = 0; i<k; i++)
		for (int j = 0; j<k; j++)
			cin >> matrix[i][j];
	route x(k);	
	int price;
	bool work = true;
	int min_price = x.route_price(matrix);
	route min = x;
    while (work){
		cout << x;
		price = x.route_price(matrix);
		if (price < min_price){
			min_price = price;
			min = x;
		};
		cout << price<<endl;
		work = x.next_route();
	};
	cout << "result = " << min << endl;
	cout << "min_price = " << min_price;
	for (int count = 0; count < k; count++)
		delete matrix[count];
	delete matrix;
	return 0;
}	