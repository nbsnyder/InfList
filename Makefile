InfList: InfList.h InfList_test.cpp
	clang++ -o InfList InfList_test.cpp -std=c++17 -O2 -Wall

inflist_c: inflist_c.h inflist_c_test.c
	clang -o inflist_c inflist_c_test.c -std=c89 -O2 -Wall