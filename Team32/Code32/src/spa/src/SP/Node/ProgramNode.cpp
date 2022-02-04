#include "SP/Node/ProgramNode.h"

#include <vector>

using namespace std;
using namespace SP;

ProgramNode::ProgramNode() = default;

void ProgramNode::addProcedureNode(unique_ptr<ProcedureNode> procedure) { procedures.push_back(move(procedure)); }

unique_ptr<ProgramNode> ProgramNode::parseProgram(Lexer& lex, int& statement_count) {
	unique_ptr<ProgramNode> program = make_unique<ProgramNode>();
	do {
		program->addProcedureNode(ProcedureNode::parseProcedure(lex, statement_count));
	} while (!lex.peekToken().empty());
	return program;
}
bool ProgramNode::extract(PKB& pkb) {
	for (auto iter = procedures.begin(); iter < procedures.end(); ++iter) {
		iter->get()->extract(pkb);
	}
	return true;
}

bool ProgramNode::equals(shared_ptr<ProgramNode> object) {
    if (this->procedures.size() != object->procedures.size()) {
        return false;
    }
    for (int i = 0; i < this->procedures.size(); i++) {
        if (!this->procedures[i]->equals(object->procedures[i])) {
            return false;
        }
    }
    return true;
}

vector<shared_ptr<ProcedureNode>> ProgramNode::getProcedures() {
    return this->procedures;
}
