#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
char * strrstr(char *string, char *find, ssize_t len)
{
  //I see the find in string when i print it 
  //printf("%s", string);
  char *cp;
  int flen = strlen(find);
  for (cp = string + len - flen; cp >= string; cp--)
  {
    if (strncmp(cp, find, flen) == 0)
        return cp;
  }
  return NULL;
}

char *nums[] = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

int firstNum(char *buf, int n) {
	for (int i=0;i<n;i++) {
		if (buf[i] >= '0' && buf[i] <= '9') {
			return buf[i] - '0';
		}
	}
	return -1;
}

int lastNum(char *buf, int n) {
	for (int i=n-1;i>=0;i--) {
		if (buf[i] >= '0' && buf[i] <= '9') {
			return buf[i] - '0';
		}
	}
	return -1;
}
	
void firstWord(char *str, int *ret) {
	ret[0] = 9999;
	ret[1] = -1;

	for (int i=0;i<10;i++) {
		char *c = strstr(str, nums[i]);
		if (c == NULL || c-str > ret[0])
			continue;
		ret[0] = c-str;
		ret[1] = i;
	}
}

int lastWord(char *str, int *ret) {
	ret[0] = -1;
	ret[1] = -1;

	for (int i=0;i<10;i++) {
		char *c = strrstr(str, nums[i], strlen(str));
		if (c == NULL || c-str < ret[0])
			continue;
		ret[0] = c-str;
		ret[1] = i;
	}
}

int main() {
	ssize_t bufsize = 200;
	char *buf = malloc(bufsize);
	int firstAns = 0;
	int secondAns = 0;

	int first[2];
	int second[2];

	while (true) {
		int n = getline(&buf, &bufsize, stdin);
		if (n == -1) {
			break;
		}

		first[0] = firstNum(buf, n);
		first[1] = lastNum(buf, n);

		firstAns += first[0]*10 + first[1];

		int nums[2];
		firstWord(buf, nums);

		if (first[0] < 0 || nums[0] < strchr(buf, first[0]+'0')-buf) {
			printf("nums = %d\n", nums[1]);
			second[0] = nums[1];
		} else {
			printf("first = %d\n", first[0]);
			second[0] = first[0];
		}

		lastWord(buf, nums);

		if (nums[0] > strrchr(buf, first[1]+'0')-buf || first[1] == -1) {
			second[1] = nums[1];
		} else {
			second[1] = first[1];
		}

		secondAns += second[0]*10 + second[1];
		printf("second val = %d, make from %d and %d\n", second[0]*10 + second[1], second[0], second[1]);
			
	}

	printf("first answer = %d\n", firstAns);
	printf("second answer = %d\n", secondAns);
	return 0;
}
