#ifndef CALL_LEXER_HPP
#define CALL_LEXER_HPP

#include <string>
#include "settings.hpp"
#include "error.hpp"

namespace call {
    enum lexer_state {
        begin,
        word,
        sep,
        bin,
        oct,
        num,
        hex,
        float_num,
        exp,
        err
    };

    class lexer {
    private:
        char _cur;
        std::string _lexem;
        lexer_state _state;
        const std::string &_source;
        size_t _index;
        std::pair<size_t, size_t> _pos;
        size_t _offset;
        settings &_settings;
        std::vector<token> &_tokens;
        std::vector<error> &_errors;
        bool _num_end;

        void read();

        void reset();

        void create_token();

        bool is_digit() const;

        bool is_alpha() const;

        bool is_sep() const;

        lexer_state num_state();

        lexer_state word_state();

        lexer_state sep_state();

        lexer_state get_state();

    public:
        lexer(settings &settings, std::vector<token> &tokens, const std::string &source,
              std::vector<error> &errors);


        void parse();
    };

} // call

#endif //CALL_LEXER_HPP
