#ifndef UNIT_TESTING_SRC_PKB_MOCKUTILITIES_H
#define UNIT_TESTING_SRC_PKB_MOCKUTILITIES_H

#include <memory>
#include <string>
#include <unordered_set>

using namespace std;

class MockInfo {
public:
	MockInfo(string ident) { this->ident = ident; };
	string getIdentifier() { return ident; };

	string ident;
};

class MockInfoStore {
public:
	MockInfoStore() = default;
	void insert(shared_ptr<MockInfo> info) { infos.insert(info); };
	unordered_set<shared_ptr<MockInfo>> getAll() const { return infos; };
	unordered_set<shared_ptr<MockInfo>> infos;
};

class MockRelation {
public:
	explicit MockRelation(shared_ptr<MockInfo> self) { this->self = move(self); };
	void insertForward(const shared_ptr<MockInfo>& front) { this->fronts.insert(front); };
	void insertReverse(const shared_ptr<MockInfo>& back) { this->backs.insert(back); };
	void appendForwardTransitive(unordered_set<shared_ptr<MockInfo>> new_front) {
		this->fronts_transitive.insert(new_front.begin(), new_front.end());
	};
	void appendReverseTransitive(unordered_set<shared_ptr<MockInfo>> new_back) {
		this->backs_transitive.insert(new_back.begin(), new_back.end());
	};

	[[nodiscard]] shared_ptr<MockInfo> getSelf() const { return this->self; };
	[[nodiscard]] unordered_set<shared_ptr<MockInfo>> getForward() const { return this->fronts; };
	[[nodiscard]] unordered_set<shared_ptr<MockInfo>> getReverse() const { return this->backs; };
	[[nodiscard]] unordered_set<shared_ptr<MockInfo>> getForwardTransitive() const { return this->fronts_transitive; };
	[[nodiscard]] unordered_set<shared_ptr<MockInfo>> getReverseTransitive() const { return this->backs_transitive; };

	shared_ptr<MockInfo> self;
	unordered_set<shared_ptr<MockInfo>> fronts;
	unordered_set<shared_ptr<MockInfo>> backs;
	unordered_set<shared_ptr<MockInfo>> fronts_transitive;
	unordered_set<shared_ptr<MockInfo>> backs_transitive;
};

#endif  // UNIT_TESTING_SRC_PKB_MOCKUTILITIES_H
