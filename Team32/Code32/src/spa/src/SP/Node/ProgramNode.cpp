#include "SP/Node/ProgramNode.h"

using namespace std;

SP::Node::ProgramNode::ProgramNode() = default;

void SP::Node::ProgramNode::addProcedureNode(unique_ptr<ProcedureNode> procedure) { procedures.push_back(move(procedure)); }

unique_ptr<SP::Node::ProgramNode> SP::Node::ProgramNode::parseProgram(Lexer& lex, StmtRef& statement_count) {
	unique_ptr<ProgramNode> program = make_unique<ProgramNode>();
	do {
		program->addProcedureNode(ProcedureNode::parseProcedure(lex, statement_count));
	} while (!lex.peekToken().empty());
	return program;
}
void SP::Node::ProgramNode::extract(PKB& pkb) {
	for (auto iter = procedures.begin(); iter < procedures.end(); ++iter) {
		iter->get()->extract(pkb);
	}
}

bool SP::Node::ProgramNode::equals(const shared_ptr<ProgramNode>& object) {
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

vector<shared_ptr<SP::Node::ProcedureNode>> SP::Node::ProgramNode::getProcedures() { return this->procedures; }
