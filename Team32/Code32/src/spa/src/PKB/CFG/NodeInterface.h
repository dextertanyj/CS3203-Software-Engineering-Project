#ifndef SPA_SRC_PKB_CFG_NODEINTERFACE_H
#define SPA_SRC_PKB_CFG_NODEINTERFACE_H

#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "PKB/PKB.h"
#include "PKB/Types.h"

class PKB::NodeInterface {
public:
	explicit NodeInterface(Types::NodeType type);
	void setGraphIndex(size_t index);
	virtual void setConnection(std::shared_ptr<NodeInterface> next) = 0;
	virtual void setPrevious(const std::shared_ptr<NodeInterface>& prev) = 0;
	virtual void setNext(const std::shared_ptr<NodeInterface>& next) = 0;

	[[nodiscard]] std::unordered_set<std::shared_ptr<NodeInterface>> getPreviousNodes() const;
	[[nodiscard]] std::unordered_set<std::shared_ptr<NodeInterface>> getNextNodes() const;
	[[nodiscard]] size_t getGraphIndex() const;
	[[nodiscard]] Types::NodeType getNodeType() const;
	[[nodiscard]] virtual size_t getNodeRef() const = 0;

	virtual ~NodeInterface() = default;

protected:
	void insertNext(const std::shared_ptr<NodeInterface>& next);
	void insertPrevious(const std::shared_ptr<NodeInterface>& previous);

private:
	size_t graph_index = 0;  // Default to 0 for convenience when populating graph index later.
	Types::NodeType node_type;
	std::vector<std::weak_ptr<NodeInterface>> previous_nodes;
	std::vector<std::weak_ptr<NodeInterface>> next_nodes;
};

#endif  // SPA_SRC_PKB_CFG_NODEINTERFACE_H
