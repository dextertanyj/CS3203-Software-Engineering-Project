#include "DispatchProcessors.h"

QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processSingleArgument(
	Types::ClauseType type, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> map,
	vector<Types::ReferenceArgument> args) {
	if (args.size() != 1) {
		throw QP::QueryException("Incorrect number of arguments.");
	}
	Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryException("Incorrect argument type.");
	}
	return {type, iter->second(args)};
}

QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processDoubleArgument(
	Types::ClauseType type,
	unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>> map,
	vector<Types::ReferenceArgument> args) {
	if (args.size() != 2) {
		throw QP::QueryException("Incorrect number of arguments.");
	}
	Types::ArgumentDispatchKey outer_key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		outer_key = args.at(0).getSynonym().type;
	}
	Types::ArgumentDispatchKey inner_key = args.at(1).getType();
	if (args.at(1).getType() == Types::ReferenceType::Synonym) {
		inner_key = args.at(1).getSynonym().type;
	}
	auto outer_map_iter = map.find(outer_key);
	if (outer_map_iter == map.end()) {
		throw QP::QueryException("Incorrect first argument type.");
	}
	auto inner_map = outer_map_iter->second;
	auto inner_map_iter = inner_map.find(inner_key);
	if (inner_map_iter == inner_map.end()) {
		throw QP::QueryException("Incorrect second argument type.");
	}
	return {type, inner_map_iter->second(args)};
}

QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processDoubleArgument(
	unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle>> map,
	vector<Types::ReferenceArgument> args) {
	if (args.size() != 2) {
		throw QP::QueryException("Incorrect number of arguments.");
	}
	Types::ArgumentDispatchKey outer_key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		outer_key = args.at(0).getSynonym().type;
	}
	Types::ArgumentDispatchKey inner_key = args.at(1).getType();
	if (args.at(1).getType() == Types::ReferenceType::Synonym) {
		inner_key = args.at(1).getSynonym().type;
	}
	auto outer_map_iter = map.find(outer_key);
	if (outer_map_iter == map.end()) {
		throw QP::QueryException("Incorrect first argument type.");
	}
	auto inner_map = outer_map_iter->second;
	auto inner_map_iter = inner_map.find(inner_key);
	if (inner_map_iter == inner_map.end()) {
		throw QP::QueryException("Incorrect second argument type.");
	}
	return {inner_map_iter->second.first, inner_map_iter->second.second(args)};
}

QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processTripleArgument(
	Types::ClauseType type,
	unordered_map<Types::ArgumentDispatchKey,
                  unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>>
		map,
	vector<Types::ReferenceArgument> args) {
	if (args.size() != 3) {
		throw QP::QueryException("Incorrect number of arguments.");
	}
	Types::ArgumentDispatchKey outer_key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		outer_key = args.at(0).getSynonym().type;
	}
	Types::ArgumentDispatchKey middle_key = args.at(1).getType();
	if (args.at(1).getType() == Types::ReferenceType::Synonym) {
		middle_key = args.at(1).getSynonym().type;
	}
	Types::ArgumentDispatchKey inner_key = args.at(2).getType();
	if (args.at(2).getType() == Types::ReferenceType::Synonym) {
		inner_key = args.at(2).getSynonym().type;
	}
	auto outer_map_iter = map.find(outer_key);
	if (outer_map_iter == map.end()) {
		throw QP::QueryException("Incorrect first argument type.");
	}
	auto middle_map = outer_map_iter->second;
	auto middle_map_iter = middle_map.find(middle_key);
	if (middle_map_iter == middle_map.end()) {
		throw QP::QueryException("Incorrect second argument type.");
	}
	auto inner_map = middle_map_iter->second;
	auto inner_map_iter = inner_map.find(inner_key);
	if (inner_map_iter == inner_map.end()) {
		throw QP::QueryException("Incorrect third argument type.");
	}
	return {type, inner_map_iter->second(args)};
}
