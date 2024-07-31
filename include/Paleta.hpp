/*---author-------------------------------------------------------------------------------------------------------------

Justin Asselin (juste-injuste)
justin.asselin@usherbrooke.ca
https://github.com/juste-injuste/Paleta

-----licence------------------------------------------------------------------------------------------------------------
 
MIT License

Copyright (c) 2024 Justin Asselin (juste-injuste)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 
-----versions-----------------------------------------------------------------------------------------------------------

Version 0.1.0 - Initial release

-----description--------------------------------------------------------------------------------------------------------

fmz::Format;

-----inclusion guard--------------------------------------------------------------------------------------------------*/
#ifndef _paleta_hpp
#define _paleta_hpp
#if __cplusplus >= 201103L
//---necessary libraries------------------------------------------------------------------------------------------------
#include <cstdint>  // for uint8_t, uint_fast8_t
#include <ostream>  // for std::ostream
#include <iostream> // for std::cout, std::clog, std::cerr
//---Paleta-------------------------------------------------------------------------------------------------------------
namespace fmz
{
  class Format;

  template<typename Specifiers>
  auto make_format(Specifiers specifiers) -> Format;

  struct Clear;

  enum class Weight
  {
    Keep,
    Faint,
    Normal,
    Bold
  };

  enum class Italic
  {
    Keep,
    False,
    True
  };

  enum class Strike
  {
    Keep,
    False,
    True
  };

  enum class Underline
  {
    Keep,
    None,
    Single,
    Double
  };

  enum class Overline
  {
    Keep,
    None,
    Single
  };

  class Color;

  struct Foreground;

  struct Background;

  inline
  std::ostream& operator<<(std::ostream& ostream, Foreground foreground) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Background background) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Weight weight) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Italic italic) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Strike strike) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Underline underline) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Overline overline) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, const Format& format) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Clear) noexcept;

  namespace _version
  {
    constexpr long MAJOR  = 000;
    constexpr long MINOR  = 001;
    constexpr long PATCH  = 000;
    constexpr long NUMBER = (MAJOR * 1000 + MINOR) * 1000 + PATCH;
  }
//----------------------------------------------------------------------------------------------------------------------
  class Color
  {
  public:
    struct RGB final
    {
      inline constexpr
      RGB(uint8_t red, uint8_t green, uint8_t blue) noexcept;
      uint_fast8_t r, g, b;
    };

    enum class Basic
    {
      Black           = 30,
      Red             = 31,
      Green           = 32,
      Yellow          = 33,
      Blue            = 34,
      Magenta         = 35,
      Cyan            = 36,
      White           = 37,
      Gray            = 90,
      Bright_Black    = Gray,
      Bright_Red      = 91,
      Bright_Green    = 92,
      Bright_Yellow   = 93,
      Bright_Blue     = 94,
      Bright_Magenta  = 95,
      Bright_Cyan     = 96,
      Bright_White    = 97
    };

    enum { Default };

    enum { Keep };
    
    inline constexpr Color(Basic basic)       noexcept;
    inline constexpr Color(RGB   rgb)         noexcept;
    inline constexpr Color(decltype(Default)) noexcept;    
    inline constexpr Color(decltype(Keep))    noexcept;
    inline constexpr Color()                  noexcept;
  private:
    enum class _type_t
    {
      KEEP,
      DEFAULT,
      BASIC,
      RGB
    } _type;

    union _data_t
    {
      constexpr _data_t(Basic basic) noexcept : _basic(basic)      {}
      constexpr _data_t(RGB   rgb)   noexcept : _rgb(rgb)          {}
      constexpr _data_t()            noexcept : _rgb(RGB(0, 0, 0)) {}
      Basic _basic;
      RGB   _rgb;
    } _data;

    friend class Format;
    friend std::ostream& operator<<(std::ostream&, Foreground) noexcept;
    friend std::ostream& operator<<(std::ostream&, Background) noexcept;
  };

  struct Background
  {
    template<typename C>
    inline constexpr
    Background(C color) noexcept;

    Color color;
  };

  struct Foreground
  {
    template<typename C>
    inline constexpr
    Foreground(C color) noexcept;

    Color color;
  };

  class Format
  {
  public:
    template<typename... F>
    inline
    Format(F... formats) noexcept;

    static inline
    Format reset() noexcept;
  private:
    Foreground _foreground = Color::Keep;
    Background _background = Color::Keep;
    Weight     _weight     = Weight::Keep;
    Italic     _italic     = Italic::Keep;
    Strike     _strike     = Strike::Keep;
    Underline  _underline  = Underline::Keep;
    Overline   _overline   = Overline::Keep;

    template<typename F, typename... F_>
    inline void _format_dispatch(F format, F_... remaining_formats) noexcept;
    inline void _format_dispatch() noexcept {};
    inline void _format(Foreground    color)     noexcept;
    inline void _format(Background    color)     noexcept;
    inline void _format(Weight        weight)    noexcept;
    inline void _format(Italic        italic)    noexcept;
    inline void _format(Strike        strike)    noexcept;
    inline void _format(Underline     underline) noexcept;
    inline void _format(Overline      overline)  noexcept;
    inline void _format(const Format& format)    noexcept;
    friend std::ostream& operator<<(std::ostream&, const Format&) noexcept;
  };

  template<typename Specifiers>
  auto make_format(const Specifiers specifiers_) -> Format
  {
    return specifiers_();
  }

