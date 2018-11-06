#include "RedBlackTree.h"
#include "test.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
using namespace std;
void test1() {
	RedBlackTree<int, int> i_i_map;
	for (int i = 0; i <= 10; i++)
		i_i_map.insert(i, i);
	i_i_map[5] = 50;
	i_i_map.erase(7);
	cout << i_i_map.size() << "\n";
	for (auto i = i_i_map.getFirst(); i != i_i_map.getEnd(); i++)
		cout << i.getKey()<< " " << i << "\n";
}
void test2() {
	RedBlackTree<string, int> s_i_map;
	string s[] = { "TES","T is", "run","nin", "g. ", "See Re","sult ","of t","he t","est."};
	int delete_order[] = { 5,2,3,7,9,6,1,0,4,8 };
	for (int i = 0; i < 10; i++) {
		s_i_map.insert(s[i], i);
	}
	while (!s_i_map.isEmpty()) {
		cout << "\n";
		cout << s_i_map.size() << "\n";
		for (auto i = s_i_map.getBegin(); i != s_i_map.getEnd(); i++) {
			cout << i.getKey() << " " << i << "\n";
		}
		s_i_map.erase(s[delete_order[10-s_i_map.size()]]);
	}
}