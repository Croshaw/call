#pragma once
#include "state.h"
#include <string>
#include <vector>
#include "settings.hpp"

class lexer {
private:
  settings _settings;
  bool _num_end;
  bool _order_sign;
  std::string _lexem;
  char _ch;
  state _st;

  std::string _source;
  std::size_t _ind;
  std::size_t _size;

  void reset();
  void read();

  bool is_digit();
  bool is_alpha();
  bool is_sep();

  state SB_state();
  state SS_state();
  state SI_state();
  state SNBIN_state();
  state SNOCT_state();
  state SN_state();
  state SNHEX_state();
  state SND_state();
  state SNDO_state();

  state get_state();

  void create_token();

public:
  lexer(settings settings);
  void analyse(const std::string& source);
};