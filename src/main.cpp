#include "lexer.h"
int main()
{
  lexer l(settings({ "program", "var", "int",  "float", "bool",  "begin", "end",     "if",  "then", "else",  "while", "do", "for",     "to",  "read", "write", "true",  "false", "or",      "and", "as",   "not" }, { ".", ";", ",", ":", "(", ")",  "[",  "]",  "+",  "-","*", "/", "=", ">", "<", "<=", ">=", "==", "\n", }));
  l.analyse("program var int k, n, sum; begin read(n); sum as 0.54; i as .321; while i <= n do [read(k): sum as sum+k: k as k+1.e54]; write(sum/n) end.");
}