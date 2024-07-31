#include "Paleta.hpp"
#include "Chronometro.hpp"
#include <iostream>
#include <cstdlib>

int main()
{
  std::cout << fmz::Clear(); // clear terminal

  std::cout << fmz::Background(0, 50, 50);

  const auto fmt_1 = fmz::terminal_format(Foreground(125, 33, 200), Strike::False, Underline::Single, Italic::True );
  const auto fmt_2 = fmz::terminal_format(Foreground(Colors::Red),  Strike::True,  Underline::None                 );
  const auto fmt_3 = fmz::terminal_format(Foreground(Reset()),      Strike::False, Underline::Double, Italic::False);

  std::cout << fmz::terminal_format(Overline::Single);
  std::cout << "huh " << fmt_1 << "test " << fmt_2 << "test " << fmt_3 << "test " << fmz::Reset() << "test ";

  std::cout << fmz::Reset();

  auto base_fmt = fmz::terminal_format(Reset(), Foreground(125, 33, 200));
  std::cout << base_fmt << "\nfirst pass " << fmz::terminal_format(Background(Colors::Gray)) << "second pass";

  std::cout << fmz::Reset();
}