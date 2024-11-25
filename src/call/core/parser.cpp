//
// Created by croshaw on 11/20/24.
//

#include <stdexcept>
#include <stack>
#include "call/core/parser.hpp"

namespace call {
    void parser::read() {
        const auto &token = _tokens[_ind];
        _table = token.table;
        _lexem = _tables.get(_table).at(token.id);
    }

    void parser::read_next() {
        if (_ind < _tokens.size())
            _ind++;
        read();
    }

    void parser::read_prev() {
        if (_ind > 0)
            _ind--;
        read();
    }

    bool parser::equal(const std::string &lexem) {
        return _lexem == lexem;
    }

    bool parser::equal(table_type table) {
        return _table == table;
    }

    std::pair<size_t, size_t> parser::get_pos() {
        return _tokens[_ind].pos;
    }

    void parser::error(const std::string &msg, error_type type, std::pair<size_t, size_t> pos) {
        if (type == expected)
            read_prev();
        else if (type == undeclared && !hz)
            return;
        _errors.push_back(error::parse(type, msg, pos, msg.size()));
    }

    void parser::error(const std::string &msg, error_type type) {
        size_t offset = 1;
        if (type == expected) {
            offset += _lexem.size();
        }
        error(msg, type, std::make_pair(get_pos().first, get_pos().second + offset));
    }

    void parser::add_action(const std::string &value, action_type type) {
        _actions.emplace_back(value, type);
    }

    void parser::add_action(int value) {
        _actions.emplace_back(value, NUMBER);
    }

    void parser::add_action(action_type type) {
        add_action(_lexem, type);
    }

    void parser::insert_action(int value, action_type type, size_t pos) {
        _actions.insert(_actions.begin() + pos, action(value, type));
    }

    void parser::start() {
        if (!equal("program"))
            error("program", expected);
        read_next();
        vars();
        hz = true;
        if (!equal(";"))
            error(";", expected);
        read_next();
        body();
    }

    void parser::vars() {
        if (!equal("var"))
            error("var", expected);
        do {
            read_next();
            try {
                descr();
            }
            catch (const char *e) {
                if (!equal(";"))
                    read_prev();
                return;
            }
        } while (equal(";"));
    }

    void parser::descr() {
        if (equal("int") || equal("float") || equal("bool")) {
            std::string str_type = _lexem;
            value_type type = str_type == "int" ? INTEGER : str_type == "float" ? FLOAT : BOOL;
            if (!_def_types.insert(type).second)
                error(str_type, redefinition);
            size_t startInd = _ind;
            read_next();
            id_cycle();
            for (const auto &elem: _actions) {
                if (_variables.contains(elem.get_name()))
                    error(elem.get_name(), redefinition, _tokens[++startInd].pos);
                else
                    _variables.emplace(elem.get_name(), type);
//                if (!_variables.insert(elem.get_name()).second)
//                    err(elem.get_name(), redefinition, _tokens[++startInd].pos);
            }
            _actions.clear();
        } else
            throw "DESCR";
    }

    void parser::body() {
        if (!equal("begin"))
            error("begin", expected);
        read_next();
        oper_cycle({";"});
        if (!equal("end"))
            error("end", expected);
        read_next();
        if (!equal("."))
            error(".", expected);
    }

    void parser::oper_cycle(const std::unordered_set<std::string> &set) {
        oper();
        while (set.find(_lexem) != set.end()) {
            read_next();
            oper();
        }
    }

