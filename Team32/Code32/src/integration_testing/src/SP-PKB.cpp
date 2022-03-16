#include <string>

#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "PKB/Storage.h"
#include "SP/Processor.h"
#include "catch_tools.h"

#define variable(name) make_shared<Common::ExpressionProcessor::TerminalNode<string>>(name)
#define constant(value) make_shared<Common::ExpressionProcessor::TerminalNode<ConstVal>>(value)
#define arithmetic(op, lhs, rhs) make_shared<Common::ExpressionProcessor::ArithmeticNode>(op, lhs, rhs)

using namespace std;

TEST_CASE("SP::Processor::process Basic Test") {
	PKB::Storage pkb;
	SP::Processor source_processor = SP::Processor(pkb);

	SECTION("Basic Print Test") {
		string source = "procedure main { print x; }";
		source_processor.process(source);
		StmtInfoPtrSet result = pkb.getStatements();
		REQUIRE_EQUALS(result.size(), 1);
		REQUIRE_FALSE(pkb.checkModifies(1, "x"));
		REQUIRE(pkb.checkUses(1, "x"));
		REQUIRE_FALSE(pkb.checkModifies("main", "x"));
		REQUIRE(pkb.checkUses("main", "x"));
		REQUIRE_EQUALS(pkb.getParent(1), nullptr);
		REQUIRE_EQUALS(pkb.getPreceding(1), nullptr);
	}

	SECTION("Basic Read Test") {
		string source = "procedure main { read x; }";
		source_processor.process(source);
		StmtInfoPtrSet result = pkb.getStatements();
		REQUIRE_EQUALS(result.size(), 1);
		REQUIRE(pkb.checkModifies(1, "x"));
		REQUIRE_FALSE(pkb.checkUses(1, "x"));
		REQUIRE(pkb.checkModifies("main", "x"));
		REQUIRE_FALSE(pkb.checkUses("main", "x"));
		REQUIRE_EQUALS(pkb.getParent(1), nullptr);
		REQUIRE_EQUALS(pkb.getPreceding(1), nullptr);
	}

	SECTION("Basic Assign Test") {
		string source = "procedure main { x = y + 1; }";
		source_processor.process(source);
		StmtInfoPtrSet result = pkb.getStatements();
		REQUIRE_EQUALS(result.size(), 1);
		REQUIRE(pkb.checkModifies(1, "x"));
		REQUIRE_FALSE(pkb.checkModifies(1, "y"));
		REQUIRE_FALSE(pkb.checkUses(1, "x"));
		REQUIRE(pkb.checkUses(1, "y"));
		REQUIRE(pkb.checkModifies("main", "x"));
		REQUIRE_FALSE(pkb.checkModifies("main", "y"));
		REQUIRE_FALSE(pkb.checkUses("main", "x"));
		REQUIRE(pkb.checkUses("main", "y"));
		REQUIRE_EQUALS(pkb.getConstants(), unordered_set<ConstVal>({1}));
		REQUIRE_EQUALS(pkb.getVariables(), unordered_set<VarRef>({"x", "y"}));
		REQUIRE_EQUALS(pkb.getParent(1), nullptr);
		REQUIRE_EQUALS(pkb.getPreceding(1), nullptr);
	}

	SECTION("Basic If Test") {
		string source = "procedure main { if (x < 3) then { read x; } else { read y; x = y + 1; print x; } read x; }";
		source_processor.process(source);
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_map = pkb.getStmtInfoMap();

		SECTION("General") {
			REQUIRE_EQUALS(pkb.getStatements().size(), 6);
			REQUIRE_EQUALS(pkb.getConstants(), unordered_set<ConstVal>({1, 3}));
			REQUIRE_EQUALS(pkb.getVariables(), unordered_set<VarRef>({"x", "y"}));
		}

		SECTION("Parent") {
			REQUIRE_EQUALS(pkb.getParent(1), nullptr);
			REQUIRE_EQUALS(pkb.getParent(2)->getIdentifier(), 1);
			REQUIRE_EQUALS(pkb.getParent(3)->getIdentifier(), 1);
			REQUIRE_EQUALS(pkb.getParent(4)->getIdentifier(), 1);
			REQUIRE_EQUALS(pkb.getParent(5)->getIdentifier(), 1);
			REQUIRE_EQUALS(pkb.getParent(6), nullptr);
			REQUIRE(pkb.checkParents(1, 2));
			REQUIRE_FALSE(pkb.checkParents(2, 1));
			std::unordered_set<shared_ptr<StmtInfo>> expected_children = {stmt_map.find(2)->second, stmt_map.find(3)->second,
			                                                              stmt_map.find(4)->second, stmt_map.find(5)->second};
			REQUIRE_EQUALS(pkb.getChildren(1), expected_children);
		}

		SECTION("Follow") {
			REQUIRE_EQUALS(pkb.getPreceding(1), nullptr);
			REQUIRE_EQUALS(pkb.getPreceding(2), nullptr);
			REQUIRE_EQUALS(pkb.getPreceding(3), nullptr);
			REQUIRE_EQUALS(pkb.getFollower(3), stmt_map.find(4)->second);
			REQUIRE_EQUALS(pkb.getPreceding(4), stmt_map.find(3)->second);
			REQUIRE(pkb.checkFollows(3, 4));
			REQUIRE_FALSE(pkb.checkFollows(4, 3));
		}

		SECTION("Use") {
			REQUIRE(pkb.checkUses(1, "x"));
			REQUIRE(pkb.checkUses(1, "y"));
			REQUIRE_FALSE(pkb.checkUses(2, "x"));
			REQUIRE_FALSE(pkb.checkUses(2, "y"));
			REQUIRE_FALSE(pkb.checkUses(3, "x"));
			REQUIRE_FALSE(pkb.checkUses(3, "y"));
			REQUIRE_FALSE(pkb.checkUses(4, "x"));
			REQUIRE(pkb.checkUses(4, "y"));
			REQUIRE(pkb.checkUses(5, "x"));
			REQUIRE_FALSE(pkb.checkUses(5, "y"));
			std::unordered_set<shared_ptr<StmtInfo>> expected_use_x = {stmt_map.find(1)->second, stmt_map.find(5)->second};
			REQUIRE_EQUALS(pkb.getStmtUsesByVar("x"), expected_use_x);
			std::unordered_set<VarRef> expected_use_stmt_4 = {"y"};
			REQUIRE_EQUALS(pkb.getUsesByStmt(4), expected_use_stmt_4);
		}

		SECTION("Modify") {
			REQUIRE(pkb.checkModifies(1, "x"));
			REQUIRE(pkb.checkModifies(1, "y"));
			REQUIRE(pkb.checkModifies(2, "x"));
			REQUIRE_FALSE(pkb.checkModifies(2, "y"));
			REQUIRE_FALSE(pkb.checkModifies(3, "x"));
			REQUIRE(pkb.checkModifies(3, "y"));
			REQUIRE(pkb.checkModifies(4, "x"));
			REQUIRE_FALSE(pkb.checkModifies(4, "y"));
			REQUIRE_FALSE(pkb.checkModifies(5, "x"));
			REQUIRE_FALSE(pkb.checkModifies(5, "y"));
			std::unordered_set<shared_ptr<StmtInfo>> expected_modify_x = {stmt_map.find(1)->second, stmt_map.find(2)->second,
			                                                              stmt_map.find(4)->second, stmt_map.find(6)->second};
			REQUIRE_EQUALS(pkb.getStmtModifiesByVar("x"), expected_modify_x);
			std::unordered_set<VarRef> expected_modify_stmt_3 = {"y"};
			REQUIRE_EQUALS(pkb.getModifiesByStmt(3), expected_modify_stmt_3);
		}

		SECTION("Pattern") {
			shared_ptr<Common::ExpressionProcessor::ExpressionNode> root =
				arithmetic(MathematicalOperator::Plus, variable("y"), constant(1));
			unordered_set<VarRef> variables = unordered_set<VarRef>({"y"});
			unordered_set<ConstVal> constants = unordered_set<ConstVal>({1});
			Common::ExpressionProcessor::Expression expression = Common::ExpressionProcessor::Expression(root, variables, constants);
			REQUIRE(pkb.patternExists("x", expression, true));
			std::unordered_set<shared_ptr<StmtInfo>> expected_pattern_x = {
				stmt_map.find(4)->second,
			};
			REQUIRE_EQUALS(pkb.getStmtsWithPattern("x", expression, true), expected_pattern_x);
			REQUIRE_EQUALS(pkb.getStmtsWithPatternLHS("x"), expected_pattern_x);
			vector<pair<shared_ptr<StmtInfo>, VarRef>> expected_vector{make_pair(stmt_map.find(4)->second, "x")};
			REQUIRE_EQUALS(pkb.getStmtsWithPatternRHS(expression, true), expected_vector);
			shared_ptr<Common::ExpressionProcessor::ExpressionNode> root_2 =
				arithmetic(MathematicalOperator::Plus, variable("x"), constant(1));
			unordered_set<VarRef> variables_2 = unordered_set<VarRef>({"x"});
			unordered_set<ConstVal> constants_2 = unordered_set<ConstVal>({1});
			Common::ExpressionProcessor::Expression expression_2 =
				Common::ExpressionProcessor::Expression(root_2, variables_2, constants_2);
			REQUIRE_FALSE(pkb.patternExists("x", expression_2, true));
		}

		SECTION("Next") {
			REQUIRE(pkb.checkNext(1, 2));
			REQUIRE(pkb.checkNext(1, 3));
			REQUIRE(pkb.checkNext(3, 4));
			REQUIRE(pkb.checkNext(4, 5));
			REQUIRE(pkb.checkNext(2, 6));
			REQUIRE(pkb.checkNext(5, 6));
		}
	}

	SECTION("Basic While Test") {
		string source = "procedure main { while (x < 3) { print x; read y; } }";
		source_processor.process(source);
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_map = pkb.getStmtInfoMap();

		SECTION("General") {
			REQUIRE_EQUALS(pkb.getStatements().size(), 3);
			REQUIRE_EQUALS(pkb.getConstants(), unordered_set<ConstVal>({3}));
			REQUIRE_EQUALS(pkb.getVariables(), unordered_set<VarRef>({"x", "y"}));
		}

		SECTION("Parent") {
			REQUIRE_EQUALS(pkb.getParent(1), nullptr);
			REQUIRE_EQUALS(pkb.getParent(2)->getIdentifier(), 1);
			REQUIRE_EQUALS(pkb.getParent(3)->getIdentifier(), 1);
			REQUIRE(pkb.checkParents(1, 2));
			REQUIRE_FALSE(pkb.checkParents(2, 1));
			std::unordered_set<shared_ptr<StmtInfo>> expected_children = {
				stmt_map.find(2)->second,
				stmt_map.find(3)->second,
			};
			REQUIRE_EQUALS(pkb.getChildren(1), expected_children);
		}

		SECTION("Follow") {
			REQUIRE_EQUALS(pkb.getPreceding(1), nullptr);
			REQUIRE_EQUALS(pkb.getPreceding(2), nullptr);
			REQUIRE_EQUALS(pkb.getFollower(2), stmt_map.find(3)->second);
			REQUIRE_EQUALS(pkb.getPreceding(3), stmt_map.find(2)->second);
			REQUIRE(pkb.checkFollows(2, 3));
			REQUIRE_FALSE(pkb.checkFollows(3, 2));
		}

		SECTION("Use") {
			REQUIRE(pkb.checkUses(1, "x"));
			REQUIRE_FALSE(pkb.checkUses(1, "y"));
			REQUIRE(pkb.checkUses(2, "x"));
			REQUIRE_FALSE(pkb.checkUses(2, "y"));
			REQUIRE_FALSE(pkb.checkUses(3, "x"));
			REQUIRE_FALSE(pkb.checkUses(3, "y"));
			std::unordered_set<shared_ptr<StmtInfo>> expected_use_x = {stmt_map.find(1)->second, stmt_map.find(2)->second};
			REQUIRE_EQUALS(pkb.getStmtUsesByVar("x"), expected_use_x);
			std::unordered_set<VarRef> expected_use_stmt_2 = {"x"};
			REQUIRE_EQUALS(pkb.getUsesByStmt(2), expected_use_stmt_2);
		}

		SECTION("Modify") {
			REQUIRE_FALSE(pkb.checkModifies(1, "x"));
			REQUIRE(pkb.checkModifies(1, "y"));
			REQUIRE_FALSE(pkb.checkModifies(2, "x"));
			REQUIRE_FALSE(pkb.checkModifies(2, "y"));
			REQUIRE_FALSE(pkb.checkModifies(3, "x"));
			REQUIRE(pkb.checkModifies(3, "y"));
			std::unordered_set<shared_ptr<StmtInfo>> expected_modify_x = {stmt_map.find(1)->second, stmt_map.find(3)->second};
			REQUIRE_EQUALS(pkb.getStmtModifiesByVar("y"), expected_modify_x);
			std::unordered_set<VarRef> expected_modify_stmt_3 = {"y"};
			REQUIRE_EQUALS(pkb.getModifiesByStmt(3), expected_modify_stmt_3);
			REQUIRE_EQUALS(pkb.getStmtModifiesByVar("Y"), std::unordered_set<shared_ptr<StmtInfo>>());
		}

		SECTION("Next") {
			REQUIRE(pkb.checkNext(1, 2));
			REQUIRE(pkb.checkNext(2, 3));
			REQUIRE(pkb.checkNext(3, 1));
		}
	}

	SECTION("Basic Call Test") {
		string source =
			"procedure main { call middle; print a; read b; } procedure middle { call final; } procedure final { read x; print y; }";
		source_processor.process(source);
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_map = pkb.getStmtInfoMap();

		SECTION("General") {
			REQUIRE_EQUALS(pkb.getStatements().size(), 6);
			REQUIRE_EQUALS(pkb.getVariables(), unordered_set<VarRef>({"a", "b", "x", "y"}));
		}

		SECTION("Call") {
			REQUIRE(pkb.checkCall("main", "middle"));
			REQUIRE(pkb.checkCall("middle", "final"));
			REQUIRE_EQUALS(pkb.getCalleeStar("main"), unordered_set<ProcRef>({"middle", "final"}));
			REQUIRE_EQUALS(pkb.getCallerStar("final"), unordered_set<ProcRef>({"main", "middle"}));
		}

		SECTION("Modifies Procedure") {
			REQUIRE_EQUALS(pkb.getModifiesByProc("main"), unordered_set<VarRef>({"b", "x"}));
			REQUIRE_EQUALS(pkb.getModifiesByProc("middle"), unordered_set<VarRef>({"x"}));
			REQUIRE_EQUALS(pkb.getModifiesByProc("final"), unordered_set<VarRef>({"x"}));
			REQUIRE_EQUALS(pkb.getProcModifiesByVar("x"), unordered_set<ProcRef>({"main", "middle", "final"}));
			REQUIRE_EQUALS(pkb.getProcModifiesByVar("b"), unordered_set<ProcRef>({"main"}));
		}

		SECTION("Uses Procedure") {
			REQUIRE_EQUALS(pkb.getUsesByProc("main"), unordered_set<VarRef>({"a", "y"}));
			REQUIRE_EQUALS(pkb.getUsesByProc("middle"), unordered_set<VarRef>({"y"}));
			REQUIRE_EQUALS(pkb.getUsesByProc("final"), unordered_set<VarRef>({"y"}));
			REQUIRE_EQUALS(pkb.getProcUsesByVar("y"), unordered_set<ProcRef>({"main", "middle", "final"}));
			REQUIRE_EQUALS(pkb.getProcUsesByVar("a"), unordered_set<ProcRef>({"main"}));
		}
	}

	SECTION("Complex Test") {
		string source =
			"procedure main { if ( ((!(x < 5)) && (y > 10)) || (z == 1)) "
			"then { read x; print y; while ( z < y ) { z = 5 * y + 2; }} else { read x; if (x == a) "
			"then { print a; read z;} else { x = 3 * a + y + 2; } print x;} read x; }";
		source_processor.process(source);
		unordered_map<StmtRef, shared_ptr<StmtInfo>> stmt_map = pkb.getStmtInfoMap();

		SECTION("General") {
			REQUIRE_EQUALS(pkb.getStatements().size(), 12);
			REQUIRE_EQUALS(pkb.getConstants(), unordered_set<ConstVal>({1, 2, 3, 5, 10}));
			REQUIRE_EQUALS(pkb.getVariables(), unordered_set<VarRef>({"a", "x", "y", "z"}));
		}

		SECTION("Parent") {
			std::unordered_set<shared_ptr<StmtInfo>> expected_children_1 = {
				stmt_map.find(2)->second,  stmt_map.find(3)->second, stmt_map.find(4)->second, stmt_map.find(5)->second,
				stmt_map.find(6)->second,  stmt_map.find(7)->second, stmt_map.find(8)->second, stmt_map.find(9)->second,
				stmt_map.find(10)->second, stmt_map.find(11)->second};
			REQUIRE_EQUALS(pkb.getParentStar(1), std::unordered_set<shared_ptr<StmtInfo>>());
			REQUIRE_EQUALS(pkb.getChildStar(1), expected_children_1);
			std::unordered_set<shared_ptr<StmtInfo>> expected_children_7 = {
				stmt_map.find(8)->second,
				stmt_map.find(9)->second,
				stmt_map.find(10)->second,
			};
			REQUIRE_EQUALS(pkb.getParentStar(7), std::unordered_set<shared_ptr<StmtInfo>>{stmt_map.find(1)->second});
			REQUIRE_EQUALS(pkb.getChildStar(7), expected_children_7);
			std::unordered_set<shared_ptr<StmtInfo>> expected_parent_8 = {
				stmt_map.find(1)->second,
				stmt_map.find(7)->second,
			};
			REQUIRE_EQUALS(pkb.getParentStar(8), expected_parent_8);
			REQUIRE_EQUALS(pkb.getChildStar(8), std::unordered_set<shared_ptr<StmtInfo>>());
		}

		SECTION("Follow") {
			REQUIRE_EQUALS(pkb.getFollowerStar(1), std::unordered_set<shared_ptr<StmtInfo>>({stmt_map.find(12)->second}));
			std::unordered_set<shared_ptr<StmtInfo>> expected_follow_2 = {
				stmt_map.find(3)->second,
				stmt_map.find(4)->second,
			};
			REQUIRE_EQUALS(pkb.getFollowerStar(2), expected_follow_2);
			REQUIRE_EQUALS(pkb.getFollower(6), stmt_map.find(7)->second);
			std::unordered_set<shared_ptr<StmtInfo>> expected_follow_6_star = {
				stmt_map.find(7)->second,
				stmt_map.find(11)->second,
			};
			REQUIRE_EQUALS(pkb.getFollowerStar(6), expected_follow_6_star);
			REQUIRE_EQUALS(pkb.getPreceding(6), nullptr);
			REQUIRE_EQUALS(pkb.getPreceding(11), stmt_map.find(7)->second);
			std::unordered_set<shared_ptr<StmtInfo>> expected_precede_11_star = {
				stmt_map.find(6)->second,
				stmt_map.find(7)->second,
			};
			REQUIRE_EQUALS(pkb.getPrecedingStar(11), expected_precede_11_star);
		}

		SECTION("Use") {
			std::unordered_set<shared_ptr<StmtInfo>> expected_use_x = {
				stmt_map.find(1)->second,
				stmt_map.find(7)->second,
				stmt_map.find(11)->second,
			};

			REQUIRE_EQUALS(pkb.getStmtUsesByVar("x"), expected_use_x);
			std::unordered_set<shared_ptr<StmtInfo>> expected_use_y = {
				stmt_map.find(1)->second, stmt_map.find(3)->second, stmt_map.find(4)->second,
				stmt_map.find(5)->second, stmt_map.find(7)->second, stmt_map.find(10)->second,
			};
			REQUIRE_EQUALS(pkb.getStmtUsesByVar("y"), expected_use_y);
			std::unordered_set<shared_ptr<StmtInfo>> expected_use_a = {
				stmt_map.find(1)->second,
				stmt_map.find(7)->second,
				stmt_map.find(8)->second,
				stmt_map.find(10)->second,
			};
			REQUIRE_EQUALS(pkb.getStmtUsesByVar("a"), expected_use_a);
			REQUIRE(pkb.checkUses(1, "z"));
			REQUIRE_FALSE(pkb.checkUses(2, "z"));
			REQUIRE_FALSE(pkb.checkUses(5, "z"));
			REQUIRE_FALSE(pkb.checkUses(9, "z"));
			std::unordered_set<VarRef> expected_use_1 = {"x", "y", "z", "a"};
			REQUIRE_EQUALS(pkb.getUsesByStmt(1), expected_use_1);
			std::unordered_set<VarRef> expected_use_5 = {"y"};
			REQUIRE_EQUALS(pkb.getUsesByStmt(5), expected_use_5);
			REQUIRE_EQUALS(pkb.getUsesByStmt(2), std::unordered_set<VarRef>());
		}

		SECTION("Modifies") {
			std::unordered_set<shared_ptr<StmtInfo>> expected_modify_x = {stmt_map.find(1)->second,  stmt_map.find(2)->second,
			                                                              stmt_map.find(6)->second,  stmt_map.find(7)->second,
			                                                              stmt_map.find(10)->second, stmt_map.find(12)->second};
			REQUIRE_EQUALS(pkb.getStmtModifiesByVar("x"), expected_modify_x);
			REQUIRE_FALSE(pkb.checkModifies(3, "y"));
			REQUIRE(pkb.checkModifies(4, "z"));
			REQUIRE(pkb.checkModifies(5, "z"));
			REQUIRE_FALSE(pkb.checkModifies(10, "a"));
			std::unordered_set<VarRef> expected_modify_1 = {"x", "z"};
			REQUIRE_EQUALS(pkb.getModifiesByStmt(1), expected_modify_1);
		}

		SECTION("Pattern") {
			shared_ptr<Common::ExpressionProcessor::ArithmeticNode> first =
				arithmetic(MathematicalOperator::Times, constant(5), variable("y"));
			shared_ptr<Common::ExpressionProcessor::ArithmeticNode> root = arithmetic(MathematicalOperator::Plus, first, constant(2));
			unordered_set<VarRef> variables = unordered_set<VarRef>({"y"});
			unordered_set<ConstVal> constants = unordered_set<ConstVal>({2, 5});
			Common::ExpressionProcessor::Expression expression = Common::ExpressionProcessor::Expression(root, variables, constants);
			REQUIRE(pkb.patternExists("z", expression, true));
			REQUIRE_FALSE(pkb.patternExists("y", expression, true));
			unordered_set<VarRef> sub_variables_1 = unordered_set<VarRef>({"y"});
			unordered_set<ConstVal> sub_constants_1 = unordered_set<ConstVal>({2});
			shared_ptr<Common::ExpressionProcessor::ArithmeticNode> sub_root_1 =
				arithmetic(MathematicalOperator::Plus, variable("y"), constant(2));
			Common::ExpressionProcessor::Expression sub_expression_1 =
				Common::ExpressionProcessor::Expression(sub_root_1, sub_variables_1, sub_constants_1);
			REQUIRE_FALSE(pkb.patternExists("z", sub_expression_1, false));
			REQUIRE_FALSE(pkb.patternExists("z", sub_expression_1, true));
			unordered_set<VarRef> sub_variables_2 = unordered_set<VarRef>({"y"});
			unordered_set<ConstVal> sub_constants_2 = unordered_set<ConstVal>({5});
			shared_ptr<Common::ExpressionProcessor::ArithmeticNode> sub_root_2 =
				arithmetic(MathematicalOperator::Times, constant(5), variable("y"));
			Common::ExpressionProcessor::Expression sub_expression_2 =
				Common::ExpressionProcessor::Expression(sub_root_2, sub_variables_2, sub_constants_2);
			REQUIRE(pkb.patternExists("z", sub_expression_2, false));
			REQUIRE_FALSE(pkb.patternExists("z", sub_expression_2, true));
			shared_ptr<Common::ExpressionProcessor::ArithmeticNode> sub_root_2_swap =
				arithmetic(MathematicalOperator::Times, variable("y"), constant(5));
			Common::ExpressionProcessor::Expression sub_expression_2_swap =
				Common::ExpressionProcessor::Expression(sub_root_2_swap, sub_variables_2, sub_constants_2);
			REQUIRE_FALSE(pkb.patternExists("z", sub_expression_2_swap, false));
			REQUIRE_FALSE(pkb.patternExists("z", sub_expression_2_swap, true));

			vector<pair<shared_ptr<StmtInfo>, VarRef>> expected_stmts_with_sub_exp_2 = {
				make_pair(stmt_map.find(5)->second, "z"),
			};
			REQUIRE_EQUALS(pkb.getStmtsWithPatternRHS(sub_expression_2, false), expected_stmts_with_sub_exp_2);
			std::unordered_set<shared_ptr<StmtInfo>> expected_assign_z = {stmt_map.find(5)->second};
			REQUIRE_EQUALS(pkb.getStmtsWithPattern("z", sub_expression_2, false), expected_assign_z);
			REQUIRE_EQUALS(pkb.getStmtsWithPattern("z", sub_expression_2, true), std::unordered_set<shared_ptr<StmtInfo>>());
			std::unordered_set<shared_ptr<StmtInfo>> expected_assign_x = {stmt_map.find(10)->second};
			REQUIRE_EQUALS(pkb.getStmtsWithPatternLHS("x"), expected_assign_x);
		}

		SECTION("Next") {
			REQUIRE(pkb.checkNext(1, 2));
			REQUIRE(pkb.checkNext(2, 3));
			REQUIRE(pkb.checkNext(3, 4));
			REQUIRE(pkb.checkNext(4, 5));
			REQUIRE(pkb.checkNext(5, 4));
			REQUIRE(pkb.checkNext(4, 12));
			REQUIRE(pkb.checkNext(1, 6));
			REQUIRE(pkb.checkNext(6, 7));
			REQUIRE(pkb.checkNext(7, 8));
			REQUIRE(pkb.checkNext(8, 9));
			REQUIRE(pkb.checkNext(9, 11));
			REQUIRE(pkb.checkNext(7, 10));
			REQUIRE(pkb.checkNext(10, 11));
			REQUIRE(pkb.checkNext(11, 12));
		}
	}
}
