#ifndef _DSETS_H_
#define _DSETS_H_

#include <vector>

using namespace std;

class DisjointSets {
	private:
		vector<int> set;
		
	public:
		void addelements(int n);
		int find(int n);
		void setunion(int a, int b);
};

#endif
