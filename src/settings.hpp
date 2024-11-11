#pragma once
#include <vector>
#include <string>
#include <table_type.hpp>
#include <map>

typedef std::vector<std::string> table;
typedef std::pair<table_type, long long> token;
typedef std::vector<token> tokens;

class settings {
private:
	std::map<table_type, table> _tables;

	tokens _tokens;
public:
	settings(const table& reserved, const table& separators) {
		_tables[RESERVED] = reserved;
		_tables[SEPARATORS] = separators;
		_tables[NUMBERS] = table();
		_tables[IDENTIFIERS] = table();
	}
	const table& get_table(table_type type) const {
		return _tables.at(type);
	}

	const table& get_reserved() const {
		return get_table(RESERVED);
	}
	const table& get_separators() const {
		return get_table(SEPARATORS);
	}
	table& get_identifiers() {
		return _tables.at(IDENTIFIERS);
	}
	table& get_numbers() {
		return _tables.at(NUMBERS);
	}
	tokens& get_tokens() {
		return _tokens;
	}
};