#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Аргументы:
// 1 - путь до файла
// 2 - первый набор
// 3 - второй набор


int main(int argc, char* argv[]) {
	int i, n;
	char c;
	FILE *fp;

	if (argc != 4) {
		puts("Неправильное количество аргументов");
		return 0;
	}

	if ((n = strlen(argv[2])) != strlen(argv[3])) {
		puts("Наборы разной длины");
		return 0;
	}

	fp = fopen(argv[1], "r+");

	while (!feof(fp)) {              // Пока не конец файла
		c = getc(fp);                // Считываем следующий символ из файла

		for (i = 0; i < n; ++i)      // Ищем его в первом наборе
			if (c == argv[2][i])
				break;

		if (i == n)                  // Если не нашли, то переходим к следующему символу
			continue;

		fseek(fp, -1, SEEK_CUR);     // Если нашли, то возращаемся на предыдущюю позицию
		putc(argv[3][i], fp);        // И перезаписываем текущий символ на соответствующий символ второго набора
	}

	return 0;
}
