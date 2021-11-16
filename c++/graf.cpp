#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <fstream>
using namespace std;

class Boolv {
private:
	unsigned int* b;
	int nbit, m;		// nbit – количество бит вектора, m – количество элементов массива b

public:
    //Конструктор 1, создает нулевой вектор длины  nn
	Boolv(int nn = 1) {
		if (nn <= 0) 
		    throw "error:  nn";        //
		else {
			nbit = nn;
			m = ((nn - 1) / 32) + 1;
			b = new unsigned int[m];
			for(int i=0; i<m; i++) 
			    b[i] = 0;
		}
	}
    //Конструктор 2, создает вектор из строки
	Boolv(const char* str) {
		if (!str){ 
		    throw "error:  str";
		} else {
			nbit = strlen(str)-1;;
			m = ((nbit - 1) / 32) + 1;
			b = new unsigned int[m];						
			for (int i = 0; i < m; i++) 
			    b[i] = 0;		
			int count = 0;
			unsigned int mask;
			for (int i = 0; i < m; i++) {
				mask = 1;
				while (mask > 0 && count < nbit) {
					if (str[count] != '0') {
						b[i] |= mask;
					};
					mask = mask << 1;
					count++;
				};
			};
		};	
	}
    //Конструктор копирования
	Boolv(const Boolv& vec) {
		if (!vec.b) 
		    throw "error:  b";
		else {
			nbit = vec.nbit;
			m = vec.m;

			b = new unsigned int[m];						//создаем массив, там мусор

			for (int i = 0; i < m; i++) {
				b[i] = vec.b[i];						//не чистим, мы все равно присваиваем значения
			};
		};
	}
    // Деструктор
	~Boolv()
	{
		delete[] b;
	};

    //Установить в i-тый бит 1
	void Set1(int i) {
		if (i < 0 || i > nbit) 
		    throw "error: Set1";
		else {
			int bit = i / 32;
			unsigned int mask = 1;
			mask = mask << i % 32;
			b[bit] = b[bit] | mask;
		};
	};
    //Установить в i-тый бит 0
	void Set0(int i) {
		if (i < 0 || i > nbit) 
		    throw "error: Set0";
		else {
			int bit = i / 32;
			unsigned int mask = 1;
			mask = mask << i % 32;
			b[bit] = b[bit] & ~mask;
		};
	};
    //Оператор []
	int operator [] (int i) {
		if (i < 0 || i > nbit) 
		    throw "error: operator[]";
		else {
			int bite = i / 32;
			unsigned int mask = 1;
			mask = mask << i % 32;
			if ((b[bite] & mask) == mask) 
			    return 1;
			else return 0;
		};
	};
    //Оператор =
	Boolv operator = (const Boolv& vec) {
		if (this != &vec) {
			nbit = vec.nbit;
			m = vec.m;
			delete[] b;
			b = new unsigned int[m];						
			for (int i = 0; i < m; i++) {
				b[i] = vec.b[i];						
			};
		};
		return *this;
	};


    //Побитовое сложение
	Boolv operator | (const Boolv& vec) {
		Boolv result;
		if (nbit >= vec.nbit) {													//если левый (this) больше правого (vec)
			result = *this;														//то тогда складываем левый (больший) с меньшим
			for (int i = 0; i < vec.m; i++) 
			    result.b[i] = result.b[i] | vec.b[i];
		} else {																		//аналогично для случая, когда
			result = vec;														//правый (vec) больше левого (this)
			for (int i = 0; i < m; i++) 
			    result.b[i] = result.b[i] | b[i];
		}
		return result;
	}
    //Побитовое умножение
	Boolv operator & (const Boolv& vec) {
		Boolv result;
		if (nbit >= vec.nbit) {													//если левый (this) больше правого (vec)
			result = *this;														
			for (int i = 0; i < vec.m; i++) 
			    result.b[i] = result.b[i] & vec.b[i];
			for (int i = vec.m; i < m; i++) 
			    result.b[i] = 0;
			return result;
		} else {																    
			result = vec;														//правый (vec) больше левого (this)
			for (int i = 0; i < m; i++) 
			    result.b[i] = result.b[i] & b[i];
			for (int i = m; i < vec.m; i++) 
			    result.b[i] = 0;
			return result;
		}
	}
    //Инверсия
	Boolv operator ~ () {
		Boolv result = *this;
		unsigned int mask = 0;
		mask = ~mask;
		mask = mask << nbit % 32;
		for (int i = 0; i < m; i++) 
		    result.b[i] = ~b[i];
		result.b[m - 1] = result.b[m - 1] & ~mask;
		return result;
	};
    //Равенство векторов
	bool operator == (const Boolv& vec) {
		if (nbit != vec.nbit) 
		    return false;
		else {
			for (int i = 0; i < m; i++) {
				if (b[i] != vec.b[i]) 
				     return false;
			};
			return true;
		};
	};
    //Вес вектора
	int weight() {
		int weight = 0;
		unsigned int mask = 1;
		for (int i = 0; i < nbit; i++) {
			if((b[(i - 1) / 32] & mask) != 0) 
			    weight++;
			mask <<= 1;
		};
		return weight;
	};
	friend ostream& operator << (ostream&, Boolv&);
};

