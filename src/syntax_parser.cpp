#include "syntax_parser.h"
#include <lexer.h>

void syntax_parser::read_next()
{
	_ind++;
	if (_ind == _settings.get_tokens().size())
		throw "ERROR";
	const auto token = _settings.get_tokens()[_ind];
	_table_type = token.first;
	_lexem = _settings.get_table(_table_type)[token.second];
}

bool syntax_parser::equal(const std::string& token)
{
	return _lexem == token;
}

bool syntax_parser::equal(table_type table_type)
{
	return _table_type == table_type;
}

void syntax_parser::PR()
{
	if (!equal("program"))
		throw "program";
	read_next();
	VARS();
	if (!equal(";"))
		throw ";";
	read_next();
	BODY();
}

void syntax_parser::VARS()
{
	if (!equal("var"))
		throw "var";
	read_next();
	do {
		DESCR();
	} while (equal(";"));
}

void syntax_parser::DESCR()
{
	if (equal("int") || equal("float") || equal("bool")) {
		read_next();
		ID();
	}
}

void syntax_parser::BODY()
{
	if (!equal("begin"));
	throw "begin";
	read_next();
	OPERBASE();
	if (!equal("end"));
	throw "end";
	read_next();
	if (!equal("."));
	throw ".";
}

void syntax_parser::OPERBASE()
{
	do {
		OPER();
		read_next();
	} while (equal(":"));
}

void syntax_parser::OPER()
{
	if (equal("[")) {
		read_next();
		OPERBASE();
		if (!equal("]"))
			throw "]";
	}
	else if (equal("if")) {
		read_next();
		//COMPARE
	}
	else if (equal("while")) {
		read_next();
		//COMPARE
	}
	else if (equal("for")) {
		read_next();
		//PR
	}
	else if (equal("read")) {
		read_next();
		if (!equal("("))
			throw "(";
		read_next();
		ID();
		if (!equal(")"))
			throw ")";
	}
	else if (equal("write")) {
		read_next();
		//EXPR
	}
	else {
		//PR;
	}
}

void syntax_parser::ID()
{
	do {
		if (!equal(IDENTIFIERS))
			throw "IDENTIFIERS";
		read_next();
	} while (equal(","));
}

syntax_parser::syntax_parser(settings settings) : _settings(settings) {
	_ind = 0;
}

void syntax_parser::parse(const std::string& source)
{
	lexer _lexer(_settings);
	//проверка, что лексер всё сделал заебись
	PR();
}
