//
// Created by croshaw on 11/20/24.
//

#ifndef CALL_CALL_HPP
#define CALL_CALL_HPP

#include <string>
#include <iostream>
#include "settings.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

namespace call {
    class call {
    private:
        const std::string &_source;
        settings &_settings;
        std::vector<token> _tokens;
        std::vector<action> _actions;
        std::map<std::string, number> _variables;
        std::vector<error> _errors;

        lexer _lexer;
        parser _parser;
        interpreter _interpreter;

        void print_errors() {
            for (const auto &error: _errors) {
                std::cerr << error.msg << std::endl;
            }
        }

        void print_table(table_type type, char delimiter = ' ') {
            const auto &table = _settings.get(type);
            for (const auto &elem: table) {
                std::cout << elem << delimiter;
            }
            std::cout << std::endl;
        }

        void print_actions() {
            int id = 0;
            for (const auto &elem: _actions) {
                std::cout << id++ << " : "
                          << (elem.get_type() == NUMBER ? elem.get_number().to_string() : elem.get_name()) << std::endl;
            }
        }

        void print_variables() {
            for (const auto &elem: _variables) {
                std::cout << elem.first << " " << elem.second.get_type() << " " << elem.second.to_string() << std::endl;
            }
        }

    public:
        call(const std::string &source, settings &settings) : _source(source), _settings(settings),
                                                              _lexer(_settings, _tokens, _source, _errors),
                                                              _parser(_settings, _tokens, _actions, _variables,
                                                                      _errors), _interpreter(_actions, _variables) {}

        void run() {
            _lexer.parse();
            _parser.parse();
            //print_actions();
            if (!_errors.empty()) {
                print_errors();
                return;
            }
            _interpreter.run();
        }

    };
}
#endif //CALL_CALL_HPP
