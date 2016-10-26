#include <iostream>
#include <deque>
#include <unordered_map>
// #include <string>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <limits>
#include "strdeque.h"
#include "strdequeconst.h"

#ifndef NDEBUG
const bool DEBUG = true;
#else
const bool DEBUG = false;
#endif

typedef std::deque<std::string> strdeque;
typedef std::unordered_map<unsigned long, strdeque> deque_map;

static const std::string DSTART = "Starting";
static const std::string DEXIT = "Exiting";
static const std::string EMPTYDEQ = "ConstEmpty";

static unsigned long max_u_long() {
	static const unsigned long max_u_long = std::numeric_limits<unsigned long>::max();
	return max_u_long;
}

static deque_map& all_deques() {
	static deque_map all_deques;
	return all_deques;
}

static std::string string_id(unsigned long& id) {
	return (id == emptystrdeque() ? EMPTYDEQ : std::to_string(id));
}

static void print_debug(const std::string& comment, const std::string& func_name, const std::string& args) {
	if(DEBUG) std::cerr << func_name << " | " << args << " | " << comment << std::endl;
}


static void print_deque_debug(strdeque de) {
	std::cerr << "Deque: ";
	for(strdeque::iterator it = de.begin(); it != de.end(); ++it) {
 		std::cerr << *it << " ";
	}
	std::cerr << std::endl;
}

static bool strdeque_exists(unsigned long& id) {
	static const std::string func_name = "strdeque_exists";
	static const std::string args = "";
	print_debug(DSTART, func_name, args);

	deque_map::iterator found = all_deques().find(id);

	if(found == all_deques().end()) {
		print_debug("Deque " + string_id(id) + " does not exist", func_name, args);
		return false;
	}
	print_debug("Deque " + string_id(id) + " exists", func_name, args);
	return true;
}

static bool strdeque_is_const_empty(const std::string& func_name, unsigned long& id) {
	if(id == emptystrdeque()) {
		print_debug("Attempting " + func_name + "on " + EMPTYDEQ, "strdeque_is_const_empty", "");
		return true;
	}
	return false;
}

extern unsigned long strdeque_new() {
	static const std::string func_name = "strdeque_new";
	static const std::string args = "";
	print_debug(DSTART, func_name, args);

	static unsigned long next_id = 0;
	assert(next_id < max_u_long());
	std::cout << "przed sprawdzeniem istnienia" << std::endl;
	// assert(!strdeque_exists(next_id)); //TODO: CO gdy nie istnieje jeszcze EMptyDEq?
	std::cout << "po sprawdzeniem istnienia, proba wlozenia" << std::endl;
	all_deques().insert(std::pair<unsigned long, strdeque>(next_id, strdeque()));
	unsigned long current_id = next_id++;

	print_debug(" created" + current_id, func_name, args);

	print_debug(DEXIT, func_name, args);
	return current_id;
}

extern void strdeque_delete(unsigned long id) {
	static const std::string func_name = "strdeque_delete";
	const std::string args = string_id(id);
	print_debug(DSTART, func_name, args);

	if(strdeque_is_const_empty(func_name, id) || !strdeque_exists(id)) return;
	deque_map::iterator found = all_deques().find(id);
	all_deques().erase(found);

	print_debug(DEXIT, func_name, args);
}

extern size_t strdeque_size(unsigned long id) {
	static const std::string func_name = "strdeque_size";
	const std::string args = string_id(id);
	print_debug(DSTART, func_name, args);

	if(!strdeque_exists(id)) return 0;
	deque_map::iterator found = all_deques().find(id);
	size_t result = found->second.size();

	print_debug(string_id(id) + " contains " + std::to_string(result) + " elements", func_name, args);

	print_debug(DEXIT, func_name, args);
	return result;
}

extern void strdeque_insert_at(unsigned long id, size_t pos, const char* value) {
	std::string string_value = value == NULL ? "NULL" : std::string(value);

	static const std::string func_name = "strdeque_insert_at";
	const std::string args = string_id(id) + ", " + std::to_string(pos) + ", " + string_value;
	print_debug(DSTART, func_name, args);

	if(value == NULL) {
		print_debug("attempt to insert NULL", func_name, args);
		return;
	}
	if(strdeque_is_const_empty(func_name, id) || !strdeque_exists(id)) return;
	size_t deque_size = all_deques()[id].size(); //TODO wszystko ponizej
	if(pos > deque_size) {
		pos = deque_size;
	}
	if(pos == deque_size) all_deques()[id].push_back(string_value);
	else if(pos == 0) all_deques()[id].push_front(string_value);
	else all_deques()[id].insert(all_deques()[id].begin() + pos, string_value);

	print_debug(string_id(id) + ", element " + string_value + " inserted at " + std::to_string(pos), func_name, args);

	print_debug(DEXIT, func_name, args);
}

