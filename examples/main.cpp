#include "Paleta.hpp"
#include <iostream>
#include <cstdlib>

int main()
{
  std::cout << Paleta::clear(); // clear terminal

  std::cout << "\033[m" << Paleta::Background(Paleta::Color::RGB(0, 50, 50));

  auto fmt1 = PALETA_FORMAT(Foreground(Color::RGB(125, 33, 200)), Italic::True,  Strike::False, Underline::Single);
  auto fmt2 = PALETA_FORMAT(Foreground(Color::Basic::Red),        Italic::Keep,  Strike::True,  Underline::None);
  auto fmt3 = PALETA_FORMAT(Foreground(Color::Default),           Italic::False, Strike::False, Underline::Double);

  std::cout << PALETA_FORMAT(Overline::Single);
  std::cout << "huh " << fmt1 << "test " << fmt2 << "test " << fmt3 << "test " << Paleta::Format::reset() << "test ";
  
  std::cout << "\033[0m";

  auto base_fmt = PALETA_FORMAT(Format::reset(), Foreground(Color::RGB(125, 33, 200)));
  std::cout << base_fmt << "\nfirst pass " << PALETA_FORMAT(Background(Color::Basic::Gray)) << "second pass";

  std::cout << Paleta::Format::reset();
}