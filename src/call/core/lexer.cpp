//
// Created by croshaw on 11/20/24.
//

#include "call/core/lexer.hpp"

namespace call {
    void lexer::read() {
        _cur = _source[_index];
        _index++;
    }

    void lexer::reset() {
        _lexem.clear();
        _state = begin;
        _num_end = false;
        _pos.second = _index - _offset;
    }

    void lexer::create_token() {
        if (_lexem.empty())
            return;
        if (_state == err || _state == exp)
            throw "An err occurred during lexical analysis";
        table_type table = _state == word ? IDENTIFIERS : (_state == sep ? SEPARATORS : NUMBERS);
        int id = -1;
        if (table == NUMBERS) {
            if (_state <= num && !_num_end) {
                _lexem += 'd';
                _num_end = true;
            }
            if (!_num_end && _state != float_num)
                throw "An err occurred during lexical analysis";
        }
        if (table == IDENTIFIERS) {
            id = _settings.find(RESERVED, _lexem);
            if (id != -1)
                table = RESERVED;
        }
        if (table != RESERVED) {
            id = _settings.write(table, _lexem);
        }
        _tokens.emplace_back(table, id, _pos);
        reset();
    }


    bool lexer::is_digit() const {
        return std::isdigit(_cur);
    }

    bool lexer::is_alpha() const {
        return std::isalpha(_cur);
    }

    bool lexer::is_sep() const {
        return _settings.find(SEPARATORS, std::string(1, _cur)) != -1;
    }

    lexer_state lexer::num_state() {
        if (is_digit()) {
            if (_state == exp) {
                if (!_num_end) {
                    _lexem += '+';
                    _num_end = true;
                }
                return float_num;
            }
            if (_cur <= '1' && (_state == bin || _state == begin)) {
                return bin;
            }
            if (_cur <= '7' && (_state == bin || _state == oct || _state == begin)) {
                return oct;
            }
            if (_cur <= '9' &&
                (_state == bin || _state == oct || _state == num || _state == begin)) {
                return num;
            }
            return _state;
        } else if (is_alpha()) {
            char lower = std::tolower(_cur);
            _cur = lower;
            if (_state == float_num && lower == 'e') {
                return _num_end ? err : exp;
            }
            if (_num_end) {
                if (_state == hex || _state == oct)
                    return err;
                if (lower >= 'a' && lower <= 'f') {
                    _num_end = false;
                    return hex;
                }
            }
            if (_state == hex && lower >= 'a' && lower <= 'f') {
                return hex;
            }
            switch (lower) {
                case 'o':
                    if (_state == oct) {
                        _num_end = true;
                        return oct;
                    }
                    return err;
                case 'b':
                    if (_state == bin) {
                        _num_end = true;
                        return bin;
                    }
                case 'd':
                    if (_state != hex) {
                        _num_end = true;
                        return num;
                    }
                case 'h': {
                    _num_end = true;
                    return hex;
                }
            }
            if (lower >= 'a' && lower <= 'f') {
                return hex;
            }
        } else if (_cur == '.' && !_num_end && _state <= num) {
            if (_state == begin)
                _lexem += '0';
            return float_num;
        } else if (_state == exp && (_cur == '+' || _cur == '-')) {
            _num_end = true;
            return exp;
        } else if (is_sep()) {
            create_token();
            return sep;
        }
        return err;
    }

    lexer_state lexer::word_state() {
        if (is_sep()) {
            create_token();
            return sep;
        }
        return word;
    }

    lexer_state lexer::sep_state() {
        if (is_digit()) {
            create_token();
            return num_state();
        }
        if (is_alpha()) {
            create_token();
            return word_state();
        }
        if (!_lexem.empty() && _settings.find(SEPARATORS, _lexem + _cur) == -1) {
            create_token();
        }
        return sep;

    }

    lexer_state lexer::get_state() {
        if (_state == begin) {
            if (is_digit() || _cur == '.')
                return num_state();
            if (is_alpha())
                return word_state();
            if (is_sep())
                return sep_state();
        }
        switch (_state) {
            case word:
                return word_state();
            case sep:
                return sep_state();
            default:
                return num_state();
        }
    }

    lexer::lexer(settings &settings, std::vector<token> &tokens, const std::string &source,
                 std::vector<error> &errors) : _settings(settings),
                                               _tokens(tokens),
                                               _source(source), _errors(errors) {
        _index = 0;
        _offset = 0;
        _state = begin;
        _num_end = false;
    }

    void lexer::parse() {
        _offset = 0;
        _settings.reset();
        _tokens.clear();
        reset();
        while (_index < _source.size()) {
            read();
            if (_cur == '\n') {
                _pos.first++;
                _offset = _index + 1;
            }
            if (_cur == ' ') {
                if (_state != err)
                    create_token();
                else {
                    _errors.push_back(error::parse(unknown, _lexem, _pos, _lexem.size()));
                    reset();
                }
                continue;
            }
            if (_state != err) {
                auto new_state = get_state();
                _state = new_state;
            }
            _lexem += _cur;
        }
        create_token();
    }
} // call