    void parser::oper() {
        if (equal("[")) {
            read_next();
            oper_cycle({":", "\n"});
            if (!equal("]"))
                error("]", expected);
            read_next();
        } else if (equal("if")) {
            read_next();
            compare();
            int jif_id = _actions.size();
            add_action("!JIF", OTHER);
            if (!equal("then"))
                error("then", expected);
            read_next();
            oper();
            if (equal("else")) {
                int j_id = _actions.size() + 1;
                add_action("!J", OTHER);
                insert_action(_actions.size() + 2, NUMBER, jif_id);
                read_next();
                oper();
                insert_action(_actions.size() + 1, NUMBER, j_id);
            } else {
                insert_action(_actions.size() + 1, NUMBER, jif_id);
            }
        } else if (equal("while")) {
            read_next();
            int j_id = _actions.size();
            compare();
            int jif_id = _actions.size();
            add_action("!JIF", OTHER);
            if (!equal("do"))
                error("do", expected);
            read_next();
            oper();
            add_action(std::to_string(j_id), NUMBER);
            add_action("!J", OTHER);
            insert_action(_actions.size(), NUMBER, jif_id);
        } else if (equal("for")) {
            read_next();
            //J_TO
            size_t j_to = _actions.size();
            assign();
            if (!equal("to"))
                error("to", expected);
            read_next();
            compare();
            //JIF_FROM
            size_t jif_from = _actions.size();
            add_action("!JIF", OTHER);
            if (!equal("do"))
                error("do", expected);
            read_next();
            oper();
            //J_FROM
            add_action(j_to);
            add_action("!J", OTHER);
            //JIF_TO
            insert_action(_actions.size() + 1, NUMBER, jif_from);
        } else if (equal("read")) {
            read_next();
            if (!equal("("))
                error("(", expected);
            read_next();
            id_cycle();
            if (!equal(")"))
                error(")", expected);
            add_action("!R", OTHER);
            read_next();
        } else if (equal("write")) {
            read_next();
            if (!equal("("))
                error("(", expected);
            read_next();
            expr();
            if (!equal(")"))
                error(")", expected);
            add_action("!W", OTHER);
            read_next();
        } else {
            assign();
        }
    }

    void parser::id_cycle() {
        if (!equal(IDENTIFIERS))
            error("IDENTIFIER", expected);
        else if (!_variables.contains(_lexem))
            error(_lexem, undeclared);
        add_action(ID);
        read_next();
        while (equal(",")) {
            read_next();
            if (!equal(IDENTIFIERS))
                error("IDENTIFIER", expected);
            else if (!_variables.contains(_lexem))
                error(_lexem, undeclared);
            add_action(ID);
            read_next();
        }
    }

    void parser::compare() {
        add();
        if (equal("=") || equal("<>") || equal(">") || equal("<") || equal(">=") || equal("<=")) {
            std::string action = _lexem;
            read_next();
            compare();
            add_action(action, OPERATOR);
        }
    }

    void parser::add() {
        mult();
        if (equal("+") || equal("-") || equal("or")) {
            std::string action = _lexem;
            read_next();
            add();
            add_action(action, OPERATOR);
        }
    }

    void parser::mult() {
        fact();
        read_next();
        if (equal("*") || equal("/") || equal("and")) {
            std::string action = _lexem;
            read_next();
            mult();
            add_action(action, OPERATOR);
        }
    }

    void parser::fact() {
        if (equal("not")) {
            read_next();
            fact();
            add_action("not", OPERATOR);
            return;
        }
        if (equal(NUMBERS) || equal("true") || equal("false") || equal(IDENTIFIERS)) {
            if (equal(IDENTIFIERS)) {
                if (!_variables.contains(_lexem))
                    error(_lexem, undeclared);
                add_action(_lexem, action_type::ID);
            } else
                add_action(_lexem, NUMBER);
            return;
        }
        if (equal("(")) {
            read_next();
            compare();
            if (!equal(")"))
                error(")", expected);
            return;
        }
    }

    void parser::assign() {
        if (!equal(IDENTIFIERS))
            error("IDENTIFIER", expected);
        else if (!_variables.contains(_lexem))
            error(_lexem, undeclared);
        add_action(ADDRESS);
        read_next();
        if (!equal("as"))
            error("as", expected);
        read_next();
        compare();
        add_action("as", OPERATOR);
    }

    void parser::expr() {
        compare();
        while (equal(",")) {
            read_next();
            compare();
        }
    }

    parser::parser(settings &tables, const std::vector<token> &tokens, std::vector<action> &actions,
                   std::map<std::string, number> &variables, std::vector<call::error> &errors) : _tables(
            tables), _tokens(tokens), _actions(actions), _variables(variables), _errors(errors), hz(false) {}

    void parser::parse() {
        _actions.clear();
        _ind = 0;
        read();
        start();
    }

} // call