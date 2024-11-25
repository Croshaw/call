//
// Created by croshaw on 11/21/24.
//

#ifndef CALL_ERROR_HPP
#define CALL_ERROR_HPP

#include <string>

namespace call {
    enum error_type {
        unknown,
        redefinition,
        undeclared,
        expected
    };

    struct error {
        const error_type type;
        const std::string msg;
        const std::pair<size_t, size_t> pos;
        const size_t length;

        static error parse(error_type type, const std::string &msg, std::pair<size_t, size_t> pos, size_t length) {
            std::string str_pos = std::to_string(pos.first) + ":" + std::to_string(pos.second);
            std::string error;
            switch (type) {
                case unknown:
                    error = "Unknown type name '" + msg + "' at position " + str_pos;
                    break;
                case expected:
                    error = "At position " + str_pos + " is expected \"" + msg + "\"";
                    break;
                case undeclared:
                    error = "Use of undeclared identifier '" + msg + "' at position " + str_pos;
                    break;
                case redefinition:
                    error = "Redefinition of " + msg + " at position " + str_pos;
                    break;
            }
            return {type, error, pos, length};
        }
    };

} // call

#endif //CALL_ERROR_HPP
