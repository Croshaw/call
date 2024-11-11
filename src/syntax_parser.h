#pragma once
#include <string>
#include <settings.hpp>
class syntax_parser
{
private:
	table_type _table_type;
	std::string &_lexem;
	std::size_t _ind;
	settings _settings;
	void read_next();
	bool equal(const std::string &token);
	bool equal(table_type table_type);
	void PR();
	void VARS();
	void DESCR();
	void BODY();
	void OPERBASE();
	void OPER();
	void ID();

public:
	syntax_parser(settings settings);
	void parse(const std::string &source);
};