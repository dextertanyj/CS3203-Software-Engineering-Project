#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/WithExecutor.tpp"
#include "catch.hpp"
#include "QP/QueryExpressionLexer.h"
#include "QP/StorageAdapter.h"
#include "QP/Executor/AttributeExecutor.h"
#include <iostream>

using namespace QP::Types;
using namespace QP::Executor;
using namespace Common::ExpressionProcessor;

TEST_CASE("WithExecutor::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Print);
	pkb.setUses(1, "x");
	pkb.setStmtType(2, StmtType::Call);
	pkb.setCall(2, "mars");
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setWhileControl(3, "x");
	pkb.setConstant(10);
	pkb.setUses(3, "x");
	pkb.setStmtType(4, StmtType::Read);
	pkb.setModifies(4, "x");
	pkb.setProc("main", 1, 4);
	pkb.setStmtType(5, StmtType::IfStmt);
	pkb.setIfControl(5, "y");
	pkb.setUses(5, "y");
	pkb.setConstant({1, 3});
	vector<string> assign_token1 = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = Expression::parse(lexer1, ExpressionType::Arithmetic);
	pkb.setStmtType(6, StmtType::Assign);
	pkb.setAssign(6, "y", expression1);
	pkb.setModifies(6, "a");
	pkb.setUses(6, "x");
	pkb.setStmtType(7, StmtType::Print);
	pkb.setUses(7, "a");
	pkb.setProc("mars", 5, 7);

	ReferenceArgument proc = ReferenceArgument(Attribute{AttributeType::Name, Declaration{DesignEntity::Procedure, "p"}});
	WithInternalExecutors<Name, Name> proc_internal_executor({AttributeExecutor::selectProcedures, AttributeExecutor::identity<Name>});
	ReferenceArgument call = ReferenceArgument(Attribute{AttributeType::Variable, Declaration{DesignEntity::Call, "c"}});
	WithInternalExecutors<Name, Number> call_internal_executor({AttributeExecutor::selectStatements, AttributeExecutor::callToProcedure});
	ReferenceArgument variable = ReferenceArgument(Attribute{AttributeType::Name, Declaration{DesignEntity::Variable, "v"}});
	WithInternalExecutors<Name, Name> variable_internal_executor({AttributeExecutor::selectVariables, AttributeExecutor::identity<Name>});
	ReferenceArgument read = ReferenceArgument(Attribute{AttributeType::Name, Declaration{DesignEntity::Read, "r"}});
	WithInternalExecutors<Name, Number> read_internal_executor({AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::ModifiesS>});
	ReferenceArgument print = ReferenceArgument(Attribute{AttributeType::Name, Declaration{DesignEntity::Print, "pr"}});
	WithInternalExecutors<Name, Number> print_internal_executor({AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::UsesS>});
	ReferenceArgument constant = ReferenceArgument(Attribute{AttributeType::Value, Declaration{DesignEntity::Constant, "const"}});
	WithInternalExecutors<Number, Number> constant_internal_executor({AttributeExecutor::selectConstants, AttributeExecutor::identity<Number>});
	ReferenceArgument stmt = ReferenceArgument(Attribute{AttributeType::Index, Declaration{DesignEntity::Stmt, "s"}});
	WithInternalExecutors<Number, Number> stmt_internal_executor({AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>});
	ReferenceArgument if_stmt = ReferenceArgument(Attribute{AttributeType::Index, Declaration{DesignEntity::If, "if"}});
	WithInternalExecutors<Number, Number> if_stmt_internal_executor({AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>});

	ReferenceArgument main = ReferenceArgument("main");
	ReferenceArgument mars = ReferenceArgument("mars");
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument index_5 = ReferenceArgument(5);
	ReferenceArgument index_6 = ReferenceArgument(6);
	WithInternalExecutors<Name, Name> name_internal_executor({AttributeExecutor::extractName, AttributeExecutor::identity<Name>});
	WithInternalExecutors<Number, Number> index_internal_executor({AttributeExecutor::extractNumber, AttributeExecutor::identity<Number>});

	SECTION("Trivial: Attribute & Attribute") {
		QP::QueryResult result1 = WithExecutor::executeTrivialAttributeAttribute(store, proc, call, proc_internal_executor, call_internal_executor);
		QP::QueryResult result2 = WithExecutor::executeTrivialAttributeAttribute(store, read, print, read_internal_executor, print_internal_executor);
		QP::QueryResult result3 = WithExecutor::executeTrivialAttributeAttribute(store, stmt, constant, stmt_internal_executor, constant_internal_executor);
		QP::QueryResult result4 = WithExecutor::executeTrivialAttributeAttribute(store, proc, read, proc_internal_executor, read_internal_executor);
		QP::QueryResult result5 = WithExecutor::executeTrivialAttributeAttribute(store, if_stmt, constant, if_stmt_internal_executor, constant_internal_executor);
		QP::QueryResult result6 = WithExecutor::executeTrivialAttributeAttribute(store, proc, proc, proc_internal_executor, proc_internal_executor);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
		REQUIRE(result6.getResult());
	}

	SECTION("Trivial: Attribute & Constant") {
		QP::QueryResult result1 = WithExecutor::executeTrivialAttributeConstant(store, proc, main, proc_internal_executor, name_internal_executor);
		QP::QueryResult result2 = WithExecutor::executeTrivialAttributeConstant(store, read, x, read_internal_executor, name_internal_executor);
		QP::QueryResult result3 = WithExecutor::executeTrivialAttributeConstant(store, if_stmt, index_5, if_stmt_internal_executor, index_internal_executor);
		QP::QueryResult result4 = WithExecutor::executeTrivialAttributeConstant(store, read, y, read_internal_executor, name_internal_executor);
		QP::QueryResult result5 = WithExecutor::executeTrivialAttributeConstant(store, call, main, call_internal_executor, name_internal_executor);
		QP::QueryResult result6 = WithExecutor::executeTrivialAttributeConstant(store, if_stmt, index_6, if_stmt_internal_executor, index_internal_executor);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
		REQUIRE_FALSE(result6.getResult());
	}


	SECTION("Trivial: Constant & Attribute") {
		QP::QueryResult result1 = WithExecutor::executeTrivialConstantAttribute(store, main, proc, name_internal_executor, proc_internal_executor);
		QP::QueryResult result2 = WithExecutor::executeTrivialConstantAttribute(store, x, read, name_internal_executor, read_internal_executor);
		QP::QueryResult result3 = WithExecutor::executeTrivialConstantAttribute(store, index_5, if_stmt, index_internal_executor, if_stmt_internal_executor);
		QP::QueryResult result4 = WithExecutor::executeTrivialConstantAttribute(store, y, read, name_internal_executor, read_internal_executor);
		QP::QueryResult result5 = WithExecutor::executeTrivialConstantAttribute(store, main, call, name_internal_executor, call_internal_executor);
		QP::QueryResult result6 = WithExecutor::executeTrivialConstantAttribute(store, index_6, if_stmt, index_internal_executor, if_stmt_internal_executor);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
		REQUIRE_FALSE(result6.getResult());
	}

	SECTION("Trivial: Constant & Constant") {
		QP::QueryResult result1 = WithExecutor::executeTrivialConstantConstant(store, main, main, name_internal_executor, name_internal_executor);
		QP::QueryResult result2 = WithExecutor::executeTrivialConstantConstant(store, index_5, index_5, index_internal_executor, index_internal_executor);
		QP::QueryResult result3 = WithExecutor::executeTrivialConstantConstant(store, main, mars, name_internal_executor, name_internal_executor);
		QP::QueryResult result4 = WithExecutor::executeTrivialConstantConstant(store, index_5, index_6, index_internal_executor, index_internal_executor);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
	}

	SECTION("Attribute & Attribute") {
		QP::QueryResult result1 = WithExecutor::executeAttributeAttribute(store, proc, call, proc_internal_executor, call_internal_executor);
		QP::QueryResult result2 = WithExecutor::executeAttributeAttribute(store, read, print, read_internal_executor, print_internal_executor);
		QP::QueryResult result3 = WithExecutor::executeAttributeAttribute(store, stmt, constant, stmt_internal_executor, constant_internal_executor);
		QP::QueryResult result4 = WithExecutor::executeAttributeAttribute(store, proc, read, proc_internal_executor, read_internal_executor);
		QP::QueryResult result5 = WithExecutor::executeAttributeAttribute(store, if_stmt, constant, if_stmt_internal_executor, constant_internal_executor);
		QP::QueryResult result6 = WithExecutor::executeAttributeAttribute(store, proc, proc, proc_internal_executor, proc_internal_executor);

		vector<string> expected_result_1 = {"mars"};
		vector<string> expected_result_2_r = {"4"};
		vector<string> expected_result_2_p = {"1"};
		vector<string> expected_result_3 = {"1", "3"};
		vector<string> expected_result_6 = {"main", "mars"};

		REQUIRE(result1.getSynonymResult("p").size() == 1);
		REQUIRE(result1.getSynonymResult("p") == expected_result_1);
		REQUIRE(result2.getSynonymResult("r").size() == 1);
		REQUIRE(result2.getSynonymResult("r") == expected_result_2_r);
		REQUIRE(result2.getSynonymResult("pr").size() == 1);
		REQUIRE(result2.getSynonymResult("pr") == expected_result_2_p);
		vector<string> actual_result_3_const = result3.getSynonymResult("const");
		vector<string> actual_result_3_stmt = result3.getSynonymResult("s");
		sort(actual_result_3_const.begin(), actual_result_3_const.end());
		sort(actual_result_3_stmt.begin(), actual_result_3_stmt.end());
		REQUIRE(result3.getSynonymResult("const").size() == 2);
		REQUIRE(actual_result_3_const == expected_result_3);
		REQUIRE(result3.getSynonymResult("s").size() == 2);
		REQUIRE(actual_result_3_stmt == expected_result_3);
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
		vector<string> actual_result_6 = result6.getSynonymResult("p");
		sort(actual_result_6.begin(), actual_result_6.end());
		REQUIRE(actual_result_6 == expected_result_6);
	}
}