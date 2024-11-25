//
// Created by croshaw on 11/20/24.
//

#ifndef CALL_NUMBER_HPP
#define CALL_NUMBER_HPP

#include <string>

namespace call {
    enum value_type {
        INTEGER,
        FLOAT,
        BOOL
    };

    struct number {
    private:
        float _value;
        const value_type _type;
    public:
        number() : _value(0), _type(INTEGER) {}

        number(value_type type) : _value(0), _type(type) {}

        number(int value) : _value(value), _type(INTEGER) {}

        number(float value) : _value(value), _type(FLOAT) {}

        number(bool value) : _value(value), _type(BOOL) {}

        value_type get_type() const {
            return _type;
        }

        int int_value() const {
            return _value;
        }

        float float_value() const {
            return _value;
        }

        bool bool_value() const {
            return _value != 0;
        }

        const std::string to_string() const {
            switch (_type) {
                case INTEGER:
                    return std::to_string(int_value());
                case FLOAT:
                    return std::to_string(float_value());
                case BOOL:
                    return bool_value() ? "true" : "false";
                default:
                    throw "Error while convert number to string";
            }
        }

        static number parse(const std::string &number) {
            if (number == "true")
                return true;
            if (number == "false")
                return false;
            char last = number[number.size() - 1];
            switch (last) {
                case 'h':
                    return std::stoi(number.substr(0, number.size() - 1), nullptr, 16);
                    break;
                case 'b':
                    return std::stoi(number.substr(0, number.size() - 1), nullptr, 2);
                    break;
                case 'o':
                    return std::stoi(number.substr(0, number.size() - 1), nullptr, 8);
                    break;
                case 'd':
                    return std::stoi(number.substr(0, number.size() - 1));

                default:
                    return std::stof(number);
            }
        }

        bool operator>(const number &other) const {
            return this->_value > other._value;
        }

        bool operator<(const number &other) const {
            return this->_value < other._value;
        }

        bool operator==(const number &other) const {
            return this->_value == other._value;
        }

        bool operator!=(const number &other) const {
            return this->_value != other._value;
        }

        bool operator>=(const number &other) const {
            return this->_value >= other._value;
        }

        bool operator<=(const number &other) const {
            return this->_value <= other._value;
        }

        number operator+(const number &other) const {
            value_type o_type = other._type;
            switch (_type) {
                case INTEGER:
                    return number((int) (int_value() +
                                         (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                  : other.bool_value())));
                case FLOAT:
                    return number(float_value() +
                                  (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                           : other.bool_value()));
                case BOOL:
                    return number((bool) (bool_value() +
                                          (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                   : other.bool_value())));
                default:
                    throw "Error while plus numbers";
            }
        }

        number operator-(const number &other) const {
            value_type o_type = other._type;
            switch (_type) {
                case INTEGER:
                    return number((int) (int_value() -
                                         (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                  : other.bool_value())));
                case FLOAT:
                    return number(float_value() -
                                  (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                           : other.bool_value()));
                case BOOL:
                    return number((bool) (bool_value() -
                                          (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                   : other.bool_value())));
                default:
                    throw "Error while minus numbers";
            }
        }

        number operator*(const number &other) const {
            value_type o_type = other._type;
            switch (_type) {
                case INTEGER:
                    return number((int) (int_value() *
                                         (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                  : other.bool_value())));
                case FLOAT:
                    return number(float_value() *
                                  (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                           : other.bool_value()));
                case BOOL:
                    return number((bool) (bool_value() *
                                          (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                   : other.bool_value())));
                default:
                    throw "Error while multiply numbers";
            }
        }

        number operator/(const number &other) const {
            value_type o_type = other._type;
            switch (_type) {
                case INTEGER:
                    return number((int) (int_value() /
                                         (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                  : other.bool_value())));
                case FLOAT:
                    return number(float_value() /
                                  (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                           : other.bool_value()));
                case BOOL:
                    return number((bool) (bool_value() /
                                          (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                   : other.bool_value())));
                default:
                    throw "Error while divide numbers";
            }
        }

        number operator||(const number &other) const {
            value_type o_type = other._type;
            switch (_type) {
                case INTEGER:
                    return number((int) (int_value() ||
                                         (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                  : other.bool_value())));
                case FLOAT:
                    return number((float) (float_value() ||
                                           (o_type == INTEGER ? other.int_value() : o_type == FLOAT
                                                                                    ? other.float_value()
                                                                                    : other.bool_value())));
                case BOOL:
                    return number((bool) (bool_value() ||
                                          (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                   : other.bool_value())));
                default:
                    throw "Error while or numbers";
            }
        }

        number operator&&(const number &other) const {
            value_type o_type = other._type;
            switch (_type) {
                case INTEGER:
                    return number((int) (int_value() &&
                                         (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                  : other.bool_value())));
                case FLOAT:
                    return number((float) (float_value() &&
                                           (o_type == INTEGER ? other.int_value() : o_type == FLOAT
                                                                                    ? other.float_value()
                                                                                    : other.bool_value())));
                case BOOL:
                    return number((bool) (bool_value() &&
                                          (o_type == INTEGER ? other.int_value() : o_type == FLOAT ? other.float_value()
                                                                                                   : other.bool_value())));
                default:
                    throw "Error while and numbers";
            }
        }

        number &operator=(const number &num) {
            switch (_type) {
                case (INTEGER):
                    _value = (int) num._value;
                    break;
                case (BOOL):
                    _value = (bool) num._value;
                    break;
                default:
                    _value = num._value;
                    break;
            }
            return *this;
        }

        number &operator=(int num) {
            _value = num;
            return *this;
        }
    };
};

#endif //CALL_NUMBER_HPP
