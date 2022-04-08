#include <string>
#include <vector>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "PKB/Storage.h"
#include "QP/Preprocessor/QueryExpressionLexer.h"
#include "QP/QueryProcessor.h"
#include "catch.hpp"

using namespace QP;
using namespace std;

TEST_CASE("Basic select") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::While);
	pkb.setStmtType(4, StmtType::If);
	pkb.setConstant({1, 4, 90, 2000});
	pkb.setUses(3, "x");
	pkb.setProc("proc", 1, 4);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Select statement") {
		string query = "stmt s; Select s";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2", "3", "4"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Select if statement") {
		string query = "if if; assign a; Select if";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"4"};
		REQUIRE(result == expected_result);
	}

	SECTION("Select variable") {
		string query = "variable v; Select v";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"x"};
		REQUIRE(result == expected_result);
	}

	SECTION("Select constant") {
		string query = "constant c; Select c";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2000", "4", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Select procedure") {
		string query = "procedure p; Select p";

		vector<string> result = processor.processQuery(query);

		REQUIRE(result == vector<string>{"proc"});
	}

	SECTION("Select tuple") {
		string query1 = "stmt s; Select <s, s>";
		string query2 = "stmt s; variable v; Select <s, v>";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"1 1", "2 2", "3 3", "4 4"};
		vector<string> expected_result2 = {"1 x", "2 x", "3 x", "4 x"};
		sort(result1.begin(), result1.end());
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Select boolean") {
		string query1 = "Select BOOLEAN";
		string query2 = "Select BOOLEAN such that Uses(3, \"x\")";
		string query3 = "Select BOOLEAN such that Uses(1, \"x\")";
		string query4 = "stmt s; variable s; Select BOOLEAN";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);
		vector<string> result3 = processor.processQuery(query3);
		vector<string> result4 = processor.processQuery(query4);

		REQUIRE(result1 == vector<string>{"TRUE"});
		REQUIRE(result2 == vector<string>{"TRUE"});
		REQUIRE(result3 == vector<string>{"FALSE"});
		REQUIRE(result4 == vector<string>{"FALSE"});
	}

	SECTION("Select attribute") {
		string query = "stmt s; Select s.stmt#";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2", "3", "4"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}
};

