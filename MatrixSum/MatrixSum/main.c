#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Эта программа вычисляет сумму элементов матрицы,
    находящихся в левом и правом треугольниках, получающихся
    при "разрезании" диагоналями. Например для матриц
    5x5 и 6x6 будут выбраны элементы, отмеченные '#':

		.....    	......
    	#...#    	#....#
    	##.##    	##..##
    	#...#    	##..##
    	.....    	#....#
        	     	......

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


typedef int    Elem;	// Тип элемента матрицы
typedef Elem** Matrix;	// Тип матрицы


// Вывод справки по программе
void print_help() {
    puts("Эта программа вычисляет сумму элементов матрицы, "
         "находящихся в левом и правом треугольниках, получающихся "
         "при \"разрезании\" диагоналями. Например для матриц "
         "5x5 и 6x6 будут выбраны элементы, отмеченные '#':");
    puts("");
    puts(".....    ......");
    puts("#...#    #....#");
    puts("##.##    ##..##");
    puts("#...#    ##..##");
    puts(".....    #....#");
    puts("         ......");
    puts("");
}


// Ввод размера матрицы
int input_matrix_size() {
    int size;
    printf("Введите размер матрицы: ");
    scanf("%d", &size);

	if (size <= 0) {
		puts("Ошибка: размер матрицы должен быть положительным");
		exit(1);
	}
    
    return size;
}


// Создание матрицы
Matrix create_matrix(int size) {
	Matrix matrix = malloc(size * sizeof(Elem*));					// Выделяем память под указатели на строки
	if (!matrix) {													// Проверяем успешность выделения памяти
		puts("Ошибка: не удалось выделить память под матрицу");		// При ошибке выводим сообщение
		exit(1);													// И завершаем приложение с ненулевым кодом
	}

	for (int i = 0; i < size; ++i) {
		matrix[i] = malloc(size * sizeof(Elem));					// Выделяем память под каждую строку
		if (!matrix[i]) {											// Проверяем успешность выделения памяти
			puts("Ошибка: не удалось выделить память под матрицу");	// При ошибке выводим сообщение
			exit(1);												// И завершаем приложение с ненулевым кодом
		}
	}

	return matrix;
}


// Удаление матрицы
void delete_matrix(Matrix matrix, int size) {
	for (int i = 0; i < size; ++i)
		free(matrix[i]);
	free(matrix);
}


// Заполнение матрицы одинаковыми значениями
void fill_matrix(Matrix matrix, int size, Elem value) {
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			matrix[i][j] = value;
}


// Заполнение матрицы случайными числами
void fill_matrix_random(Matrix matrix, int size, Elem min, Elem max) {
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			matrix[i][j] = (rand() % (max - min + 1)) + min;
	// Здесь ((rand() % (max - min + 1)) + min) -- случайное число,
	// лежащее между min и max включительно
}


// Ввод матрицы
void input_matrix(Matrix matrix, int size) {
	printf("Введите значения элементов матрицы %dx%d. Вводите значения по строкам, "
		   "значения разделяйте пробелом.\n", size, size);
	for (int i = 0; i < size; ++i) {
		printf("строка %d: ", i + 1);
		for (int j = 0; j < size; ++j)
			scanf("%d", &matrix[i][j]);
	}
}


// Вывод матрицы
void print_matrix(Matrix matrix, int size) {
	printf("Вычисляем сумму для следующей матрицы %dx%d:\n", size, size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j)
			printf("%4d ", matrix[i][j]);
		puts("");
	}
}


// Вычисление суммы элементов
int get_sum(Matrix matrix, int size) {
	Elem sum = 0;

	//     i > j        -- ниже главной диагонали
	//     i < j 		-- выше главной диагонали
	// i + j < size - 1 -- выше побочной диагонали
	// i + j > size - 1 -- ниже побочной диагонали

	// Таким образом:
	// (i > j && i + j < size - 1) соответствует левому треугольнику
	// (i < j && i + j > size - 1) соответствует правому треугольнику

	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			if ((i > j && i + j < size - 1) || (i < j && i + j > size - 1))
				sum += matrix[i][j];

	return sum;
}


int main(int argc, char **argv) {
	for (int i = 1; i < argc; ++i)
		if (strcmp(argv[i], "-h") == 0) {	// Проверяем аргументы коммандной строки на наличие "-h"
		    print_help();					// Если есть, то выводим справку
		    exit(0);						// И завершаем работу
		}

	const int size = input_matrix_size();	// Вводим размер матрицы
	Matrix matrix = create_matrix(size);	// Создаём матрицу данного размера

	input_matrix(matrix, size);				// Вводим саму матрицу (значения)
	print_matrix(matrix, size);				// Выводим матрицу (для проверки)

	int sum = get_sum(matrix, size);		// Вычисляем искомую сумму
	printf("Искомая сумма для введённой матрицы равна %d\n", sum);		// Выводим результат

	delete_matrix(matrix, size);			// Очищаем память, выделенную под матрицу
}
