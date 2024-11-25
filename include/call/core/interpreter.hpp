//
// Created by croshaw on 11/21/24.
//

#ifndef CALL_INTERPRETER_HPP
#define CALL_INTERPRETER_HPP

#include <map>
#include <string>
#include <vector>
#include <stack>
#include "number.hpp"
#include "action.hpp"

namespace call {

    class interpreter {
    private:
        std::map<std::string, number> &_variables;
        const std::vector<action> &_actions;
        std::stack<action> _stack;

        std::pair<number, number> get_pair();

    public:
        interpreter(const std::vector<action> &actions, std::map<std::string, number> &variables);

        void run();
    };

} // call

#endif //CALL_INTERPRETER_HPP
