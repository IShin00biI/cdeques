#include <iostream>
#include "strdeque.h"
#include "strdequeconst.h"

#ifndef NDEBUG
const bool DEBUG = true;
#else
const bool DEBUG = false;
#endif

static unsigned long init_emptystrdeque() {
	if(DEBUG) std::cerr << "strdequeconst init invoked" << std::endl;
	unsigned long id = strdeque_new();
	if(DEBUG) std::cerr << "strdeque_const init finished" << std::endl;
	return id;
}

extern unsigned long emptystrdeque() {
	// if(DEBUG) std::cerr << "emptystrdeque() invoked" << std::endl;
	static const unsigned long id = init_emptystrdeque();
	// if(DEBUG) std::cerr << "emptystrdeque() done with id = " << id  << std::endl;
	return id;
}
