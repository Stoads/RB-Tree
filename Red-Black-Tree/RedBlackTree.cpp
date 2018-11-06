#include <stdio.h>
#include <string>
#include "RedBlackTree.h"
using namespace std;
int main() {
	RedBlackTree<string, int> m;
	m["A"] = 123;
	m["C"] = 4;
	m["E"] = 5;
	m["D"] = 1;
	m["B"] = 9;
	for (auto i = m.getFirst(); i != m.getEnd(); i++)
		printf("%s %d\n", i.getKey().c_str(), i);
	m.erase("123");
	printf("Size : %d\n", m.size());
}