TEST_CASE("Parent clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::If);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::While);
	pkb.setStmtType(4, StmtType::If);
	pkb.setParent(1, 2);
	pkb.setParent(1, 3);
	pkb.setParent(3, 4);
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Parent") {
		string query1 = "stmt s; Select s such that Parent(1, s)";
		string query2 = "stmt s1, s2; Select <s1, s2> such that Parent(s1, s2)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"2", "3"};
		vector<string> expected_result2 = {"1 2", "1 3", "3 4"};
		sort(result1.begin(), result1.end());
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Parent*") {
		string query1 = "stmt s; Select s such that Parent*(1, s)";
		string query2 = "if i1, i2; Select BOOLEAN such that Parent*(i1, i2)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"2", "3", "4"};
		vector<string> expected_result2 = {"TRUE"};
		sort(result1.begin(), result1.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("Follows clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::If);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setFollows(1, 4);
	pkb.setFollows(2, 3);
	pkb.setFollows(4, 5);
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Follows") {
		string query1 = "read r; Select r such that Follows(r, 3)";
		string query2 = "stmt s; assign a; Select <s, a> such that Follows(s, a)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"2"};
		vector<string> expected_result2 = {"1 4", "4 5"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Follows*") {
		string query1 = "stmt s; Select s such that Follows*(1, s)";
		string query2 = "if i; assign a; Select BOOLEAN such that Follows*(i, a)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"4", "5"};
		vector<string> expected_result2 = {"TRUE"};
		sort(result1.begin(), result1.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("Uses clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::If);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setUses(1, "a");
	pkb.setUses(3, "b");
	pkb.setUses(4, {"a", "x", "y"});
	pkb.setUses(5, "z");
	pkb.setProc("proc1", 1, 2);
	pkb.setProc("proc2", 3, 5);
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("UsesS") {
		string query1 = "assign a; Select a such that Uses(a, \"a\")";
		string query2 = "stmt s; variable v; Select <s, v> such that Uses(s, v)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"4"};
		vector<string> expected_result2 = {"1 a", "3 b", "4 a", "4 x", "4 y", "5 z"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("UsesP") {
		string query1 = "procedure BOOLEAN; Select BOOLEAN such that Uses(BOOLEAN, \"z\")";
		string query2 = "procedure p; Select BOOLEAN such that Uses(p, \"a\")";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"proc2"};
		vector<string> expected_result2 = {"TRUE"};
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("Modifies clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Call);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setModifies(2, "x");
	pkb.setModifies(4, "x");
	pkb.setModifies(5, "z");
	pkb.setProc("proc1", 1, 2);
	pkb.setProc("proc2", 3, 5);
	pkb.setCall(1, "proc2");
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("ModifiesS") {
		string query1 = "assign a; Select a such that Modifies(a, \"x\")";
		string query2 = "stmt s; variable v; Select <s, v> such that Modifies(s, v)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"4"};
		vector<string> expected_result2 = {"1 x", "1 z", "2 x", "4 x", "5 z"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("ModifiesP") {
		string query1 = "procedure BOOLEAN; Select BOOLEAN such that Modifies(BOOLEAN, \"z\")";
		string query2 = "Select BOOLEAN such that Modifies(\"proc1\", \"z\")";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"proc1", "proc2"};
		vector<string> expected_result2 = {"TRUE"};
		sort(result1.begin(), result1.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("Calls clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Call);
	pkb.setStmtType(2, StmtType::Call);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Call);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setProc("proc1", 1, 1);
	pkb.setProc("proc2", 2, 3);
	pkb.setProc("proc3", 4, 4);
	pkb.setProc("proc4", 5, 5);
	pkb.setCall(1, "proc2");
	pkb.setCall(2, "proc3");
	pkb.setCall(3, "proc4");
	pkb.setCall(4, "proc4");
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Calls") {
		string query1 = "procedure p; Select p such that Calls(p, \"proc2\")";
		string query2 = "procedure p1, p2; Select <p1, p2> such that Calls(p1, p2)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"proc1"};
		vector<string> expected_result2 = {"proc1 proc2", "proc2 proc3", "proc2 proc4", "proc3 proc4"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Calls*") {
		string query1 = "procedure p; Select p such that Calls*(p, \"proc4\")";
		string query2 = "Select BOOLEAN such that Calls*(\"proc2\", \"proc1\")";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"proc1", "proc2", "proc3"};
		vector<string> expected_result2 = {"FALSE"};
		sort(result1.begin(), result1.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("Next clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Read);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::While);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setNext(1, 2);
	pkb.setNext(2, 3);
	pkb.setNext(3, 4);
	pkb.setNext(4, 5);
	pkb.setNext(5, 4);
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Next") {
		string query1 = "stmt s; Select s such that Next(s, 5)";
		string query2 = "stmt s; read r; Select <s, r> such that Next(r, s)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"4"};
		vector<string> expected_result2 = {"2 1"};
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Next*") {
		string query1 = "read r; stmt s; Select s such that Next*(r, s)";
		string query2 = "stmt s; Select BOOLEAN such that Next*(s, s)";
		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"2", "3", "4", "5"};
		vector<string> expected_result2 = {"TRUE"};
		sort(result1.begin(), result1.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("Affects clause") {
	PKB::Storage pkb = PKB::Storage();

	vector<string> assign_token1 = {"1"};
	Preprocessor::QueryExpressionLexer lexer1 = Preprocessor::QueryExpressionLexer(assign_token1);
	auto expression1 =
		Common::ExpressionProcessor::ExpressionParser{lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	vector<string> assign_token2 = {"x", "+", "10", "*", "3"};
	Preprocessor::QueryExpressionLexer lexer2 = Preprocessor::QueryExpressionLexer(assign_token2);
	auto expression2 =
		Common::ExpressionProcessor::ExpressionParser{lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	vector<string> assign_token3 = {"y"};
	Preprocessor::QueryExpressionLexer lexer3 = Preprocessor::QueryExpressionLexer(assign_token1);
	auto expression3 =
		Common::ExpressionProcessor::ExpressionParser{lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	vector<string> assign_token4 = {"y", "+", "x"};
	Preprocessor::QueryExpressionLexer lexer4 = Preprocessor::QueryExpressionLexer(assign_token2);
	auto expression4 =
		Common::ExpressionProcessor::ExpressionParser{lexer4, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();

	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::Assign);

	pkb.setAssign(1, "x", expression1);
	pkb.setModifies(1, "x");
	pkb.setAssign(2, "y", expression2);
	pkb.setModifies(2, "y");
	pkb.setUses(2, "x");
	pkb.setAssign(3, "x", expression3);
	pkb.setModifies(3, "x");
	pkb.setUses(3, "y");
	pkb.setAssign(4, "z", expression4);
	pkb.setModifies(4, "z");
	pkb.setUses(4, "x");
	pkb.setUses(4, "y");

	pkb.setNext(1, 2);
	pkb.setNext(2, 3);
	pkb.setNext(3, 4);

	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Affects") {
		string query1 = "Select BOOLEAN such that Affects(1, 2)";
		string query2 = "Select BOOLEAN such that Affects(1, _)";
		string query3 = "Select BOOLEAN such that Affects(_, 2)";
		string query4 = "Select BOOLEAN such that Affects(_, _)";
		string query5 = "stmt s; Select s such that Affects(_, s)";
		string query6 = "stmt s; Select s such that Affects(1, s)";
		string query7 = "stmt s; Select s such that Affects(s, _)";
		string query8 = "stmt s; Select s such that Affects(s, 3)";
		string query9 = "stmt s, s1; Select <s, s1> such that Affects(s, s1)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);
		vector<string> result3 = processor.processQuery(query3);
		vector<string> result4 = processor.processQuery(query4);
		vector<string> result5 = processor.processQuery(query5);
		vector<string> result6 = processor.processQuery(query6);
		vector<string> result7 = processor.processQuery(query7);
		vector<string> result8 = processor.processQuery(query8);
		vector<string> result9 = processor.processQuery(query9);

		vector<string> expected_result1 = {"TRUE"};
		vector<string> expected_result2 = {"TRUE"};
		vector<string> expected_result3 = {"TRUE"};
		vector<string> expected_result4 = {"TRUE"};
		vector<string> expected_result5 = {"2", "3", "4"};
		vector<string> expected_result6 = {"2"};
		vector<string> expected_result7 = {"1", "2", "3"};
		vector<string> expected_result8 = {"2"};
		vector<string> expected_result9 = {"1 2", "2 3", "2 4", "3 4"};
		sort(result5.begin(), result5.end());
		sort(result7.begin(), result7.end());
		sort(result9.begin(), result9.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
		REQUIRE(result3 == expected_result3);
		REQUIRE(result4 == expected_result4);
		REQUIRE(result5 == expected_result5);
		REQUIRE(result6 == expected_result6);
		REQUIRE(result7 == expected_result7);
		REQUIRE(result8 == expected_result8);
		REQUIRE(result9 == expected_result9);
	}

	SECTION("Affects*") {
		string query1 = "Select BOOLEAN such that Affects*(2, 3)";
		string query2 = "Select BOOLEAN such that Affects*(2, _)";
		string query3 = "Select BOOLEAN such that Affects*(_, 3)";
		string query4 = "Select BOOLEAN such that Affects*(_, _)";
		string query5 = "stmt s; Select s such that Affects*(_, s)";
		string query6 = "stmt s; Select s such that Affects*(2, s)";
		string query7 = "stmt s; Select s such that Affects*(s, _)";
		string query8 = "stmt s; Select s such that Affects*(s, 3)";
		string query9 = "stmt s, s1; Select <s, s1> such that Affects*(s, s1)";
		
		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);
		vector<string> result3 = processor.processQuery(query3);
		vector<string> result4 = processor.processQuery(query4);
		vector<string> result5 = processor.processQuery(query5);
		vector<string> result6 = processor.processQuery(query6);
		vector<string> result7 = processor.processQuery(query7);
		vector<string> result8 = processor.processQuery(query8);
		vector<string> result9 = processor.processQuery(query9);

		vector<string> expected_result1 = {"TRUE"};
		vector<string> expected_result2 = {"TRUE"};
		vector<string> expected_result3 = {"TRUE"};
		vector<string> expected_result4 = {"TRUE"};
		vector<string> expected_result5 = {"2", "3", "4"};
		vector<string> expected_result6 = {"3", "4"};
		vector<string> expected_result7 = {"1", "2", "3"};
		vector<string> expected_result8 = {"1", "2"};
		vector<string> expected_result9 = {"1 2", "1 3", "1 4", "2 3", "2 4", "3 4"};
		sort(result5.begin(), result5.end());
		sort(result6.begin(), result6.end());
		sort(result7.begin(), result7.end());
		sort(result8.begin(), result8.end());
		sort(result9.begin(), result9.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
		REQUIRE(result3 == expected_result3);
		REQUIRE(result4 == expected_result4);
		REQUIRE(result5 == expected_result5);
		REQUIRE(result6 == expected_result6);
		REQUIRE(result7 == expected_result7);
		REQUIRE(result8 == expected_result8);
		REQUIRE(result9 == expected_result9);
	}
};

TEST_CASE("Pattern clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::If);
	pkb.setStmtType(2, StmtType::While);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::If);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::While);

	// Assign statements
	vector<string> assign_token1 = {"x", "+", "1", "*", "9"};
	Preprocessor::QueryExpressionLexer lexer1 = Preprocessor::QueryExpressionLexer(assign_token1);
	auto expression1 =
		Common::ExpressionProcessor::ExpressionParser{lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(3, "y", expression1);
	vector<string> assign_token2 = {"x", "+", "y"};
	Preprocessor::QueryExpressionLexer lexer2 = Preprocessor::QueryExpressionLexer(assign_token2);
	auto expression2 =
		Common::ExpressionProcessor::ExpressionParser{lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(5, "y", expression2);

	// While statements
	pkb.setWhileControl(2, "x");
	pkb.setWhileControl(2, "y");
	pkb.setUses(2, "x");
	pkb.setUses(2, "y");

	// If statements
	pkb.setIfControl(1, "x");
	pkb.setIfControl(4, "y");
	pkb.setIfControl(4, "z");
	pkb.setUses(1, "x");
	pkb.setUses(4, "y");
	pkb.setUses(4, "z");

	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Assign") {
		string query1 = "assign a; variable v; Select a pattern a(v, _\"1 * 9\"_)";
		string query2 = "assign a; while w; Select <a, w> pattern a(\"y\", _)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"3"};
		vector<string> expected_result2 = {"3 2", "3 6", "5 2", "5 6"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("If") {
		string query1 = "if i; Select i pattern i(\"x\", _, _)";
		string query2 = "if i; variable v; Select <i, v> pattern i(v, _, _)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"1"};
		vector<string> expected_result2 = {"1 x", "4 y", "4 z"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("While") {
		string query1 = "while w; Select w pattern w(\"x\", _)";
		string query2 = "while w; variable v; Select v pattern w(v, _)";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"2"};
		vector<string> expected_result2 = {"x", "y"};
		sort(result2.begin(), result2.end());
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}
};

TEST_CASE("One such that clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::While);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setConstant({1, 4, 90, 2000});
	pkb.setUses(1, "x");
	pkb.setParent(2, 3);
	pkb.setFollows(1, 2);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Trivial clause") {
		string query = "constant c; Select c such that Parent(2, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "2000", "4", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Trivial clause, no result") {
		string query = "constant c; assign a; Select c such that Uses(a, \"y\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}

	SECTION("Non trivial clause") {
		string query = "stmt s; variable v; Select v such that Uses(s, v)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"x"};
		REQUIRE(result == expected_result);
	}

	SECTION("Non trivial clause, no result") {
		string query = "stmt s; assign a; Select s such that Parent*(s, a)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}
};

TEST_CASE("One pattern clause") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setConstant({1, 90});

	vector<string> assign_token1 = {"90"};
	Preprocessor::QueryExpressionLexer lexer1 = Preprocessor::QueryExpressionLexer(assign_token1);
	auto expression1 =
		Common::ExpressionProcessor::ExpressionParser{lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	Preprocessor::QueryExpressionLexer lexer2 = Preprocessor::QueryExpressionLexer(assign_token2);
	auto expression2 =
		Common::ExpressionProcessor::ExpressionParser{lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(2, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	Preprocessor::QueryExpressionLexer lexer3 = Preprocessor::QueryExpressionLexer(assign_token3);
	auto expression3 =
		Common::ExpressionProcessor::ExpressionParser{lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(3, "x", expression3);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Trivial clause") {
		string query = "constant c; assign a; variable v; Select c pattern a (v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	};

	SECTION("Trivial clause, no result") {
		string query = "constant c; assign a; variable v; Select c pattern a (v, \"x\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}

	SECTION("Non-trivial clause") {
		string query = "assign a; variable v; Select a pattern a (v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"2", "3"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Non-trivial clause, no result") {
		string query = "assign a; variable v; Select a pattern a (v, \"x\")";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}
};

TEST_CASE("One such that and one pattern") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::While);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setStmtType(5, StmtType::Assign);

	pkb.setConstant({1, 90});
	pkb.setParent(2, 3);
	pkb.setFollows(1, 2);
	pkb.setFollows(3, 4);
	pkb.setFollows(4, 5);

	pkb.setUses(4, "x");
	pkb.setUses(5, "x");
	pkb.setUses(5, "y");

	vector<string> assign_token1 = {"90"};
	Preprocessor::QueryExpressionLexer lexer1 = Preprocessor::QueryExpressionLexer(assign_token1);
	auto expression1 =
		Common::ExpressionProcessor::ExpressionParser{lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	Preprocessor::QueryExpressionLexer lexer2 = Preprocessor::QueryExpressionLexer(assign_token2);
	auto expression2 =
		Common::ExpressionProcessor::ExpressionParser{lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(4, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	Preprocessor::QueryExpressionLexer lexer3 = Preprocessor::QueryExpressionLexer(assign_token3);
	auto expression3 =
		Common::ExpressionProcessor::ExpressionParser{lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(5, "x", expression3);

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Both trivial") {
		string query = "constant c; stmt s1, s2; assign a; Select c such that Parent(s1, s2) pattern a(_, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "90"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	};

	SECTION("Forms a trivial group") {
		string query = "constant c; stmt s1, s2; assign a; Select c such that Follows(s1, a) pattern a(_, _\"0\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {};
		REQUIRE(result == expected_result);
	}

	SECTION("Non-trivial such that, trivial pattern") {
		string query = "stmt s1, s2; assign a; Select s1 such that Follows(s1, s2) pattern a(_, _\"1\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"1", "3", "4"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Trivial such that, non-trivial pattern") {
		string query = "stmt s1, s2; assign a; variable v; Select a such that Follows(s1, s2) pattern a(v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"4", "5"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Both non-trivial") {
		string query = "assign a; variable v; Select a such that Uses(a, v) pattern a(v, _\"x\"_)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"5"};
		REQUIRE(result == expected_result);
	}
}

TEST_CASE("With clauses") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Read);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Assign);

	pkb.setModifies(1, "y");
	pkb.setUses(2, "y");
	pkb.setModifies(4, "x");
	pkb.setProc("proc1", 1, 3);
	pkb.setProc("proc2", 4, 4);
	pkb.setCall(3, "proc2");
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Trivial clauses") {
		string query1 = "stmt s, s1; Select s.stmt# with 1 = s1.stmt#";
		string query2 = "stmt s, s1; Select s.stmt# with 10 = s1.stmt#";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		sort(result1.begin(), result1.end());
		REQUIRE(result1 == vector<string>({"1", "2", "3", "4"}));
		REQUIRE(result2 == vector<string>({}));
	}

	SECTION("int & int") {
		string query1 = "Select BOOLEAN with 1 = 1";
		string query2 = "Select BOOLEAN with 2 = 1";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		REQUIRE(result1 == vector<string>({"TRUE"}));
		REQUIRE(result2 == vector<string>({"FALSE"}));
	}

	SECTION("synonym & int") {
		string query1 = "stmt s; Select s with s.stmt# = 1";
		string query2 = "assign a; Select a with a.stmt# = 1";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		REQUIRE(result1 == vector<string>({"1"}));
		REQUIRE(result2 == vector<string>({}));
	}

	SECTION("int & synonynm") {
		string query1 = "stmt s; Select s with 1 = s.stmt#";
		string query2 = "assign a; Select a with 1 = a.stmt#";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		REQUIRE(result1 == vector<string>({"1"}));
		REQUIRE(result2 == vector<string>({}));
	}

	SECTION("string & string") {
		string query1 = "stmt s; Select s.stmt# with \"a\" = \"a\"";
		string query2 = "stmt s; Select s.stmt# with \"a\" = \"b\"";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		sort(result1.begin(), result1.end());
		REQUIRE(result1 == vector<string>({"1", "2", "3", "4"}));
		REQUIRE(result2 == vector<string>({}));
	}

	SECTION("synonym & string") {
		string query1 = "variable v; Select v.varName with v.varName = \"x\"";
		string query2 = "variable v; Select v with v.varName = \"b\"";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		REQUIRE(result1 == vector<string>({"x"}));
		REQUIRE(result2 == vector<string>({}));
	}

	SECTION("string & synonym") {
		string query1 = "variable v; Select v.varName with  \"x\" = v.varName";
		string query2 = "variable v; Select v with  \"b\" = v.varName";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		REQUIRE(result1 == vector<string>({"x"}));
		REQUIRE(result2 == vector<string>({}));
	}

	SECTION("synonym & synonym") {
		string query1 = "stmt s, s1; Select s.stmt# with s.stmt# = s1.stmt#";
		string query2 = "print p; read r; Select p with p.varName = r.varName";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		sort(result1.begin(), result1.end());
		REQUIRE(result1 == vector<string>({"1", "2", "3", "4"}));
		REQUIRE(result2 == vector<string>({"2"}));
	}
}

TEST_CASE("Multiple clauses") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Read);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::While);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Print);
	pkb.setFollows(1, 2);
	pkb.setParent(2, 3);
	pkb.setParent(2, 4);
	pkb.setParent(4, 5);
	pkb.setNext(1, 2);
	pkb.setNext(2, 3);
	pkb.setNext(4, 5);
	pkb.setNext(5, 4);
	pkb.setUses(2, "x");
	pkb.setUses(2, "y");
	pkb.setUses(4, "z");
	pkb.setUses(5, "w");
	pkb.setModifies(1, "y");
	pkb.setModifies(5, "x");
	pkb.setWhileControl(4, "a");
	pkb.setUses(4, "a");
	pkb.setIfControl(2, "a");
	pkb.setIfControl(2, "b");
	pkb.setUses(2, "a");
	pkb.setUses(2, "b");
	pkb.setConstant({10, 20, 30});
	pkb.setProc("proc1", 1, 5);
	pkb.setProc("proc2", 6, 6);
	pkb.setCall(3, "proc2");
	pkb.populateComplexRelations();

	QP::QueryProcessor processor = QP::QueryProcessor(pkb);

	SECTION("Select boolean") {
		string query1 = "read r; stmt s; if i; procedure p; while w; ";
		query1 += "Select BOOLEAN such that Next(r, i) and Parent(2, w) and Uses(p, \"w\") and Calls(p, \"proc2\") and Follows(1, s)";
		string query2 = "Select BOOLEAN such that Follows(1, 2) and Uses(2, \"a\") and Modifies(1, \"x\") and Calls( \"proc1\", \"proc2\")";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"TRUE"};
		vector<string> expected_result2 = {"FALSE"};
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Select tuple") {
		string query1 = "read r; stmt s; if i; while w; procedure p; ";
		query1 += "Select <r, s, i, p> such that Next(r, i) and Parent(2, w) and Uses(p, \"w\") and Calls(p, \"proc2\") and Follows(1, s)";
		string query2 = "read r; while w; ";
		query2 += "Select <r, w> such that Follows(1, 2) and Uses(2, \"w\") and Modifies(1, \"y\") and Calls( \"proc1\", \"proc2\")";

		vector<string> result1 = processor.processQuery(query1);
		vector<string> result2 = processor.processQuery(query2);

		vector<string> expected_result1 = {"1 2 2 proc1"};
		vector<string> expected_result2 = {"1 4"};
		REQUIRE(result1 == expected_result1);
		REQUIRE(result2 == expected_result2);
	}

	SECTION("Clauses form one group") {
		string query = "read r; stmt s1, s2, s3; while w; constant c; procedure p; variable v, v1; if i; Select c ";
		query += "such that Follows(r, i) and Parent(i, s2) and Parent*(s2, s3) and Calls(p, \"proc2\") and Uses(p, v) and Modifies(r, v) ";
		query += "pattern i(v1, _, _) and w(v1, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"10", "20", "30"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}

	SECTION("Clauses form many groups") {
		string query = "read r; stmt s1, s2, s3; while w; constant c; procedure p; variable v, v1; if i; Select c ";
		query += "such that Follows(r, s1) and Parent(s1, s2) ";
		query += "such that Calls(p, \"proc2\") and Uses(p, v) ";
		query += "pattern i(v1, _, _) and w(v1, _) ";
		query += "such that Next(s3, _)";

		vector<string> result = processor.processQuery(query);

		vector<string> expected_result = {"10", "20", "30"};
		sort(result.begin(), result.end());
		REQUIRE(result == expected_result);
	}
}