# define make_format(...)         \
    make_format([]{               \
      using namespace fmz;        \
      return Format(__VA_ARGS__); \
    })

  struct Clear final {};
//---------------------------------------------------------------------------------------------------------------------- 
  constexpr
  Color::RGB::RGB(uint8_t red, uint8_t green, uint8_t blue) noexcept :
    r(red),
    g(green),
    b(blue)
  {}

  constexpr
  Color::Color() noexcept :
    _type(_type_t::KEEP)
  {}

  constexpr
  Color::Color(decltype(Keep)) noexcept :
    _type(_type_t::KEEP)
  {}

  constexpr
  Color::Color(decltype(Default)) noexcept :
    _type(_type_t::DEFAULT)
  {}

  constexpr
  Color::Color(Basic basic) noexcept :
    _type(_type_t::BASIC),
    _data(basic)
  {}

  constexpr
  Color::Color(RGB rgb) noexcept :
    _type(_type_t::RGB),
    _data(rgb)
  {}

  template<typename C>
  constexpr
  Background::Background(C color_) noexcept :
    color(color_)
  {}
  
  template<typename C>
  constexpr
  Foreground::Foreground(C color_) noexcept :
    color(color_)
  {}
  
  template<typename... F>
  Format::Format(F... formats) noexcept
  {
    _format_dispatch(formats...);
  }

  Format Format::reset() noexcept
  {
    return Format(
      Foreground(Color::Default),
      Background(Color::Default),
      Weight::Normal,
      Italic::False,
      Strike::False,
      Underline::None,
      Overline::None);
  }

  template<typename F, typename... F_>
  void Format::_format_dispatch(F format_, F_... remaining_formats) noexcept
  {
    _format(format_);
    _format_dispatch(remaining_formats...);
  }

  void Format::_format(Background background_) noexcept
  {
    _background = background_;
  }

  void Format::_format(Foreground foreground_) noexcept
  {
    _foreground = foreground_;
  }

  void Format::_format(Weight weight_) noexcept
  {
    _weight = weight_;
  }

  void Format::_format(Italic italic_) noexcept
  {
    _italic = italic_;
  }

  void Format::_format(Strike strike_) noexcept
  {
    _strike = strike_;
  }

  void Format::_format(Underline underline_) noexcept
  {
    _underline = underline_;
  }

  void Format::_format(Overline overline_) noexcept
  {
    _overline = overline_;
  }

  void Format::_format(const Format& format) noexcept
  {
    if (format._foreground.color._type != Color::_type_t::KEEP)
    {
      _foreground = format._foreground;
    }
    
    if (format._background.color._type != Color::_type_t::KEEP)
    {
      _background = format._background;
    }
    
    if (format._weight != Weight::Keep)
    {
      _weight = format._weight;
    }
    
    if (format._italic != Italic::Keep)
    {
      _italic = format._italic;
    }
    
    if (format._strike != Strike::Keep)
    {
      _strike = format._strike;
    }
    
    if (format._underline != Underline::Keep)
    {
      _underline = format._underline;
    }
    
    if (format._overline != Overline::Keep)
    {
      _overline = format._overline;
    }
  }

  std::ostream& operator<<(std::ostream& ostream, Foreground foreground) noexcept
  {
    switch (foreground.color._type)
    {
    case Color::_type_t::DEFAULT:
      ostream << "\033[39m";
      break;
    case Color::_type_t::BASIC:
      ostream << "\033[";
      ostream << static_cast<unsigned>(foreground.color._data._basic) << "m";
      break;
    case Color::_type_t::RGB:
      ostream << "\033[38;2;";
      ostream << static_cast<unsigned>(foreground.color._data._rgb.r) << ";";
      ostream << static_cast<unsigned>(foreground.color._data._rgb.g) << ";";
      ostream << static_cast<unsigned>(foreground.color._data._rgb.b) << "m";
      break;
    default:
      break;
    }

    return ostream;
  };

  std::ostream& operator<<(std::ostream& ostream, Background background) noexcept
  {
    switch (background.color._type)
    {
    case Color::_type_t::DEFAULT:
      ostream << "\033[49m";
      break;
    case Color::_type_t::BASIC:
      ostream << "\033[";
      ostream << static_cast<unsigned>(background.color._data._basic) + 10 << "m";
      break;
    case Color::_type_t::RGB:
      ostream << "\033[48;2;";
      ostream << static_cast<unsigned>(background.color._data._rgb.r) << ";";
      ostream << static_cast<unsigned>(background.color._data._rgb.g) << ";";
      ostream << static_cast<unsigned>(background.color._data._rgb.b) << "m";
      break;
    default:
      break;
    }

    return ostream;
  };

  std::ostream& operator<<(std::ostream& ostream, Weight weight) noexcept
  {
    switch (weight)
    {
    case Weight::Faint:
      return ostream << "\033[2m";
    case Weight::Normal:
      return ostream << "\033[22m";
    case Weight::Bold:
      return ostream << "\033[1m";
    default:
      return ostream;
    }
  };

  std::ostream& operator<<(std::ostream& ostream, Italic italic) noexcept
  {
    switch (italic)
    {
    case Italic::True:
      return ostream << "\033[3m";
    case Italic::False:
      return ostream << "\033[23m";
    default:
      return ostream;
    }
  };

  std::ostream& operator<<(std::ostream& ostream, Strike strike) noexcept
  {
    switch (strike)
    {
    case Strike::True:
      return ostream << "\033[9m";
    case Strike::False:
      return ostream << "\033[29m";
    default:
      return ostream;
    }
  };

  std::ostream& operator<<(std::ostream& ostream, Underline underline) noexcept
  {
    switch (underline)
    {
    case Underline::None:
      return ostream << "\033[24m";
    case Underline::Single:
      return ostream << "\033[4m";
    case Underline::Double:
      return ostream << "\033[21m";
    default:
      return ostream;
    }
  };

  std::ostream& operator<<(std::ostream& ostream, Overline overline) noexcept
  {
    switch (overline)
    {
    case Overline::None:
      return ostream << "\033[55m";
    case Overline::Single:
      return ostream << "\033[53m";
    default:
      return ostream;
    }
  };

  std::ostream& operator<<(std::ostream& ostream, const Format& format) noexcept
  {
    ostream << format._foreground;
    ostream << format._background;
    ostream << format._weight;
    ostream << format._italic;
    ostream << format._strike;
    ostream << format._underline;
    ostream << format._overline;

    return ostream;
  };

  std::ostream& operator<<(std::ostream& ostream, Clear) noexcept
  {
    if ((ostream.rdbuf() == std::cout.rdbuf()) ||
        (ostream.rdbuf() == std::cerr.rdbuf()) ||
        (ostream.rdbuf() == std::clog.rdbuf()))
    {
      ostream << "\033[H\033[J";
    }

    return ostream;
  }
//----------------------------------------------------------------------------------------------------------------------
}
#else
#error "Paleta: Support for ISO C++11 is required"
#endif
#endif