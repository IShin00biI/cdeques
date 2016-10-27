#include <iostream>
#include "strdeque.h"
#include "strdequeconst.h"

#ifndef NDEBUG
const bool DEBUG = true;
#else
const bool DEBUG = false;
#endif

static unsigned long init_emptystrdeque() {
	if(DEBUG) std::cerr << "ConstEmpty initialisation: starting" << std::endl;
	unsigned long id = strdeque_new();
	if(DEBUG) std::cerr << "ConstEmpty initialisation: exiting with id = " << id << std::endl;
	return id;
}

// Zwraca identyfikator pustej kolejki dwustronnej, do której nic nie można wstawiać i z której nic nie można usuwać.
extern unsigned long emptystrdeque() {
	std::ios_base::Init();
	// if(DEBUG) std::cerr << "emptystrdeque() invoked" << std::endl;
	static const unsigned long id = init_emptystrdeque();
	if(DEBUG) std::cerr << "emptystrdeque() invoked and done with id = " << id  << std::endl;
	return id;
}
