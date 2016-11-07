g++ -c -Wall -O2 -std=c++14 def_strdeque_test.cc -o def_strdeque_test.o
g++ def_strdeque_test.o strdeque.o strdequeconst.o -o def_strdeque_test

