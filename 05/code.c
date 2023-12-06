#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

typedef struct Range Range;

struct Range {
	long long start;
	long long length;
	bool changed;
};

int nrange;
Range *ranges;

void splitRange(Range *orig, Range *new, long long destination) {
	if (orig->start > new->start + new->length) return;
	if (new->start > orig->start + orig->length) return;

	//printf("intersection found: destination %lld\n\t\torig: start %lld length %lld\n\t\t new: start %lld length %lld\n", 
	//		destination, orig->start, orig->length, new->start, new->length);
	// orig is entirely within new
	if (orig->start >= new->start && orig->start + orig->length <= new->start + new->length) {
		//printf("\t\torig inside\n");
		orig->start = destination + (orig->start - new->start);
		orig->changed = true;
		return;
	}
	// orig on right, new on left
	if (orig->start >= new->start && new->start + new->length <= orig->start + orig->length) {
		//printf("\t\torig on right\n");
		Range newRange;
		newRange.start = destination + (orig->start - new->start);
		newRange.length = new->start + new->length - orig->start;
		newRange.changed = true;
		orig->start = new->start + new->length;
		orig->length -= newRange.length;
		ranges[nrange] = newRange;
		nrange++;
		return;
	}
	// end of orig is within new, but start of orig isn't
	if (orig->start < new->start && orig->start + orig->length <= new->start + new->length) {
		//printf("\t\torig on left\n");
		Range newRange;
		newRange.start = destination;
		newRange.length = orig->start + orig->length - new->start;
		newRange.changed = true;
		orig->length -= newRange.length;
		ranges[nrange] = newRange;
		nrange++;
		return;
	}
	// new is entirely within orig
	if (new->start > orig->start && new->start + new->length < orig->start + orig->length) {
		//printf("\t\tnew inside\n");
		Range midRange, endRange;
		midRange.start = destination;
		midRange.length = new->length;
		midRange.changed = true;
		endRange.start = new->start + new->length;
		endRange.length = orig->start + orig->length - (new->start + new->length);
		endRange.changed = false;
		orig->length = new->start - orig->start;
		ranges[nrange] = midRange;
		nrange++;
		ranges[nrange] = endRange;
		nrange++;
		return;
	}
	printf("check for off by 1 errors\n");
}

void printRanges() {
	printf("All ranges: \n");
	for (int i=0;i<nrange;i++) {
		printf("\tstart: %lld, length: %lld\n", ranges[i].start, ranges[i].length);
	}
}

int main() {
	ssize_t bufsize = 1000;
	char *buf = malloc(bufsize);
	long long firstAns = 0;
	long long secondAns = 0;

	int nseeds = 0;
	long long seeds[30];
	bool changed[30];

	nrange = 0;
	ranges = malloc(sizeof(Range) * 50000);

	int n = getline(&buf, &bufsize, stdin);

	int i = 6;
	while (true) {
		if (buf[i] == '\n') break;
		i++;
		long long val = 0;
		while (buf[i] >= '0' && buf[i] <= '9') {
			val *= 10;
			val += buf[i++] - '0';
		}
		seeds[nseeds++] = val;
	}

	i = 6;
	while (true) {
		if (buf[i] == '\n') break;
		i++;
		long long val = 0;
		while (buf[i] >= '0' && buf[i] <= '9') {
			val *= 10;
			val += buf[i++] - '0';
		}
		i++;
		ranges[nrange].start = val;
		val = 0;
		while (buf[i] >= '0' && buf[i] <= '9') {
			val *= 10;
			val += buf[i++] - '0';
		}
		ranges[nrange].length = val;
		nrange++;
	}

	//printRanges();

	getline(&buf, &bufsize, stdin);

	while (true) {
		n = getline(&buf, &bufsize, stdin);
		if (n == -1) {
			break;
		}

		for (int i=0;i<nseeds;i++) {
			changed[i] = false;
		}

		for (int i=0;i<nrange;i++) {
			ranges[i].changed = false;
		}

		while (true) {

			n = getline(&buf, &bufsize, stdin);
			if (n < 3) break;
			long long destination = 0, source = 0, range = 0;

			int i=0;
			while (buf[i] != ' ') {
				destination *= 10;
				destination += buf[i++] - '0';
			}
			i++;
			while (buf[i] != ' ') {
				source *= 10;
				source += buf[i++] - '0';
			}
			i++;
			while (buf[i] != '\n') {
				range *= 10;
				range += buf[i++] - '0';
			}

			Range newRange;
			newRange.start = source;
			newRange.length = range;

			for (int j = 0;j<nseeds;j++) {
				if (!changed[j]) {
					if (seeds[j] >= source && seeds[j] < source + range) {
						seeds[j] = destination + (seeds[j] - source);
						changed[j] = true;
					}
				}
			}

			for (int j=0;j<nrange;j++) {
				if (!ranges[j].changed) {
					splitRange(ranges+j, &newRange, destination);
				}
			}
		}
		//printRanges();
	}

	firstAns = LLONG_MAX;
	for (int i=0;i<nseeds;i++) {
		if (seeds[i] < firstAns) firstAns = seeds[i];
	}

	secondAns = LLONG_MAX;
	for (int i=0;i<nrange;i++) {
		if (ranges[i].start < secondAns) secondAns = ranges[i].start;
	}

	printf("first answer = %lld\n", firstAns);
	printf("second answer = %d\n", secondAns);
	return 0;
}
