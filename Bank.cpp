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
bool tracker_comp(vector<Loaner*>::iterator it, const Loaner* val) {
	return (*it)->may_need < val->may_need;
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::add(KEY_TYPE id, RESOURCE_TYPE max_needed) {
	return add(id, max_needed, 0);
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::add(KEY_TYPE id, RESOURCE_TYPE max_needed, RESOURCE_TYPE given) {
	//error checking
	if (index_tracker.find(id) != index_tracker.end()) {
		throw "Error in Bank::add : Duplicate id.";
	}

	if (given > resource) {
		return false;
	}
	resource -= given;

	Loaner* new_entry = new Loaner{id, max_needed-given, given};
	
	auto insert_it = lower_bound(track.begin(), tracker.end(), *new_entry, tracker_comp);
	tracker.insert(insert_it, new_entry); //insert to tracker
	index_tracker[(*insert)->id] = insert_it - tracker.begin(); //record index

	//update index tracker
	for (auto it = insert_it+1; it != tracker.end(); ++it) {
		index_tracker[(*it)->id] += 1; //update index
	}
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
void Bank::erase(KEY_TYPE id) {
	//error checking
	if (index_tracker.find(id) != index_tracker.end()) {
		throw "Error in Bank::erase : Nonexistent id.";
	}

	int ind = index_tracker[id]; //find index in tracker
	Loaner *target = tracker[ind];

	//return resource to bank
	resource += target->given;

	//update index tracker
	for (int i = ind+1; i < tracker.size(); ++i) {
		index_tracker[tracker[i]->id] -= 1;
	}
	//erase from index tracker
	index_tracker.erase(index_tracker.find(target->id));

	//erase from tracker
	tracker.erase(ind);

	//erase object
	delete target;

}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::loan(KEY_TYPE id, RESOURCE_TYPE amount) {
	//error checking
	if (index_tracker.find(id) != index_tracker.end()) {
		throw "Error in Bank::loan : Nonexistent id.";
	}

	if (amount == 0) {
		return check_status();
	}

	//get target loaner
	size_t ind = index_tracker[id];
	Loaner *target = tracker[ind];

	//create and add new entry
	Loaner *updated = new Loaner{target->id, target->may_need, target->given};
	auto insert_it = lower_bound(track.begin(), tracker.end(), *new_entry, tracker_comp);
	auto insert_ind = insert_it - tracker.begin();
	tracker.insert(insert_it, new_entry); //insert to tracker
	
	//if save to give loan
	if (check_status(true, ind)) {
		//deleted old entry
		tracker.erase(ind);

		//update index tracker
		for (auto i = ind; i < insert_ind; ++i) {
			index_tracker[tracker[i]->id] -= 1;
		}

		return true;
	}

	//if not save to give loan
	//delete new entry
	tracker.erase(insert_ind);
	return false;
}


template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::check_status() {
	check_status_impl(false, 0);
}

template <typename KEY_TYPE, typename RESOURCE_TYPE>
bool Bank::check_status_impl(bool ignore, size_t ignore_ind) {
	RESOURCE_TYPE current_resource = resource;

	for (size_t i = 0; i < tracker.size(); ++i) {
		if (ignore && i == ignore_ind) {
			continue;
		} else if (current_resource < tracker[i]->may_need) {
			return false;
		}
		current_resource += tracker[i]->given;
	}
	return true;
}