#include "lexer.h"
#include "utils.hpp"

void lexer::reset()
{
  _lexem.clear();
  _ind = 0;
  _size = _source.size();
  _st = state::SB;
  _order_sign = false;
  _num_end = false;
}

void lexer::read()
{
  if (_ind == _size)
  {
    create_token();
    _st = state::SE;
    return;
  }
  _ch = _source[_ind++];
}

bool lexer::is_digit() { return std::isdigit(_ch); }

bool lexer::is_alpha() { return std::isalpha(_ch); }

bool lexer::is_sep()
{
  return hufv::find(_settings.get_separators(), std::string(1, _ch)) != -1;
}

state lexer::SB_state()
{
  if (is_alpha())
    return state::SI;
  if (is_digit())
    return state::SN;
  if (is_sep())
    return state::SS;
  return state::SER;
}

state lexer::SS_state()
{
  if (hufv::find(_settings.get_separators(), _lexem + _ch) != -1)
    return SS;
  if (is_digit() && _lexem == ".")
    return SND;
  create_token();
  _st = SS;
  return SB_state();
}

state lexer::SI_state()
{
  if (is_alpha() || is_digit())
    return state::SI;
  if (is_sep())
    return state::SS;
  return state::SER;
}

state lexer::SNBIN_state()
{
  if (is_digit())
  {
    if (_num_end)
      return SNHEX;
    if (_ch <= '1')
      return SNBIN;
    if (_ch <= '8')
      return SNOCT;
    return SN;
  }
  if (is_alpha())
  {
    char lower = std::tolower(_ch);
    bool canHEX = lower >= 'a' && lower <= 'f';
    if (_num_end && canHEX)
    {
      return SNHEX;
    }
    switch (lower)
    {
    case 'b':
      _num_end = true;
      return SNBIN;
    case 'o':
      _num_end = true;
      return SNOCT;
    case 'd':
      _num_end = true;
      return SN;
    case 'h':
      _num_end = true;
      return SNHEX;
    case 'e':
      return SND;
    }
    if (canHEX)
      return SNHEX;
  }
  else if (_ch == '.' && !_num_end)
  {
    return SND;
  }
  else if (is_sep() && _num_end)
    return state::SS;
  return SER;
}

state lexer::SNOCT_state()
{
  if (_num_end)
    return SER;
  if (is_digit())
  {
    if (_ch <= '8')
      return SNOCT;
    return SN;
  }
  if (is_alpha())
  {
    char lower = std::tolower(_ch);
    bool canHEX = lower >= 'a' && lower <= 'f';
    switch (lower)
    {
    case 'o':
      _num_end = true;
      return SNOCT;
    case 'd':
      _num_end = true;
      return SN;
    case 'h':
      _num_end = true;
      return SNHEX;
    case 'e':
      return SND;
    }
    if (canHEX)
      return SNHEX;
  }
  else if (_ch == '.')
  {
    return SND;
  }
  else if (is_sep() && _num_end)
    return state::SS;
  return SER;
}

state lexer::SN_state()
{
  if (is_digit())
  {
    if (_num_end)
      return SNHEX;
    return SN;
  }
  if (is_alpha())
  {
    char lower = std::tolower(_ch);
    bool canHEX = lower >= 'a' && lower <= 'f';
    if (_num_end && canHEX)
    {
      return SNHEX;
    }
    switch (lower)
    {
    case 'd':
      _num_end = true;
      return SN;
    case 'h':
      _num_end = true;
      return SNHEX;
    case 'e':
      return SNDO;
    }
    if (canHEX)
      return SNHEX;
  }
  else if (is_sep())
  {
    if (_ch == '.' && !_num_end)
      return SND;
    return SS;
  }
  return SER;
}

state lexer::SNHEX_state()
{
  if (_num_end)
    return SER;
  if (is_digit())
  {
    return SNHEX;
  }
  if (is_alpha())
  {
    char lower = std::tolower(_ch);
    bool canHEX = lower >= 'a' && lower <= 'f';
    switch (lower)
    {
    case 'h':
      _num_end = true;
      return SNHEX;
    }
    if (canHEX)
      return SNHEX;
  }
  else if (is_sep() && _num_end)
    return SS;
  return SER;
}

state lexer::SND_state()
{
  if (is_digit())
    return SND;
  if (tolower(_ch) == 'e')
  {
    return SNDO;
  }
  return SER;
}

state lexer::SNDO_state()
{
  if (_order_sign)
  {
    if (is_digit())
      return SNDO;
  }
  else
  {
    if (is_digit())
    {
      _order_sign = true;
      _lexem += "+";
      return SNDO;
    }
    if (_ch == ' ')
    {
      _ch = '+';
    }
    if (_ch == '+' || _ch == '-')
    {
      _order_sign = true;
      return SNDO;
    }
  }
  return SER;
}

state lexer::get_state()
{
  if (is_sep() && _ch != '.' && _st != SS)
    return SS;
  switch (_st)
  {
  case SS:
    return SS_state();
  case SB:
    return SB_state();
  case SI:
    return SI_state();
  case SNBIN:
    return SNBIN_state();
  case SNOCT:
    return SNOCT_state();
  case SN:
    return SN_state();
  case SNHEX:
    return SNHEX_state();
  case SND:
    return SND_state();
  case SNDO:
    return SNDO_state();
  default:
    return SER;
  }
}

void lexer::create_token()
{
  if (_lexem.empty())
    return;
  if (_st == state::SER)
    throw "An error occurred during lexical analysis";
  table_type table_id = RESERVED;
  long long id = 0;
  switch (_st)
  {
  case SI:
    id = hufv::find(_settings.get_reserved(), _lexem);
    if (id == -1) {
      id = hufv::write(_settings.get_identifiers(), _lexem);
      table_id = IDENTIFIERS;
    }
    break;
  case SS:
    table_id = SEPARATORS;
    id = hufv::find(_settings.get_separators(), _lexem);
  default:
    table_id = NUMBERS;
    std::transform(_lexem.begin(), _lexem.end(), _lexem.begin(), ::tolower);
    if (_st == SN && !_num_end)
      _lexem += 'd';
    id = hufv::write(_settings.get_numbers(), _lexem);
  }
  if (id == -1)
    throw "An error occurred during lexical analysis";
  _settings.get_tokens().push_back({ table_id, id });
  _lexem.clear();
  _st = state::SB;
  _num_end = false;
  _order_sign = false;
}

lexer::lexer(settings settings) : _settings(settings) {}

void lexer::analyse(const std::string& source)
{
  _source = source;
  reset();
  while (true)
  {
    read();
    if (_st == state::SE)
      break;
    if (_ch == ' ' && (_st != SNDO || _order_sign))
    {
      create_token();
      continue;
    }
    state newState = get_state();
    if (newState != SND && _lexem != "." &&
      ((newState == SS && _st != SS) || (_st == SS && newState != SS)))
    {
      create_token();
    }
    _st = newState;
    if (newState == state::SER)
      break;
    _lexem += _ch;
  }
  std::string reverse = "";
  for (const auto& item : _settings.get_tokens())
  {
    long long id = item.second;
    switch (item.first)
    {
    case 0:
      reverse += _settings.get_reserved()[id];
      break;
    case 1:
      reverse += _settings.get_separators()[id];
      break;
    case 2:
      reverse += _settings.get_numbers()[id];
      break;
    case 3:
      reverse += _settings.get_identifiers()[id];
      break;
    default:
      break;
    }
    reverse += " ";
  }
  reset();
}