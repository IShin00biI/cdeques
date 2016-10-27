#include <iostream>
#include <deque>
#include <unordered_map>
#include <string>
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

static const std::string& DSTART() {
	static const std::string value = "starting";
	return value;
}

static const std::string& DEXIT() {
	static const std::string value = "exiting";
	return value;
}

static const std::string& EMPTYDEQUE_NAME() {
	static const std::string value = "ConstEmpty";
	return value;
}

static const unsigned long& max_u_long() {
	static const unsigned long max_u_long = std::numeric_limits<unsigned long>::max();
	return max_u_long;
}

static deque_map& all_deques() {
	static deque_map all_deques;
	return all_deques;
}

static std::string string_id(unsigned long& id) {
	return (id == emptystrdeque() ? EMPTYDEQUE_NAME() : "Deque " + std::to_string(id));
}

// Funkcja wypisująca komunikaty diagnostyczne
static void print_debug(const std::string& func_name, const std::string& args, const std::string& comment) {
	if(DEBUG) {
		std::ios_base::Init();
		std::cerr << func_name << "(" << args << "): " << comment << std::endl;
	}
}

// Sprawdzanie czy kolejka o danym id istnieje
static bool strdeque_exists(const std::string& func_name, const std::string& args, unsigned long& id) {
	deque_map::iterator found = all_deques().find(id);

	if(found == all_deques().end()) {
		print_debug(func_name, args, string_id(id) + " does not exist");
		return false;
	}
	print_debug(func_name, args, string_id(id) + " exists");
	return true;
}

// Zwraca true jeśli deque o identyfikatorze id jest pustą kolejką, false wpp. Ponadto dla pustej kolejki wypisywany jest komunikat diagnostyczny.
static bool strdeque_is_const_empty(const std::string& func_name, const std::string& args, unsigned long& id) {
	if(id == emptystrdeque()) {
		print_debug(func_name, args, "Attempting " + func_name + "on " + EMPTYDEQUE_NAME());
		return true;
	}
	return false;
}

// Tworzy nową, pustą kolejkę dwustronną ciągów znaków i zwraca jej identyfikator.
extern unsigned long strdeque_new() {
	static const std::string func_name = "strdeque_new";
	static const std::string args = "";
	print_debug(func_name, args, DSTART());
	//all_deques();
	static unsigned long next_id = 0;
	assert(next_id < max_u_long());
	//assert(!strdeque_exists(next_id)); //TODO: CO gdy nie istnieje jeszcze EMptyDEq?
	all_deques().insert(std::pair<unsigned long, strdeque>(next_id, strdeque()));
	unsigned long current_id = next_id++;

	print_debug(func_name, args, "created with id = " + std::to_string(current_id));

	print_debug(func_name, args, DEXIT());
	return current_id;
}

// Jeżeli istnieje kolejka dwustronna o identyfikatorze id, usuwa ją, a w przeciwnym przypadku nic nie robi.
extern void strdeque_delete(unsigned long id) {
	static const std::string func_name = "strdeque_delete";
	const std::string args = string_id(id);
	print_debug(func_name, args, DSTART());

	if(!strdeque_is_const_empty(func_name, args, id) && strdeque_exists(func_name, args, id)) {
		deque_map::iterator found = all_deques().find(id);
		all_deques().erase(found);
		print_debug(func_name, args, string_id(id) + " deleted");
	}
	print_debug(func_name, args, DEXIT());
}

// Jeżeli istnieje kolejka dwustronna o identyfikatorze id, zwraca liczbę jej elementów, a w przeciwnym przypadku zwraca 0.
extern size_t strdeque_size(unsigned long id) {
	static const std::string func_name = "strdeque_size";
	const std::string args = string_id(id);
	print_debug(func_name, args, DSTART());
	size_t result;
	if(!strdeque_exists(func_name, args, id)) result = 0;
	else {
		result = all_deques()[id].size();

		print_debug(func_name, args, string_id(id) + " contains " + std::to_string(result) + " elements");

		print_debug(func_name, args, DEXIT());
	}
	return result;
}

// Jeżeli istnieje kolejka dwustronna o identyfikatorze id oraz value != NULL, wstawia element value przed pozycją pos lub na koniec kolejki, jeżeli pos  wykracza poza kolejkę. Gwarancje odnośnie kosztów wstawienia na początku, na końcu i w środku mają być takie same jak w przypadku kontenera deque (plus koszt odnalezienia kolejki o danym identyfikatorze). Jeżeli kolejka dwustronna nie istnieje lub value == NULL, to nic nie robi. Pozycje w kolejce numerowane są od zera.
extern void strdeque_insert_at(unsigned long id, size_t pos, const char* value) {
	std::string string_value = value == NULL ? "NULL" : std::string(value);

	static const std::string func_name = "strdeque_insert_at";
	const std::string args = string_id(id) + ", " + std::to_string(pos) + ", \"" + string_value + "\"";
	print_debug(func_name, args, DSTART());

	if(value == NULL) {
		print_debug(func_name, args, "attempt to insert NULL");
		return;
	}
	if(!strdeque_is_const_empty(func_name, args, id) && strdeque_exists(func_name, args, id)) {
		size_t deque_size = all_deques()[id].size();
		if (pos > deque_size) pos = deque_size;
		if (pos == deque_size) all_deques()[id].push_back(string_value);
		else if (pos == 0) all_deques()[id].push_front(string_value);
		else all_deques()[id].insert(all_deques()[id].begin() + pos, string_value);

		print_debug(func_name, args,
								string_id(id) + ", element \"" + string_value + "\" inserted at " + std::to_string(pos));
	}
	print_debug(func_name, args, DEXIT());
}

