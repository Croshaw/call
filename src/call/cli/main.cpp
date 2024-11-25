#include "call/core/settings.hpp"
#include "call/core/call.hpp"

int main() {
    call::settings settings(
            {"program", "var", "int", "float", "bool", "begin", "end", "if", "then", "else", "while", "do", "for", "to",
             "read", "write", "true", "false", "or", "and", "as", "not"},
            {".", ";", ",", ":", "(", ")", "[", "]", "+", "-", "*", "/", "=", ">", "<", "<=", ">=", "<>", "\n",});
    std::string source = "program var bool n; begin write(not n and false) end.";
    call::call lp(source, settings);
    lp.run();
}