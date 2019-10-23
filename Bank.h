#ifndef BANK_H
#define BANK_H

#include <vector>
#include <unordered_map>

template <typename KEY_TYPE, typename RESOURCE_TYPE>
class Bank {
public:
	Bank(RESOURCE_TYPE initial_resource);

	//add or remove resources owned by the Bank
	//return false if resource cannot be added or removed
	bool add_resource(RESOURCE_TYPE amount);
	bool remove_resource(RESOURCE_TYPE amount);

	//add new loaner
	void add(KEY_TYPE id, RESOURCE_TYPE max_needed);
	void add(KEY_TYPE id, RESOURCE_TYPE max_needed, RESOURCE_TYPE already_give);

	//erase a loaner and retrieve their given resource
	void erase(KEY_TYPE id);

	//give out resource to a loaner
	//does nothing and returns false if amount exceeds loaner's limit
	bool loan(KEY_TYPE id, RESOURCE_TYPE amount);

	//returns true
	bool check_status();

private:
	struct Loaner {
		KEY_TPYE id;
		RESOURCE_TYPE may_need;
		RESOURCE_TYPE current_have;
	};

	//functor class for ordering Loaners
	class Tracker_Comp {
	public:
		bool operator()(const Loaner &lhs, const Loaner &rhs) {
			return lhs.may_need < rhs.may_need;
		}
	}

	//all Loaners sorted in ascending order of their may_need
	std::vector<Loaner> tracker;
	std::unordered_map<KEY_TYPE, size_t> index_tracker;
};





#endif