extern void strdeque_remove_at(unsigned long id, size_t pos) {
	static const std::string func_name = "strdeque_remove_at";
	const std::string args = string_id(id) + ", " + std::to_string(pos);
	print_debug(DSTART, func_name, args);

	std::string string_id_local = string_id(id);
	if(strdeque_is_const_empty(func_name, id) || !strdeque_exists(id)) return;
	size_t deque_size = all_deques()[id].size();
	if(deque_size <= pos || pos < 0) {
		print_debug(string_id_local + " does not contain an element at " + std::to_string(pos), func_name, args);
		return;
	}
	if(pos == deque_size - 1) all_deques()[id].pop_back();
	else if(pos == 0) all_deques()[id].pop_front();
	else all_deques()[id].erase(all_deques()[id].begin() + pos);
	print_debug("removed", func_name, args);
	print_debug(DEXIT, func_name, args);
}

extern const char* strdeque_get_at(unsigned long id, size_t pos) {
	std::string string_id_local = string_id(id);

	static const std::string func_name = "strdeque_get_at";
	const std::string args = string_id_local + ", " + std::to_string(pos);
	print_debug(DSTART, func_name, args);

	if(!strdeque_exists(id));

	size_t deque_size = all_deques()[id].size();
	if(pos >= deque_size || pos < 0 ) {
		print_debug(string_id_local + " does not contain an element at " + std::to_string(pos), func_name, args);
		return NULL;
	}
	std::string string_value = *(all_deques()[id].begin() + pos);
	const char* char_value = string_value.c_str();
	assert(char_value != NULL);

	print_debug(string_id_local + ", element at " + std::to_string(pos) + " is " + string_value, func_name, args);
	print_debug(DEXIT, func_name, args);
	return char_value;
}

extern void strdeque_clear(unsigned long id) {
	std::string string_id_local = string_id(id);

	static const std::string func_name = "strdeque_clear";
	const std::string args = string_id_local;
	print_debug(DSTART, func_name, args);

	if(id == emptystrdeque()) {
		print_debug(string_id_local + " is empty", func_name, args);
		return;
	}
	deque_map::iterator found = all_deques().find(id);
	if(found == all_deques().end()) {
		print_debug(string_id_local + " does not exist", func_name, args);
		return;
	}
	found->second.clear();

	print_debug(string_id_local + " cleared", func_name, args);
	print_debug(DEXIT, func_name, args);
}

extern int strdeque_comp(unsigned long id1, unsigned long id2) {
	std::string string_id1 = string_id(id1);
	std::string string_id2 = string_id(id2);

	static const std::string func_name = "strdeque_comp";
	const std::string args = string_id1 + ", " + string_id2;
	print_debug(DSTART, func_name, args);

	deque_map::iterator foundIter1 = all_deques().find(id1);
	if(foundIter1 == all_deques().end()) {
		print_debug(string_id1 + " does not exist", func_name, args);
		foundIter1 = all_deques().find(emptystrdeque());
	}

	strdeque contentId1 = (*foundIter1).second;
	print_deque_debug(contentId1);

	deque_map::iterator foundIter2 = all_deques().find(id2);
	if(foundIter2 == all_deques().end()) {
		print_debug(string_id2 + " does not exist", func_name, args);
		foundIter2 = all_deques().find(emptystrdeque());
	}
	strdeque contentId2 = (*foundIter2).second;
	print_deque_debug(contentId2);

	print_debug(DEXIT, func_name, args);

	// return (int) std::lexicographical_compare<strdeque::iterator>(contentId2.begin(), contentId2.end(), contentId1.begin(), contentId1.end());
	return (contentId1 < contentId2) ? -1 : (contentId1 > contentId2) ? 1 : 0;
	//TODO: jako ostatni argument w lexicographical_compare można dać komparator, std::strcmp się wywala, więc trzeba sprawdzić czy nie trzeba zrobić komparatora stringów
}
