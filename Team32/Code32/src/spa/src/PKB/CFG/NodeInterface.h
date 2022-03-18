#ifndef SPA_NODEINTERFACE_H
#define SPA_NODEINTERFACE_H

#include <algorithm>
#include <set>

#include "PKB/PKB.h"

class PKB::NodeInterface {
public:
	explicit NodeInterface(NodeType type);
	void setGraphIndex(size_t index);
	virtual void setConnection(shared_ptr<NodeInterface> next) = 0;
	virtual void setPrevious(shared_ptr<NodeInterface> prev) = 0;
	virtual void setNext(shared_ptr<NodeInterface> next) = 0;

	[[nodiscard]] set<shared_ptr<NodeInterface>> getPreviousNodes() const;
	[[nodiscard]] set<shared_ptr<NodeInterface>> getNextNodes() const;
	[[nodiscard]] size_t getGraphIndex() const;
	[[nodiscard]] NodeType getNodeType() const;

	virtual size_t getNodeRef() = 0;
	virtual ~NodeInterface() = default;

protected:
	void insertNext(shared_ptr<NodeInterface> next);
	void insertPrevious(shared_ptr<NodeInterface> previous);

private:
	size_t graph_index = 0;  // Default to 0 for convenience when populating graph index later.
	NodeType node_type;
	vector<weak_ptr<NodeInterface>> previous_nodes;
	vector<weak_ptr<NodeInterface>> next_nodes;
};

#endif  // SPA_NODEINTERFACE_H
