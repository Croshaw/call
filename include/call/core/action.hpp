//
// Created by croshaw on 11/20/24.
//

#ifndef CALL_ACTION_HPP
#define CALL_ACTION_HPP

#include <string>
#include "number.hpp"

namespace call {
    enum action_type {
        OPERATOR,
        NUMBER,
        ID,
        ADDRESS,
        OTHER
    };

    class action {
    private:
        std::string _name;
        action_type _type;
        number _num_value;
    public:
        action(const char *name, action_type type) : _name(name), _type(type),
                                                     _num_value(type == NUMBER ? number::parse(name) : 0) {
            if (type == NUMBER) {
                _name = "_";
            }
        }

        action(const std::string &name, action_type type) : _name(name), _type(type),
                                                            _num_value(type == NUMBER ? number::parse(name) : 0) {
            if (type == NUMBER) {
                _name = "_";
            }
        }

        action(number value, action_type type) : _name("_"), _type(type), _num_value(value) {}

        std::string get_name() const {
            return _name;
        }

        action_type get_type() const {
            return _type;
        }

        number get_number() const {
            return _num_value;
        }

        action &operator=(const action &other) {
            if (&other != this) {
                _name = other._name;
                _num_value = other._num_value;
                _type = other._type;
            }
            return *this;
        }
    };
}
#endif //CALL_ACTION_HPP
