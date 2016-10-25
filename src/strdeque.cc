#include <iostream>
#include <deque>
#include <unordered_map>
#include <string>
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

static unsigned long max_u_long() {
	static const unsigned long max_u_long = std::numeric_limits<unsigned long>::max();
	return max_u_long;
}

static deque_map& all_deques() {
	static deque_map all_deques;
	return all_deques;
}

static std::string string_id(unsigned long& id) {
	return (id == emptystrdeque() ? "ConstEmpty" : std::to_string(id));
}

/* static std::string string_id(unsigned long& id) {
	std::string bare = string_id_bare(id);
	return (id == emptystrdeque() ? bare : "deque " + bare);
} */

static void print_init(const std::string& func_name, const std::string& args) {
	if(DEBUG) std::cerr << "Starting " << func_name << args << " {" << std::endl;
	return;
}

static bool strdeque_exists(unsigned long& id) {
	deque_map::iterator found = all_deques().find(id);
	if(found == all_deques().end()) {
		if(DEBUG) std::cerr << "Deque " << string_id(id) << "exists" << std::endl;
		return false;
	}
	return true;
}

static bool strdeque_is_const_empty(const std::string& func_name, unsigned long& id) {
	if(id == emptystrdeque()) {
		if(DEBUG) std::cerr << "Attempting " << func_name << "on deque ConstEmpty" << std::endl;
		return true;
	}
	return false;
}

extern unsigned long strdeque_new() {
	static const std::string func_name = "strdeque_new()";
	print_init(func_name, "");
	static unsigned long next_id = 0;
	assert(next_id < max_u_long());
	assert(!strdeque_exists(next_id));
	deque_map all_deques = all_deques();
	all_deques.insert(std::pair<unsigned long, strdeque>(next_id, strdeque()));
	unsigned long current_id = next_id++;
// TODO	if(DEBUG) std::cerr << "strdeque_new: deque " << current_id << " created" << std::endl;
	return current_id;
}

extern void strdeque_delete(unsigned long id) {
	static const std::string func_name = "strdeque_delete";
	const std::string args = DEBUG ? "(" + string_id(id) + ")" : "";
	print_init(func_name, args);
	if(strdeque_is_const_empty(func_name, id) || !strdeque_exists(id)) return;
	deque_map::iterator found = all_deques().find(id);
	all_deques().erase(found);
}

extern size_t strdeque_size(unsigned long id) {
	static const std::string func_name = "strdeque_size";
	const std::string args = DEBUG ? "(" + string_id(id) + ")" : "";
	print_init(func_name, args);
	if(!strdeque_exists(id)) return 0;
	deque_map::iterator found = all_deques().find(id);
	size_t result = found->second.size();
	//TODO if(DEBUG) std::cerr << "strdeque_size: " << string_id << " contains " << result << " elements" << std::endl;
	return result;
}

extern void strdeque_insert_at(unsigned long id, size_t pos, const char* value) {
	std::string string_value = value == NULL ? "NULL" : std::string(value);
	static const std::string func_name = "strdeque_insert_at";
	const std::string args = DEBUG ? "(" + string_id(id) + ", " +
			std::to_string(pos) + ", " + string_value +")" : "";
	print_init(func_name, args);
	if(value == NULL) {
		if(DEBUG) std::cerr << "strdeque_insert_at: attempt to insert NULL into a deque" << std::endl;
		return;
	}
	if(strdeque_is_const_empty(func_name, id) || !strdeque_exists(id)) return;
	size_t deque_size = all_deques()[id].size(); //TODO wszystko ponizej
	if(pos >= deque_size) {
		pos = deque_size - 1;

	}
	if(pos == deque_size - 1) all_deques()[id].push_back(string_value);
	else if(pos == 0) all_deques()[id].push_front(string_value);
	else all_deques()[id].insert(all_deques()[id].begin() + pos, string_value);
	if(DEBUG) std::cerr << "strdeque_insert_at: " << string_id << ", element " << string_value << " inserted at " << pos << std::endl;
}

