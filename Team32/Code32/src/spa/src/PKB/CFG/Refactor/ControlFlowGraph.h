#ifndef SPA_CONTROLFLOWGRAPH_H
#define SPA_CONTROLFLOWGRAPH_H

#include <unordered_map>

#include "NodeInterface.h"
#include "PKB/PKB.h"

class PKB::ControlFlowGraph {
public:
	void createNode(shared_ptr<StmtInfo> stmt_info);
	shared_ptr<PKB::NodeInterface> getNode(StmtRef ref);
	void setNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second);
	void removeNext(shared_ptr<PKB::NodeInterface> first, shared_ptr<PKB::NodeInterface> second);
	vector<shared_ptr<PKB::NodeInterface>> getPrevious(StmtRef ref);
	vector<shared_ptr<PKB::NodeInterface>> getNext(StmtRef ref);
	vector<shared_ptr<PKB::NodeInterface>> getPreviousTransitive(StmtRef ref);
	vector<shared_ptr<PKB::NodeInterface>> getNextTransitive(StmtRef ref);

	void optimize();

private:
	void createDummyNode(shared_ptr<PKB::IfNode> if_node);
	//	void setupDummyNodes();
	//	void reconstructIfConnections();
	void populateUniqueIndex();
	unordered_map<StmtRef, shared_ptr<PKB::NodeInterface>> stmt_to_normal_node_store;
	unordered_map<StmtRef, shared_ptr<PKB::NodeInterface>> stmt_to_dummy_node_store;
};

#endif  // SPA_CONTROLFLOWGRAPH_H
