#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main() {
	ssize_t bufsize = 1000;
	char *buf = malloc(bufsize);
	int firstAns = 0;
	int secondAns = 0;

	while (true) {
		int n = getline(&buf, &bufsize, stdin);
		if (n == -1) {
			break;
		}

		int maxr = 0, maxg = 0, maxb = 0;

		int gameId = 0;
		int i;
		bool possible = true;

		for (i=5;buf[i] != ':';i++) {
			gameId *= 10;
			gameId += buf[i] - '0';
		}

		i += 2;
		while (true) {
			int marbles = 0;
			for (;buf[i] != ' ';i++) {
				marbles *= 10;
				marbles += buf[i] - '0';
			}
			i++;

			if (buf[i] == 'r') {
				if (marbles > maxr) maxr = marbles;
				if (marbles > 12) {
					possible = false;
				}
				i += 3;
			} else if (buf[i] == 'g') {
				if (marbles > maxg) maxg = marbles;
				if (marbles > 13) {
					possible = false;
				}
				i += 5;
			} else if (buf[i] == 'b') {
				if (marbles > maxb) maxb = marbles;
				if (marbles > 14) {
					possible = false;
				}
				i += 4;
			} else {
				printf("unexpected char: %c\n", buf[i]);
			}

			if (buf[i] == '\n') break;
			i += 2;
		}
		if (possible) {
			firstAns += gameId;
		}
		int power = maxr * maxg * maxb;
		secondAns += power;
	}

	printf("first answer = %d\n", firstAns);
	printf("second answer = %d\n", secondAns);
	return 0;
}