extern void strdeque_remove_at(unsigned long id, size_t pos) {
	if(DEBUG) {
		std::string string_id_bare = string_id_bare(id);
		std::string string_id = string_id(id);
		std::cerr << "strdeque_remove_at(" << string_id_bare << ", " << pos << ")" << std::endl;
	}
	if(id == emptystrdeque()) {
		if(DEBUG) std::cerr << "strdeque_remove_at: attempt to remove from the Empty Deque" << std::endl;
		return;
	}
	deque_map::iterator found = all_deques().find(id);
	if(found == all_deques().end()) {
		if(DEBUG) std::cerr << "strdeque_remove_at: " << string_id << " does not exist" << std::endl;
		return;
	}
	size_t deque_size = all_deques()[id].size();
	if(deque_size < pos) {
		if (DEBUG)
			std::cerr << "strdeque_remove_at: " << string_id << " does not contain an element at " << pos << std::endl;
		return;
	}
	if(pos == deque_size - 1) all_deques()[id].pop_back();
	else if(pos == 0) all_deques()[id].pop_front();
	else all_deques()[id].erase(all_deques()[id].begin() + pos);
	if(DEBUG) std::cerr << "strdeque_remove_at: " << string_id << ", element at " << pos << " removed" << std::endl;
}

extern const char* strdeque_get_at(unsigned long id, size_t pos) {
	if(DEBUG) {
		std::string string_id_bare = string_id_bare(id);
		std::string string_id = string_id(id);
		std::cerr << "strdeque_get_at(" << string_id_bare << ", " << pos << ")" << std::endl;
	}
	deque_map::iterator found = all_deques().find(id);
	if(found == all_deques().end()) {
		if(DEBUG) std::cerr << "strdeque_get_at: " << string_id << " does not exist" << std::endl;
		return NULL;
	}
	size_t deque_size = all_deques()[id].size();
	if(deque_size < pos) {
		if (DEBUG)
			std::cerr << "strdeque_get_at: " << string_id << " does not contain an element at " << pos << std::endl;
		return NULL;
	}
	std::string string_value = *(all_deques()[id].begin() + pos);
	const char* char_value = string_value.c_str();
	assert(char_value != NULL);
	if (DEBUG)
		std::cerr << "strdeque_get_at: " << string_id << ", element at " << pos << " is " << string_value << std::endl;
	return char_value;
}

extern void strdeque_clear(unsigned long id) {
	if(DEBUG) {
		std::string string_id_bare = string_id_bare(id);
		std::string string_id = string_id(id);
		std::cerr << "strdeque_clear(" << string_id_bare << ")" << std::endl;
	}
	if(id == emptystrdeque()) {
		if (DEBUG) std::cerr << "strdeque_clear: attempt to clear the Empty Deque" << std::endl;
		return;
	}
	deque_map::iterator found = all_deques().find(id);
	if(found == all_deques().end()) {
		if(DEBUG) std::cerr << "strdeque_clear: " << string_id << " does not exist" << std::endl;
		return;
	}
	found->second.clear();
	if(DEBUG) std::cerr << "strdeque_clear: " << string_id << " cleared" << std::endl;
}

extern void strdeque_comp(unsigned long id1, unsigned long id2) {
	if(DEBUG) {
		std::string string_id_bare1 = string_id_bare(id1);
		std::string string_id1 = string_id(id1);
		std::string string_id_bare2 = string_id_bare(id2);
		std::string string_id2 = string_id(id2);
		std::cerr << "strdeque_clear(" << string_id_bare1 << ", " << string_id_bare2 << ")" << std::endl;
	}
	deque_map::iterator found1 = all_deques().find(id1);
	if(found1 == all_deques().end()) {
		if(DEBUG) std::cerr << "strdeque_clear: " << string_id1 << " does not exist" << std::endl;
		found1 = all_deques().find(emptystrdeque());
	}
	deque_map::iterator found2 = all_deques().find(id2);
	if(found2 == all_deques().end()) {
		if(DEBUG) std::cerr << "strdeque_clear: " << string_id2 << " does not exist" << std::endl;
		found2 = all_deques().find(emptystrdeque());
	}
	//porownanie
}