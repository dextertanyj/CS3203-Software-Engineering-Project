#include "QP/Executor/WithExecutor.tpp"

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/AttributeExecutor.tpp"
#include "QP/Preprocessor/QueryExpressionLexer.h"
#include "QP/StorageAdapter.h"
#include "catch.hpp"

using namespace std;
using namespace QP;
using namespace Preprocessor;
using namespace Executor;
using namespace Types;
using namespace Common::ExpressionProcessor;

TEST_CASE("WithExecutor::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Print);
	pkb.setStmtType(2, StmtType::Call);
	pkb.setStmtType(3, StmtType::While);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setUses(1, "x");
	pkb.setUses(3, "x");
	pkb.setModifies(4, "x");
	pkb.setConstant(10);
	pkb.setCall(2, "mars");
	pkb.setWhileControl(3, "x");
	pkb.setProc("main", 1, 4);

	pkb.setStmtType(5, StmtType::If);
	pkb.setStmtType(6, StmtType::Assign);
	pkb.setStmtType(7, StmtType::Print);
	pkb.setUses(5, "y");
	pkb.setUses(6, "x");
	pkb.setUses(7, "x");
	pkb.setModifies(6, "a");
	pkb.setConstant({1, 3});
	pkb.setIfControl(5, "y");
	vector<string> assign_token1 = {"x", "+", "1"};
	QueryExpressionLexer lexer1 = QueryExpressionLexer(assign_token1);
	auto expression1 = ExpressionParser(lexer1, ExpressionType::Arithmetic).parse();
	pkb.setAssign(6, "y", expression1);
	pkb.setProc("mars", 5, 7);

	ClauseArgument proc = ClauseArgument(Attribute{AttributeType::NameIdentifier, Declaration{DesignEntity::Procedure, "p"}});
	WithInternalExecutors<Name, Name> proc_internal_executor({AttributeExecutor::selectProcedures, AttributeExecutor::identity<Name>});
	ClauseArgument call = ClauseArgument(Attribute{AttributeType::ProcedureName, Declaration{DesignEntity::Call, "c"}});
	WithInternalExecutors<Name, Number> call_internal_executor({AttributeExecutor::selectStatements, AttributeExecutor::callToProcedure});
	ClauseArgument variable = ClauseArgument(Attribute{AttributeType::NameIdentifier, Declaration{DesignEntity::Variable, "v"}});
	WithInternalExecutors<Name, Name> variable_internal_executor({AttributeExecutor::selectVariables, AttributeExecutor::identity<Name>});
	ClauseArgument read = ClauseArgument(Attribute{AttributeType::VariableName, Declaration{DesignEntity::Read, "r"}});
	WithInternalExecutors<Name, Number> read_internal_executor(
		{AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<Types::ClauseType::ModifiesS>});
	ClauseArgument print = ClauseArgument(Attribute{AttributeType::VariableName, Declaration{DesignEntity::Print, "pr"}});
	WithInternalExecutors<Name, Number> print_internal_executor(
		{AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<Types::ClauseType::UsesS>});
	ClauseArgument constant =
		ClauseArgument(Attribute{AttributeType::NumberIdentifier, Declaration{DesignEntity::Constant, "const"}});
	WithInternalExecutors<Number, Number> constant_internal_executor(
		{AttributeExecutor::selectConstants, AttributeExecutor::identity<Number>});
	ClauseArgument stmt = ClauseArgument(Attribute{AttributeType::NumberIdentifier, Declaration{DesignEntity::Stmt, "s"}});
	WithInternalExecutors<Number, Number> stmt_internal_executor(
		{AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>});
	ClauseArgument if_stmt = ClauseArgument(Attribute{AttributeType::NumberIdentifier, Declaration{DesignEntity::If, "if"}});
	WithInternalExecutors<Number, Number> if_stmt_internal_executor(
		{AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>});
	ClauseArgument while_stmt = ClauseArgument(Attribute{AttributeType::NumberIdentifier, Declaration{DesignEntity::While, "while"}});
	WithInternalExecutors<Number, Number> while_stmt_internal_executor(
		{AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>});
	ClauseArgument assign_stmt = ClauseArgument(Attribute{AttributeType::NumberIdentifier, Declaration{DesignEntity::Assign, "a"}});
	WithInternalExecutors<Number, Number> assign_stmt_internal_executor(
		{AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>});

	ClauseArgument main = ClauseArgument("main");
	ClauseArgument mars = ClauseArgument("mars");
	ClauseArgument x = ClauseArgument("x");
	ClauseArgument y = ClauseArgument("y");
	ClauseArgument invalid_name = ClauseArgument("b");
	ClauseArgument index_3 = ClauseArgument(3);
	ClauseArgument index_5 = ClauseArgument(5);
	ClauseArgument index_6 = ClauseArgument(6);
	ClauseArgument invalid_index = ClauseArgument(11);
	WithInternalExecutors<Name, Name> name_internal_executor({AttributeExecutor::extractName, AttributeExecutor::identity<Name>});
	WithInternalExecutors<Number, Number> index_internal_executor({AttributeExecutor::extractNumber, AttributeExecutor::identity<Number>});

	SECTION("Trivial: Attribute & Attribute") {
		QueryResult result1 =
			WithExecutor::executeTrivialAttributeAttribute(store, proc, call, proc_internal_executor, call_internal_executor);
		REQUIRE(result1.getResult());
		QueryResult result2 =
			WithExecutor::executeTrivialAttributeAttribute(store, read, print, read_internal_executor, print_internal_executor);
		REQUIRE(result2.getResult());
		QueryResult result3 =
			WithExecutor::executeTrivialAttributeAttribute(store, stmt, constant, stmt_internal_executor, constant_internal_executor);
		REQUIRE(result3.getResult());
		QueryResult result4 =
			WithExecutor::executeTrivialAttributeAttribute(store, proc, proc, proc_internal_executor, proc_internal_executor);
		REQUIRE(result4.getResult());
		QueryResult result5 =
			WithExecutor::executeTrivialAttributeAttribute(store, proc, read, proc_internal_executor, read_internal_executor);
		REQUIRE_FALSE(result5.getResult());
		QueryResult result6 =
			WithExecutor::executeTrivialAttributeAttribute(store, if_stmt, constant, if_stmt_internal_executor, constant_internal_executor);
		REQUIRE_FALSE(result6.getResult());
	}

	SECTION("Trivial: Attribute & Constant") {
		QueryResult result1 =
			WithExecutor::executeTrivialAttributeConstant(store, proc, main, proc_internal_executor, name_internal_executor);
		REQUIRE(result1.getResult());
		QueryResult result2 =
			WithExecutor::executeTrivialAttributeConstant(store, read, x, read_internal_executor, name_internal_executor);
		REQUIRE(result2.getResult());
		QueryResult result3 =
			WithExecutor::executeTrivialAttributeConstant(store, if_stmt, index_5, if_stmt_internal_executor, index_internal_executor);
		REQUIRE(result3.getResult());
		QueryResult result4 =
			WithExecutor::executeTrivialAttributeConstant(store, read, y, read_internal_executor, name_internal_executor);
		REQUIRE_FALSE(result4.getResult());
		QueryResult result5 =
			WithExecutor::executeTrivialAttributeConstant(store, call, main, call_internal_executor, name_internal_executor);
		REQUIRE_FALSE(result5.getResult());
		QueryResult result6 = WithExecutor::executeTrivialAttributeConstant(store, if_stmt, invalid_index, if_stmt_internal_executor,
		                                                                        index_internal_executor);
		REQUIRE_FALSE(result6.getResult());
		QueryResult result7 =
			WithExecutor::executeTrivialAttributeConstant(store, stmt, invalid_index, if_stmt_internal_executor, index_internal_executor);
		REQUIRE_FALSE(result7.getResult());
	}

	SECTION("Trivial: Constant & Attribute") {
		QueryResult result1 =
			WithExecutor::executeTrivialConstantAttribute(store, main, proc, name_internal_executor, proc_internal_executor);
		REQUIRE(result1.getResult());
		QueryResult result2 =
			WithExecutor::executeTrivialConstantAttribute(store, x, read, name_internal_executor, read_internal_executor);
		REQUIRE(result2.getResult());
		QueryResult result3 =
			WithExecutor::executeTrivialConstantAttribute(store, index_5, if_stmt, index_internal_executor, if_stmt_internal_executor);
		REQUIRE(result3.getResult());
		QueryResult result4 =
			WithExecutor::executeTrivialConstantAttribute(store, y, read, name_internal_executor, read_internal_executor);
		REQUIRE_FALSE(result4.getResult());
		QueryResult result5 =
			WithExecutor::executeTrivialConstantAttribute(store, main, call, name_internal_executor, call_internal_executor);
		REQUIRE_FALSE(result5.getResult());
		QueryResult result6 = WithExecutor::executeTrivialConstantAttribute(store, invalid_index, if_stmt, index_internal_executor,
		                                                                        if_stmt_internal_executor);
		REQUIRE_FALSE(result6.getResult());
		QueryResult result7 =
			WithExecutor::executeTrivialConstantAttribute(store, invalid_index, stmt, index_internal_executor, if_stmt_internal_executor);
		REQUIRE_FALSE(result7.getResult());
	}

	SECTION("Trivial: Constant & Constant") {
		QueryResult result1 =
			WithExecutor::executeTrivialConstantConstant(store, main, main, name_internal_executor, name_internal_executor);
		REQUIRE(result1.getResult());
		QueryResult result2 =
			WithExecutor::executeTrivialConstantConstant(store, index_5, index_5, index_internal_executor, index_internal_executor);
		REQUIRE(result2.getResult());
		QueryResult result3 =
			WithExecutor::executeTrivialConstantConstant(store, main, mars, name_internal_executor, name_internal_executor);
		REQUIRE_FALSE(result3.getResult());
		QueryResult result4 =
			WithExecutor::executeTrivialConstantConstant(store, index_5, invalid_index, index_internal_executor, index_internal_executor);
		REQUIRE_FALSE(result4.getResult());
	}

	SECTION("Attribute & Attribute") {
		QueryResult result1 =
			WithExecutor::executeAttributeAttribute(store, proc, call, proc_internal_executor, call_internal_executor);
		vector<string> expected_result_1 = {"mars"};
		REQUIRE(result1.getSynonymResult("p") == expected_result_1);

		QueryResult result2 =
			WithExecutor::executeAttributeAttribute(store, read, print, read_internal_executor, print_internal_executor);
		vector<string> expected_result_2_r = {"4", "4"};
		vector<string> expected_result_2_p = {"1", "7"};
		REQUIRE(result2.getSynonymResult("r") == expected_result_2_r);
		vector<string> actual_result_2_print = result2.getSynonymResult("pr");
		sort(actual_result_2_print.begin(), actual_result_2_print.end());
		REQUIRE(actual_result_2_print == expected_result_2_p);

		QueryResult result3 =
			WithExecutor::executeAttributeAttribute(store, stmt, constant, stmt_internal_executor, constant_internal_executor);
		vector<string> expected_result_3 = {"1", "3"};
		vector<string> actual_result_3_const = result3.getSynonymResult("const");
		vector<string> actual_result_3_stmt = result3.getSynonymResult("s");
		sort(actual_result_3_const.begin(), actual_result_3_const.end());
		sort(actual_result_3_stmt.begin(), actual_result_3_stmt.end());
		REQUIRE(actual_result_3_const == expected_result_3);
		REQUIRE(actual_result_3_stmt == expected_result_3);

		QueryResult result4 =
			WithExecutor::executeAttributeAttribute(store, proc, proc, proc_internal_executor, proc_internal_executor);
		vector<string> expected_result_4 = {"main", "mars"};
		vector<string> actual_result_4 = result4.getSynonymResult("p");
		sort(actual_result_4.begin(), actual_result_4.end());
		REQUIRE(actual_result_4 == expected_result_4);

		QueryResult result5 =
			WithExecutor::executeAttributeAttribute(store, variable, print, variable_internal_executor, print_internal_executor);
		vector<string> expected_result_5 = {"x", "x"};
		vector<string> actual_result_5 = result5.getSynonymResult("v");
		sort(actual_result_5.begin(), actual_result_5.end());
		REQUIRE(actual_result_5 == expected_result_5);

		QueryResult result6 =
			WithExecutor::executeAttributeAttribute(store, proc, read, proc_internal_executor, read_internal_executor);
		REQUIRE_FALSE(result6.getResult());
		QueryResult result7 =
			WithExecutor::executeAttributeAttribute(store, if_stmt, constant, if_stmt_internal_executor, constant_internal_executor);
		REQUIRE_FALSE(result7.getResult());
		QueryResult result8 =
			WithExecutor::executeAttributeAttribute(store, variable, call, variable_internal_executor, call_internal_executor);
		REQUIRE_FALSE(result8.getResult());
	}

	SECTION("Attribute and Constant") {
		QueryResult result1 = WithExecutor::executeAttributeConstant(store, proc, mars, proc_internal_executor, name_internal_executor);
		vector<string> expected_result_1 = {"mars"};
		REQUIRE(result1.getSynonymResult("p") == expected_result_1);
		QueryResult result2 = WithExecutor::executeAttributeConstant(store, call, mars, call_internal_executor, name_internal_executor);
		vector<string> expected_result_2 = {"2"};
		REQUIRE(result2.getSynonymResult("c") == expected_result_2);
		QueryResult result3 =
			WithExecutor::executeAttributeConstant(store, while_stmt, index_3, while_stmt_internal_executor, index_internal_executor);
		vector<string> expected_result_3 = {"3"};
		REQUIRE(result3.getSynonymResult("while") == expected_result_3);
		QueryResult result4 =
			WithExecutor::executeAttributeConstant(store, variable, x, variable_internal_executor, name_internal_executor);
		vector<string> expected_result_4 = {"x"};
		REQUIRE(result4.getSynonymResult("v") == expected_result_4);

		QueryResult result5 = WithExecutor::executeAttributeConstant(store, print, x, print_internal_executor, name_internal_executor);
		vector<string> expected_result_5 = {"1", "7"};
		vector<string> actual_result_5 = result5.getSynonymResult("pr");
		sort(actual_result_5.begin(), actual_result_5.end());
		REQUIRE(actual_result_5 == expected_result_5);

		QueryResult result6 = WithExecutor::executeAttributeConstant(store, call, main, call_internal_executor, name_internal_executor);
		REQUIRE_FALSE(result6.getResult());
		QueryResult result7 =
			WithExecutor::executeAttributeConstant(store, while_stmt, index_5, while_stmt_internal_executor, index_internal_executor);
		REQUIRE_FALSE(result7.getResult());
		QueryResult result8 = WithExecutor::executeAttributeConstant(store, print, y, print_internal_executor, name_internal_executor);
		REQUIRE_FALSE(result8.getResult());
		QueryResult result9 =
			WithExecutor::executeAttributeConstant(store, variable, invalid_name, variable_internal_executor, name_internal_executor);
		REQUIRE_FALSE(result9.getResult());
	}

	SECTION("Constant and Attribute") {
		QueryResult result1 = WithExecutor::executeConstantAttribute(store, mars, proc, name_internal_executor, proc_internal_executor);
		vector<string> expected_result_1 = {"mars"};
		REQUIRE(result1.getSynonymResult("p") == expected_result_1);
		QueryResult result2 = WithExecutor::executeConstantAttribute(store, mars, call, name_internal_executor, call_internal_executor);
		vector<string> expected_result_2 = {"2"};
		REQUIRE(result2.getSynonymResult("c") == expected_result_2);
		QueryResult result3 =
			WithExecutor::executeConstantAttribute(store, index_6, assign_stmt, index_internal_executor, assign_stmt_internal_executor);
		vector<string> expected_result_3 = {"6"};
		REQUIRE(result3.getSynonymResult("a") == expected_result_3);
		QueryResult result4 =
			WithExecutor::executeConstantAttribute(store, x, variable, name_internal_executor, variable_internal_executor);
		vector<string> expected_result_4 = {"x"};
		REQUIRE(result4.getSynonymResult("v") == expected_result_4);

		QueryResult result5 = WithExecutor::executeConstantAttribute(store, x, print, name_internal_executor, print_internal_executor);
		vector<string> expected_result_5 = {"1", "7"};
		vector<string> actual_result_5 = result5.getSynonymResult("pr");
		sort(actual_result_5.begin(), actual_result_5.end());
		REQUIRE(actual_result_5 == expected_result_5);

		QueryResult result6 = WithExecutor::executeConstantAttribute(store, main, call, name_internal_executor, call_internal_executor);
		REQUIRE_FALSE(result6.getResult());
		QueryResult result7 =
			WithExecutor::executeConstantAttribute(store, index_5, while_stmt, index_internal_executor, while_stmt_internal_executor);
		REQUIRE_FALSE(result7.getResult());
		QueryResult result8 = WithExecutor::executeConstantAttribute(store, y, print, name_internal_executor, print_internal_executor);
		REQUIRE_FALSE(result8.getResult());
		QueryResult result9 =
			WithExecutor::executeConstantAttribute(store, invalid_name, variable, name_internal_executor, variable_internal_executor);
		REQUIRE_FALSE(result9.getResult());
	}
}