// Jeżeli istnieje kolejka dwustronna o identyfikatorze id i pos nie wykracza poza nią, usuwa element na pozycji pos, a w przeciwnym przypadku nic nie robi.
extern void strdeque_remove_at(unsigned long id, size_t pos) {
	static const std::string func_name = "strdeque_remove_at";
	const std::string args = string_id(id) + ", " + std::to_string(pos);
	print_debug(func_name, args, DSTART());

	std::string string_id_local = string_id(id);
	if(!strdeque_is_const_empty(func_name, args, id) && strdeque_exists(func_name, args, id)) {
		size_t deque_size = all_deques()[id].size();
		if (deque_size <= pos) {
			print_debug(func_name, args, string_id_local + " does not contain an element at " + std::to_string(pos));
			return;
		}
		if (pos == deque_size - 1) all_deques()[id].pop_back();
		else if (pos == 0) all_deques()[id].pop_front();
		else all_deques()[id].erase(all_deques()[id].begin() + pos);
		print_debug(func_name, args, string_id(id) + ", removed element at " + std::to_string(pos));
	}
	print_debug(func_name, args, DEXIT());
}

// Jeżeli istnieje kolejka dwustronna o identyfikatorze id i pos nie wykraczapoza kolejkę, zwraca wskaźnik do elementu na pozycji pos, a w przeciwnym przypadku zwraca NULL.
extern const char* strdeque_get_at(unsigned long id, size_t pos) {
	std::string string_id_local = string_id(id);

	static const std::string func_name = "strdeque_get_at";
	const std::string args = string_id_local + ", " + std::to_string(pos);
	print_debug(func_name, args, DSTART());
	const char* char_value;
	if(!strdeque_exists(func_name, args, id)) char_value = NULL;
	else {
		size_t deque_size = all_deques()[id].size();
		if (pos >= deque_size) {
			print_debug(func_name, args, string_id_local + " does not contain an element at " + std::to_string(pos));
			return NULL;
		}
		std::string string_value = *(all_deques()[id].begin() + pos);
		char_value = string_value.c_str();
		assert(char_value != NULL);

		print_debug(func_name, args,
								string_id_local + ", element at " + std::to_string(pos) + " is \"" + string_value + "\"");
	}
	print_debug(func_name, args, DEXIT());
	return char_value;
}

// Jeżeli istnieje kolejka dwustronna o identyfikatorze id, usuwa wszystkie jej elementy, a w przeciwnym przypadki nic nie robi.
extern void strdeque_clear(unsigned long id) {
	std::string string_id_local = string_id(id);

	static const std::string func_name = "strdeque_clear";
	const std::string args = string_id_local;
	print_debug(func_name, args, DSTART());

	if(!strdeque_is_const_empty(func_name, args, id) && strdeque_exists(func_name, args, id)) {
		all_deques()[id].clear();
		print_debug(func_name, args, string_id_local + " cleared");
	}
	print_debug(func_name, args, DEXIT());
}

// Porównuje leksykograficznie kolejki dwustronne o identyfikatorach id1 i id2, zwracając
//      -1, gdy kolejka(id1) < kolejka(id2),
//       0, gdy kolejka(id1) = kolejka(id2),
//       1, gdy kolejka(id1) > kolejka(id2).
// Jeżeli kolejka dwustronna o którymś z identyfikatorów nie istnieje, to jest traktowana jako leksykograficznie równa liście pustej.
extern int strdeque_comp(unsigned long id1, unsigned long id2) {
	static const std::string func_name = "strdeque_comp";
	std::string string_id1 = string_id(id1);
	std::string string_id2 = string_id(id2);
	const std::string args = string_id1 + ", " + string_id2;
	print_debug(func_name, args, DSTART());

	if(!strdeque_exists(func_name, args, id1)) id1 = emptystrdeque();
	if(!strdeque_exists(func_name, args, id2)) id2 = emptystrdeque();

	strdeque content1 = all_deques()[id1];
	strdeque content2 = all_deques()[id2];
	int result = content1 < content2 ? -1 : (content1 > content2 ? 1 : 0);
	print_debug(func_name, args, "result of comparing " + string_id1 + " to " + string_id2 + " is " + std::to_string(result));
	print_debug(func_name, args, DEXIT());
	return result;
}
