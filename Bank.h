#ifndef BANK_H
#define BANK_H

#include <vector>
#include <unordered_map>

template <typename KEY_TYPE, typename RESOURCE_TYPE>
class Bank {
public:
	Bank(RESOURCE_TYPE initial_resource);

	~Bank();

	//disable copy constructor
	Bank(const Bank&) = delete;
	//disable copy assignment operator
    Bank& operator=(const Bank&) = delete;

	//add or remove resources owned by the Bank
	//return false if resource cannot be added or removed
	bool add_resource(RESOURCE_TYPE amount);
	bool remove_resource(RESOURCE_TYPE amount);

	//add new loaner
	bool add(KEY_TYPE id, RESOURCE_TYPE max_needed);
	bool add(KEY_TYPE id, RESOURCE_TYPE max_needed, RESOURCE_TYPE given);

	//erase a loaner and retrieve their given resource
	void erase(KEY_TYPE id);

	//give out resource to a loaner
	//does nothing and returns false if amount exceeds loaner's limit
	bool loan(KEY_TYPE id, RESOURCE_TYPE amount);

	//returns true if there exists a schedule
	//to satisfy all of current loaners
	bool check_status();

private:
	bool check_status_impl(bool ignore, size_t ignore_ind);

	struct Loaner {
		KEY_TPYE id;
		RESOURCE_TYPE may_need;
		RESOURCE_TYPE given;
	};

	//function used for ordering Loaners in tracker
	bool tracker_comp(vector<Loaner*>::iterator it, const Loaner* val);

	//all Loaners sorted in ascending order of their may_need
	std::vector<Loaner*> tracker;
	std::unordered_map<KEY_TYPE, size_t> index_tracker;

	RESOURCE_TYPE resource;
};





#endif