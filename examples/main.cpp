#include "Paleta.hpp"
#include <iostream>
#include <cstdlib>

int main()
{
  std::cout << stz::clear; // clear terminal

  std::cout << stz::Background(0, 50, 50);
  
  // const auto fmt_1 = stz::terminal_format(Foreground(125, 33, 200), Strike::None,   Underline::Single, Font::Italic );
  // const auto fmt_2 = stz::terminal_format(Foreground(Colors::Red),  Strike::Single, Underline::None                 );
  // const auto fmt_3 = stz::terminal_format(Foreground(reset),        Strike::None,   Underline::Double, Font::Regular);
  
  constexpr auto fmt_1 = stz::Format(stz::Foreground(125, 33, 200),     stz::Strike::None,   stz::Underline::Single, stz::Font::Italic );
  constexpr auto fmt_2 = stz::Format(stz::Foreground(stz::Colors::Red), stz::Strike::Single, stz::Underline::None                      );
  constexpr auto fmt_3 = stz::Format(stz::Foreground(stz::reset),       stz::Strike::None,   stz::Underline::Double, stz::Font::Regular);

  std::cout << stz::Format(stz::Overline::Single);
  std::cout << "huh " << fmt_1 << "test " << fmt_2 << "test " << fmt_3 << "test " << stz::reset << "test ";

  std::cout << stz::reset;

  auto base_fmt = stz::Format(stz::reset, stz::Foreground(125, 33, 200));
  std::cout << base_fmt << "\nfirst pass " << stz::Format(stz::Background(stz::Colors::Gray)) << "second pass";

  std::cout << stz::reset;
}