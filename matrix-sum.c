#include <stdio.h>
#include <stdlib.h>


int get_sum(int n, int **matrix) {
	int sum = 0;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			if ((i > j && i + j < n - 1) || (i < j && i + j > n - 1))
				sum += matrix[i][j];

	return sum;
}


int main() {
	int n;
	int **matrix;

	printf("n = ");
	scanf("%d", &n);

	printf("Матрица %dx%d:\n", n, n);
	matrix = malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) {
		matrix[i] = malloc(n * sizeof(int));
		for (int j = 0; j < n; ++j)
			scanf("%d", &matrix[i][j]);
	}

	int s = get_sum(n, matrix);
	printf("s = %d\n", s);

	for (int i = 0; i < n; ++i)
		free(matrix[i]);
	free(matrix);

	return 0;
}
