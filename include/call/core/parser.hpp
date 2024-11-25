//
// Created by croshaw on 11/20/24.
//

#ifndef CALL_PARSER_HPP
#define CALL_PARSER_HPP

#include <unordered_set>
#include "settings.hpp"
#include "action.hpp"
#include "error.hpp"

namespace call {
    class parser {
    private:
        table_type _table;
        std::string _lexem;
        size_t _ind;
        settings &_tables;
        const std::vector<token> &_tokens;
        std::vector<call::action> &_actions;
        std::map<std::string, number> &_variables;
        std::unordered_set<value_type> _def_types;
        std::vector<call::error> &_errors;
        bool hz;

        void read();

        void read_next();

        void read_prev();

        bool equal(const std::string &lexem);

        bool equal(table_type table);

        std::pair<size_t, size_t> get_pos();

        void error(const std::string &msg, error_type type, std::pair<size_t, size_t> pos);

        void error(const std::string &msg, error_type type);

        void add_action(const std::string &value, action_type type);

        void add_action(int value);

        void add_action(action_type type);

        void insert_action(int value, action_type type, size_t pos);

        void start();

        void vars();

        void descr();

        void body();

        void oper_cycle(const std::unordered_set<std::string> &set);

        void oper();

        void id_cycle();

        void compare();

        void add();

        void mult();

        void fact();

        void assign();

        void expr();

    public:
        parser(settings &tables, const std::vector<token> &tokens, std::vector<action> &actions,
               std::map<std::string, number> &variables,
               std::vector<call::error> &errors);

        void parse();
    };

} // call

#endif //CALL_PARSER_HPP
