#include "dsets.h"


void DisjointSets::addelements(int n) {
	for (int i = 0; i < n; i++) {
		set.push_back(-1);
	}
}

int DisjointSets::find(int n) {
	if (set[n] < 0) {return n;}
	else {return find(set[n]);}
}

void DisjointSets::setunion(int a, int b) {
	a = find(a);
	b = find(b);
	
	int size = set[a] + set[b];
	
	if (set[b] < set[a]) {
		set[a] = b;
		set[b] = size;
	}
	else {
		set[b] = a;
		set[a] = size;
	}	
}
