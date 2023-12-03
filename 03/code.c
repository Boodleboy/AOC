#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int lines;

int getNum(char grid[][150], int i, int j) {
	int val = 0;
	
	char cur = grid[i][j];
	while (j >= 0 && (cur >= '0' && cur <= '9')) {
		j--;
		cur = grid[i][j];
	}

	j++;
	cur = grid[i][j];

	while (cur >= '0' && cur <= '9') {
		val *= 10;
		val += cur - '0';
		j++;
		cur = grid[i][j];
	}
	return val;
}

int condGetNum(char grid[][150], int i, int j, int *val, int *nnums) {
	if (i < 0 || j < 0 || i >= lines || j >= lines) {
		return 0;
	}

	char c = grid[i][j];
	printf("%d, %d: %c\n", i, j, c);

	if (c >= '0' && c <= '9') {
		int num = getNum(grid, i, j);
		*val *= num;
		*nnums += 1;

		printf("condGetNum = %d\n", num);
		return num;
	}
	return 0;
}


int main() {
	ssize_t bufsize = 1000;
	char *buf = malloc(bufsize);
	char grid[150][150];

	int firstAns = 0;
	int secondAns = 0;

	lines = 0;

	while (true) {
		int n = getline(&buf, &bufsize, stdin);
		if (n == -1) {
			break;
		}

		strncpy(grid[lines], buf, n);
		grid[lines][n-1] = '.'; 
		lines++;
	}

	for (int i=0;i<lines;i++) {
		for (int j=0;j<lines;j++) {
			char cur = grid[i][j];
			if (cur >= '0' && cur <= '9') {
				int num = 0;
				int leftj = j == 0 ? 0 : j-1;
				while (cur >= '0' && cur <= '9') {
					num *= 10;
					num += cur - '0';
					cur = grid[i][++j];
				}
				int rightj = j;
				bool hasSpec = false;
				for (int k=leftj;k<=rightj;k++) {
					if (i > 0) {
						if (grid[i-1][k] != '.') {
							hasSpec = true;
							break;
						}
					}
					if (i < lines-1) {
						if (grid[i+1][k] != '.') {
							hasSpec = true;
							break;
						}
					}
				}
				if (leftj > 0 && grid[i][leftj] != '.') {
					hasSpec = true;
				} 
				if (grid[i][rightj] != '.') {
					hasSpec = true;
				}

				if (hasSpec) {
					firstAns += num;
				}
			}
		}
	}

	for (int i=0;i<lines;i++) {
		for (int j=0;j<lines;j++) {
			if (grid[i][j] == '*') {
				int nnums = 0;
				int val = 1;
				printf("testing * at %d %d\n", i, j);

				int n = condGetNum(grid, i-1, j, &val, &nnums);
				
				if (!n) {
					condGetNum(grid, i-1, j-1, &val, &nnums);
					condGetNum(grid, i-1, j+1, &val, &nnums);
				}

				n = condGetNum(grid, i+1, j, &val, &nnums);

				if (!n) {
					condGetNum(grid, i+1, j-1, &val, &nnums);
					condGetNum(grid, i+1, j+1, &val, &nnums);
				}

				condGetNum(grid, i, j-1, &val, &nnums);
				condGetNum(grid, i, j+1, &val, &nnums);

				if (nnums == 2) {
					secondAns += val;
				}

			}
		}
	}

	printf("first answer = %d\n", firstAns);
	printf("second answer = %d\n", secondAns);
	return 0;
}
