#include "Bank.h"
#include <algorithm> //lower_bound

using namespace std;

template <typename KEY_TYPE, typename RESOURCE_TYPE>
Bank::Bank(RESOURCE_TYPE initial_resource) : resource{initial_resource} {}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
~Bank::Bank() {
	for (Loaner* entry : tracker) {
		delete entry;
	}
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::add_resource(RESOURCE_TYPE amount) {
	resource += amount;
	return true;
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::remove_resource(RESOURCE_TYPE amount) {
	if (resource < amount) {
		return false;
	}
	resource -= amount;
	return true;
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
void Bank::add(KEY_TYPE id, RESOURCE_TYPE max_needed);

template <typename KEY_TYPE, typename RESOURCE_TYPE>
void Bank::add(KEY_TYPE id, RESOURCE_TYPE max_needed, RESOURCE_TYPE already_give);

template <typename KEY_TYPE, typename RESOURCE_TYPE>
void Bank::erase(KEY_TYPE id);

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::loan(KEY_TYPE id, RESOURCE_TYPE amount);


template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::check_status();
