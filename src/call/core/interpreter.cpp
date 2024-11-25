//
// Created by croshaw on 11/21/24.
//

#include <stack>
#include <iostream>
#include "call/core/interpreter.hpp"

namespace call {
    std::pair<number, number> interpreter::get_pair() {
        const auto &second = _stack.top();
        _stack.pop();
        const auto &first = _stack.top();
        _stack.pop();
        return std::make_pair(first.get_type() == ID ? _variables[first.get_name()] : first.get_number(),
                              second.get_type() == ID ? _variables[second.get_name()] : second.get_number());
    }

    interpreter::interpreter(const std::vector<action> &actions, std::map<std::string, number> &variables) : _actions(
            actions), _variables(variables) {}

    void interpreter::run() {
        for (size_t i = 0; i < _actions.size(); i++) {
            const auto &action = _actions.at(i);
            if (action.get_type() != action_type::OPERATOR && action.get_type() != action_type::OTHER) {
                _stack.push(action);
            } else {
                if (action.get_type() == OPERATOR) {
                    if (action.get_name() == "not") {
                        const auto &hz = _stack.top();
                        _stack.pop();
                        _stack.emplace(hz.get_number().bool_value() ? "false" : "true", NUMBER);
                    } else if (action.get_name() == "as") {
                        const auto &value = _stack.top();
                        _stack.pop();
                        const auto &var = _stack.top();
                        _stack.pop();
                        _variables[var.get_name()] = value.get_number();
                    } else if (action.get_name() == "+") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first + pair.second, NUMBER);
                    } else if (action.get_name() == "-") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first - pair.second, NUMBER);
                    } else if (action.get_name() == "or") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first || pair.second, NUMBER);
                    } else if (action.get_name() == "*") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first * pair.second, NUMBER);
                    } else if (action.get_name() == "/") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first / pair.second, NUMBER);
                    } else if (action.get_name() == "and") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first && pair.second, NUMBER);
                    } else if (action.get_name() == ">") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first > pair.second, NUMBER);
                    } else if (action.get_name() == "<") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first < pair.second, NUMBER);
                    } else if (action.get_name() == "=") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first == pair.second, NUMBER);
                    } else if (action.get_name() == "<>") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first != pair.second, NUMBER);
                    } else if (action.get_name() == ">=") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first >= pair.second, NUMBER);
                    } else if (action.get_name() == "<=") {
                        const auto &pair = get_pair();
                        _stack.emplace(pair.first <= pair.second, NUMBER);
                    }
                } else {
                    if (action.get_name() == "!J") {
                        i = _stack.top().get_number().int_value() - 1;
                        _stack.pop();
                    } else if (action.get_name() == "!JIF") {
                        int pos = _stack.top().get_number().int_value() - 1;
                        _stack.pop();
                        if (!_stack.top().get_number().bool_value()) {
                            i = pos;
                        }
                        _stack.pop();
                    } else if (action.get_name() == "!W") {
                        if (_stack.empty()) {
                            std::cout << std::endl;
                            return;
                        }
                        const auto &elem = _stack.top();
                        std::cout << (elem.get_type() == NUMBER ? elem.get_number().to_string()
                                                                : _variables[elem.get_name()].to_string()) << std::endl;
                        _stack.pop();
                    } else if (action.get_name() == "!R") {
                        while (!_stack.empty()) {
                            const auto &var = _stack.top();
                            _stack.pop();
                            float temp;
                            std::cin >> temp;
                            _variables[var.get_name()] = number(temp);
                        }
                    }
                }
            }
        }
    }


} // call