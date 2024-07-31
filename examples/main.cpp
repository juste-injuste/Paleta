#include "Paleta.hpp"
#include "Chronometro.hpp"
#include <iostream>
#include <cstdlib>

int main()
{
  std::cout << fmz::Clear(); // clear terminal

  std::cout << fmz::Background(fmz::Color::RGB(0, 50, 50));

  auto fmt_1 = fmz::make_format(Foreground(Color::RGB(125, 33, 200)), Italic::True,  Strike::False, Underline::Single);
  auto fmt_2 = fmz::make_format(Foreground(Color::Basic::Red),        Italic::Keep,  Strike::True,  Underline::None);
  auto fmt_3 = fmz::make_format(Foreground(Color::Default),           Italic::False, Strike::False, Underline::Double);

  std::cout << fmz::make_format(Overline::Single);
  std::cout << "huh " << fmt_1 << "test " << fmt_2 << "test " << fmt_3 << "test " << fmz::Format::reset() << "test ";
  
  std::cout << "\033[0m";

  auto base_fmt = fmz::make_format(Format::reset(), Foreground(Color::RGB(125, 33, 200)));
  std::cout << base_fmt << "\nfirst pass " << fmz::make_format(Background(Color::Basic::Gray)) << "second pass";

  std::cout << fmz::Format::reset();

  std::cout << sizeof(fmz::Format);
}