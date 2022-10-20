#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>

using namespace std;

class Array{
	int *a;
    int	m;
public:
    Array (int mm=1, int t=1, int d=10){
		a = new int [mm];
		m = mm;
		if (t == 1){
			for (int i=0; i < mm; i++)
				a[i] = rand () % d + 1;
		};
		if (t == 2){
			a[0] = rand()%3+1;
			for (int i=1; i<mm; i++)
				a[i] = a[i-1] + rand() % 3 + 1;
		};
		if (t == 3){
			a[m-1] = rand () % 10 + 1;
			for (int i=mm-2; i>=0; i--)
				a[i] = a[i+1] + rand () % (a[i+1]+10);
		};
	};
	Array (int* mas, int mm){
		a = new int [mm]();
		m = mm;
		for (int i = 0; i < mm; i++)
			a[i] = mas[i];
	};
	Array (Array& mas){
		a = new int [mas.m];
	    for (m = 0; m < mas.m; m++)
			a[m] = mas.a[m];
	};
	~Array(){
		delete [] a;
	};
	Array& operator = (Array&);
	int& operator [] (int);
	bool Test();
	bool operator == (Array);
	friend istream & operator >> (istream&, Array&);
	friend ostream & operator << (ostream&, Array&);
	int size ();
	void Shell ();
	void Shaker ();
	void Pir_sort ();
	void Sift(int *a, int n, int i);
	void Bit_sort ();
	void BitSort (int* a, int l, int r, int k);
	void Quick_sort ();
	void quick (int*a, int l, int r);
};
inline Array& Array:: operator = (Array& x){
	if (this != &x){
		delete [] a;
		a = new int [m = x.m];
		for (int i = 0; i < m; i++)
			a[i] = x[i];
	};
	return *this;
};
inline int& Array:: operator [] (int i){
	if (i < 0 || i >= m){
		cout << "Error" << endl;
		exit (0);
	};
	return a[i];
};
inline bool Array:: Test (){
	for (int i = 0; i < m-1; i++){
	    if (a[i]>a[i+1])
			return false;
	};
	return true;
};
inline bool Array:: operator == (Array mas){
	if (m != mas.m)
		return false;
	int *x = new int [m]();
	for (int i = 0; i<m; i++){
		int j = 0;
		while ((a[j] != mas[i] || x[j] == 1) && j<m)
			    j++;
		if (j >= m)
			return false;
		x[j] = 1;
	};
	delete [] x;
	return true;
};
inline istream & operator >> (istream& in, Array& x){
	delete [] x.a;
	cout << "How many elements will be in the array?" ;
	cin >> x.m;
	x.a = new int [x.m];
	for (int i = 0; i<x.m; i++)
		in >> x[i];
	return in;
};
inline ostream & operator << (ostream& out, Array& x){
	for (int i = 0; i<x.m; i++)
		out << x[i] << " ";
	return out;
};
inline void Array:: Shell (){
	double start = clock() / 1000.0; 
	int t = (log(m)/log(2))-1;
	int *h = new int [t]();
	h[0] = m / 2;  //шаг  
    for(int i = 1; i<=t; i++)
		h[i] = h[i-1]/2;
	int i = 0;
    while (i<=t)
    {
		int s = h[i];
		int b = 0;
		while(b<s){
			int j = b+s;
            while(j<m){
				int x = a[j];
				int k = j-s;
				while(k>=0 && a[k]>x){
					a[k+s] = a[k];
					k = k-s;
				};
				a[k+s] = x;
				j = j+s;
			};
			b++;
		};
		i++;
    };
    double end = clock() / 1000.0;
    cout << "Time Shell: " <<  end - start  << endl;
	delete [] h;
	return;
};
inline void Array:: Shaker (){
	double start = clock() / 1000.0;
	int L = 0;
	int R = m-1;
	while(L<R){
		int i = L;
		int R1 = L;
		while(i<R){
			if (a[i] > a[i+1]){
				int t = a[i];
				a[i] = a[i+1];
				a[i+1] = t;
				R1 = i;
			};
			i++;
		};
		R = R1;
		i = R;
		int L1 = R;
		while(i>L){
			if (a[i]<a[i-1]){
				int t = a[i];
				a[i] = a[i-1];
				a[i-1] = t;
				L1 = i;
			};
			i--;
		};
		L=L1;
	};
    double end = clock() / 1000.0;
    cout << "Time Shaker : " <<  end - start  << endl;
	return;
};
inline void Array:: Pir_sort (){
	double start = clock() / 1000.0;
    for (int i = m / 2 - 1; i >= 0; i--)
        Sift(a, m, i);
	
    for (int i=m-1; i>=0; i--){
        int t = a[0];
        a[0] = a[i];
        a[i] = t;
        Sift(a, i, 0);
    };

	double end = clock() / 1000.0;
    cout << "Time Pir_sort : " <<  end - start  << endl;
	return;
};
inline void Array:: Sift(int *a, int n, int i){
    int j = 2*i+1;   
    int x = a[i];
    int f = 1;	
    while (j < n && f){
        if (j+1 < n)
            if (a[j+1]>a[j])
                j = j+1;
	    if (a[j]>x){
		    a[i] = a[j];
		    i = j;
		    j = 2*i+1;
	    } else {
		    f = 0;
		};
	};
	a[i] = x;
		return;
};
inline void Array:: Bit_sort (){
	double start = clock() / 1000.0;
	int max = a[0];
	for (int i = 1; i<m; i++){
		if (a[i]>max)
			max = a[i];
	};
	int k = log(max)/log(2.0);
	int l = 0;
	int r = m-1;
	BitSort(a, l, r, k);
	double end = clock() / 1000.0;
    cout << "Time Bit_sort : " <<  end - start  << endl;
	return;
};
inline void Array:: BitSort(int *a, int l, int r, int k){
    if(l >= r || k < 0) 
		return;
    int i = l, j = r;
    while(i <= j){
        while(i <= j && (bool(a[i] & (1<<k))) == 0) 
	        i++;
        while(i <= j && (bool(a[j] & (1<<k))) == 1) 
	        j--;
        if(i < j){
            int t = a[i];
		    a[i] = a[j];
		    a[j] = t;
            i++;
            j--;
        };
    };
    BitSort(a, l, j, k-1);
    BitSort(a, i, r, k-1);
};
inline void Array:: Quick_sort (){
	double start = clock() / 1000.0;
	int l = 0;
	int r = m-1;
	quick(a, l, r);
	double end = clock() / 1000.0;
    cout << "Time Quick_sort : " <<  end - start  << endl;
	return;
};
inline void Array:: quick (int *a, int l, int r){
	if (l<r){	
	    int i = l;
	    int j = r;
	    int x = a[(l+r)/2];
		
		while (i<=j){
	        while (a[i]<x)
		        i++;
	        while (a[j]>x)
		        j--;
	        if (i<=j){
		        int t = a[i];
		        a[i] = a[j];
		        a[j] = t;
		        i++;
		        j--;
	        };
		};
	    quick(a, l, j);
	    quick(a, i, r);
		
	};
	return;
};
int main(){
	srand(time(NULL));
	Array a(100000, 1, 10000), b;

	b = a;
	a.Shaker();
	if (a.Test() && (a==b))
		cout << "Shaker OK" << endl;
	a = b;
	a.Pir_sort();
	if (a.Test() && (a==b))
		cout << "Pir_sort OK" << endl;
	a = b;
	a.Bit_sort();
	if (a.Test() && (a==b))
		cout << "Bit_sort OK" << endl;
	a = b;
	a.Quick_sort();
	if (a.Test() && (a==b))
		cout << "Quick_sort OK" << endl;
	a = b;
	a.Shell();
	if (a.Test() && (a==b))
		cout << "Shell_sort OK" << endl;
	return 0;
}