//Вывод вектора
ostream& operator << (ostream& out, Boolv& vec) {
	int count = 0, i=0;
	unsigned int mask;
	while (count < vec.nbit) {
		mask = 1;
		while (mask > 0 && count < vec.nbit) {
			if ((vec.b[i] & mask) != 0) {
				out << '1';
			}else {
				out << '0';
			};
			mask <<= 1;
			count++;
		};
		i++;
	};
	return out;
};

class BoolM {
private:
	Boolv* bm;
	int m, n;							// m – количество строк, n – количество столбцов

public:
	BoolM(int k = 1, int l = 1) {		// формируется нулевая булева матрица размера k на l
		if (k <= 0 || l <= 0) 
		    throw "error: constructor1 BoolM";
		else {
			m = l;
			n = k;
			bm = new Boolv[m];
			Boolv b(n);
			for (int i = 0; i < m; i++) {
				bm[i] = b;
			};
		};
	};
	//Конструктор копирования
	BoolM(const BoolM& matrix) {
		
		m = matrix.m;
		n = matrix.n;
		bm = new Boolv[m];
		for (int i = 0; i < m; i++){
			bm[i] = matrix.bm[i];
		};
	};

	Boolv& operator[] (int k) {
		if (k < 0 || k > n) 
		    throw "error: matrix operator[]";
		else 
			return bm[k];
	};

	BoolM operator = (const BoolM& matrix) {
		if (this != &matrix) {
			m = matrix.m;
			n = matrix.n;
			delete[] bm;
			bm = new Boolv[m];
			for (int i = 0; i < m; i++) {
				bm[i] = matrix.bm[i];
			};
		};
		return *this;
	};
    //Строим матрицу по данным из файла
	int BuildMatrix() {
		ifstream file("Graf.txt");
		if (file.is_open()) {
			int a, b, size;
			file >> size;
			BoolM BM(size, size);
			do {
				file >> a;
				file >> b;
				BM.bm[a - 1].Set1(b - 1);
			} while (!file.eof());
			file.close();
			*this = BM;
			return size;
		};
		return 0;
	};
    //считаем дизъюнцию строк в массиве, чтобы найти нулевые строки
	Boolv DisVec() {						
		Boolv result(n);
		for (int i = 0; i < m; i++) {
			result = result | bm[i];
		};
		return result;
	};
	friend ostream& operator << (ostream&, BoolM&);
};

//Вывод матрицы
ostream& operator << (ostream& out, BoolM& matrix) {
	for (int i = 0; i < matrix.m; i++) {
		out << matrix.bm[i] << endl;
	}
	return out;
}

//Топологическая сортировка
void Topsort(BoolM& M, int m, int** a) {
	Boolv v0(m);											    //пройденные вершины
	Boolv v1 = M.DisVec();										//дизъюнкция всех строк матрицы
	Boolv NullV(m);												//для изменения значения 
	int count = 0;												//счетчик для массива а
	*a = new int[m];											//отсортированный массив
	while ((v1.weight() != m - count) && (v0.weight() != m)) {	//работает пока не заполнится вектор v0 или пока вес v1 не станет равен m-count
		for (int i = 0; i < m; i++) {							//при выполнении одного из этих условий мы отсортировали все
			if ((v1[i] == 0) && (v0[i] != 1)) {					//если какой-нибудь из битов v1 = 0, значит что есть нулевой столбец 
				M[i] = NullV;									//обнуляем строку, соответствующую номеру столбца
				(*a)[count] = i + 1;							//в массив записываем номер этого столбец и увеличиваем счетчик
				count++;
			};
		};
		
		v0 = v0 | (~v1);										//делаем дизъюнкцию с инверсией вектора v1
		v1 = M.DisVec();										//высчитываем v1

		cout << "v0: " << v0 << endl;
		cout << "v1: " << v1 << endl;
		cout << M << endl;
	};
	if (v0.weight() != m) {										//если у нас нет вершин, у которых нет предшествеников, или есть цикл
		delete[](*a);											//удаляем массив  
		delete a;												
		cout << "There are no zero rows" << endl;               //массиве нет таких нулевых строк
	} else {
	    for (int i = 0; i < m; i++) {
		    cout << a[i] << ' ';
		};
	};
};



int main() {
	//Создание векторов и проверка операторов
	Boolv b1("01010101");
	cout << "b1:" << b1 << endl;

	Boolv b2("10101010");
	cout << "b2:" << b2 << endl;

	if (b1 == b1) 
	    cout << "b1 == b1" << endl;
	else 
	    cout << "b1 != b1" << endl;
	
	if(b1 == b2) 
	    cout << "b1 == b2" << endl;
	else 
	    cout << "b1 != b2" << endl;

	if(b1 == ~b2) 
	    cout << "b1 == ~b2" << endl;
	else 
	    cout << "b1 != ~b2" << endl;

	Boolv b3 = ~b1;
	cout << "~b1:" << b3 << endl;

	b3 = b1 & b2;
	cout << "b1&b2:" << b3 << endl;

	b3 = b1 | b2;
	cout << "b1|b2:" << b3 << endl;

    //Создание матрицы и топологическая сортировка
	BoolM m1;
	int* a;
	int mn;
	mn = m1.BuildMatrix();

	cout << m1 << endl;
    
	Topsort(m1, mn, &a);
	return 0;
}
