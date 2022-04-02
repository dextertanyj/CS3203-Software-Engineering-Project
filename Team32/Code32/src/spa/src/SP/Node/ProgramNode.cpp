#include "SP/Node/ProgramNode.h"

#include <utility>

using namespace std;

void SP::Node::ProgramNode::addProcedureNode(unique_ptr<ProcedureNode> procedure) { procedures.push_back(move(procedure)); }

void SP::Node::ProgramNode::extract(PKB::StorageUpdateInterface& pkb) const {
	for (auto iter = procedures.begin(); iter < procedures.end(); ++iter) {
		iter->get()->extract(pkb);
	}
}

bool SP::Node::ProgramNode::equals(const shared_ptr<ProgramNode>& object) const {
	if (this->procedures.size() != object->procedures.size()) {
		return false;
	}
	for (size_t i = 0; i < this->procedures.size(); i++) {
		if (!this->procedures[i]->equals(object->procedures[i])) {
			return false;
		}
	}
	return true;
}

vector<shared_ptr<SP::Node::ProcedureNode>> SP::Node::ProgramNode::getProcedures() const { return this->procedures; }

unique_ptr<SP::Node::ProgramNode> SP::Node::ProgramNode::parseProgram(Lexer& lex, StmtRef& statement_count) {
	unique_ptr<ProgramNode> program = make_unique<ProgramNode>();
	do {
		program->addProcedureNode(ProcedureNode::parseProcedure(lex, statement_count));
	} while (!lex.peekToken().empty());
	return program;
}
