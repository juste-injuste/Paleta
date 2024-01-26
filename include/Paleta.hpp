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

-----inclusion guard--------------------------------------------------------------------------------------------------*/
#ifndef PALETA_HPP
#define PALETA_HPP
#if __cplusplus >= 201103L
//---necessary libraries------------------------------------------------------------------------------------------------
#include <cstdint>  // for uint8_t
#include <ostream>  // for std::ostream
#include <iostream> // for std::cout, std::clog, std::cerr
//---Paleta-------------------------------------------------------------------------------------------------------------
namespace Paleta
{
  class Color;
  class Foreground;
  class Background;
  class Format;

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

# define PALETA_FORMAT(...)

  struct Clear;

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
  std::ostream& operator<<(std::ostream& ostream, Format format) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, Clear) noexcept;

  namespace Version
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
    struct RGB
    {
      inline
      RGB(uint8_t red, uint8_t green, uint8_t blue) noexcept;
      uint8_t r, g, b;
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

    enum
    {
      Default
    };

    enum
    {
      Keep
    };
    
    inline Color(Basic basic)       noexcept;
    inline Color(RGB   rgb)         noexcept;
    inline Color(decltype(Default)) noexcept;
    inline Color(decltype(Keep))    noexcept;
    inline Color()                  noexcept;
  private:
    enum class Type
    {
      Keep,
      Default,
      Basic,
      RGB
    } type;

    union
    {
      Basic basic;
      RGB   rgb = RGB(0, 0, 0);
    };

  friend class Format;
  friend std::ostream& operator<<(std::ostream&, Foreground) noexcept;
  friend std::ostream& operator<<(std::ostream&, Background) noexcept;
  };

  class Background
  {
  public:
    template<typename C>
    inline
    Background(C color) noexcept;

    Color color;
  };

  class Foreground
  {
  public:
    template<typename C>
    inline
    Foreground(C color) noexcept;

    Color color;
  };

  class Format
  {
  public:
    template<typename... F>
    inline
    Format(F... formats) noexcept;

    Foreground foreground = Color::Keep;
    Background background = Color::Keep;
    Weight     weight     = Weight::Keep;
    Italic     italic     = Italic::Keep;
    Strike     strike     = Strike::Keep;
    Underline  underline  = Underline::Keep;
    Overline   overline   = Overline::Keep;

    static inline
    Format reset() noexcept;
  private:
    template<typename F, typename... F_>
    inline void _format_dispatch(F format, F_... remaining_formats) noexcept;
    inline void _format_dispatch() noexcept;
    inline void _format(Foreground    color)     noexcept;
    inline void _format(Background    color)     noexcept;
    inline void _format(Weight        weight)    noexcept;
    inline void _format(Italic        italic)    noexcept;
    inline void _format(Strike        strike)    noexcept;
    inline void _format(Underline     underline) noexcept;
    inline void _format(Overline      overline)  noexcept;
    inline void _format(const Format& format)    noexcept;
  };

# undef  PALETA_FORMAT
# define PALETA_FORMAT(...)       \
    []{                           \
      using namespace Paleta;     \
      return Format(__VA_ARGS__); \
    }()

  struct Clear final
  {};
//---------------------------------------------------------------------------------------------------------------------- 
  Color::RGB::RGB(uint8_t red, uint8_t green, uint8_t blue) noexcept :
    r(red),
    g(green),
    b(blue)
  {}

  Color::Color() noexcept :
    type(Type::Keep)
  {}

  Color::Color(decltype(Keep)) noexcept :
    type(Type::Keep)
  {}

  Color::Color(decltype(Default)) noexcept :
    type(Type::Default)
  {}

  Color::Color(Basic basic_) noexcept :
    type(Type::Basic),
    basic(basic_)
  {}

  Color::Color(RGB rgb_color) noexcept :
    type(Type::RGB),
    rgb(rgb_color)
  {}

  template<typename C>
  Background::Background(C color_) noexcept :
    color(color_)
  {}

  template<typename C>
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
  void Format::_format_dispatch(F format, F_... remaining_formats) noexcept
  {
    _format(format);
    _format_dispatch(remaining_formats...);
  }
  
  void Format::_format_dispatch() noexcept
  {}

  void Format::_format(Background background_) noexcept
  {
    background = background_;
  }

  void Format::_format(Foreground foreground_) noexcept
  {
    foreground = foreground_;
  }

  void Format::_format(Weight weight_) noexcept
  {
    weight = weight_;
  }

  void Format::_format(Italic italic_) noexcept
  {
    italic = italic_;
  }

  void Format::_format(Strike strike_) noexcept
  {
    strike = strike_;
  }

  void Format::_format(Underline underline_) noexcept
  {
    underline = underline_;
  }

  void Format::_format(Overline overline_) noexcept
  {
    overline = overline_;
  }

  void Format::_format(const Format& format) noexcept
  {
    if (format.foreground.color.type != Color::Type::Keep)
    {
      foreground = format.foreground;
    }
    
    if (format.background.color.type != Color::Type::Keep)
    {
      background = format.background;
    }
    
    if (format.weight != Weight::Keep)
    {
      weight = format.weight;
    }
    
    if (format.italic != Italic::Keep)
    {
      italic = format.italic;
    }
    
    if (format.strike != Strike::Keep)
    {
      strike = format.strike;
    }
    
    if (format.underline != Underline::Keep)
    {
      underline = format.underline;
    }
    
    if (format.overline != Overline::Keep)
    {
      overline = format.overline;
    }
  }

  std::ostream& operator<<(std::ostream& ostream, Foreground foreground) noexcept
  {
    switch(foreground.color.type)
    {
      case Color::Type::Default:
        ostream << "\033[39m";
        break;
      case Color::Type::Basic:
        ostream << "\033[";
        ostream << static_cast<unsigned>(foreground.color.basic) << "m";
        break;
      case Color::Type::RGB:
        ostream << "\033[38;2;";
        ostream << static_cast<unsigned>(foreground.color.rgb.r) << ";";
        ostream << static_cast<unsigned>(foreground.color.rgb.g) << ";";
        ostream << static_cast<unsigned>(foreground.color.rgb.b) << "m";
        break;
      default:
        break;
    }

    return ostream;
  };

  std::ostream& operator<<(std::ostream& ostream, Background background) noexcept
  {
    switch(background.color.type)
    {
      case Color::Type::Default:
        ostream << "\033[49m";
        break;
      case Color::Type::Basic:
        ostream << "\033[";
        ostream << static_cast<unsigned>(background.color.basic) + 10 << "m";
        break;
      case Color::Type::RGB:
        ostream << "\033[48;2;";
        ostream << static_cast<unsigned>(background.color.rgb.r) << ";";
        ostream << static_cast<unsigned>(background.color.rgb.g) << ";";
        ostream << static_cast<unsigned>(background.color.rgb.b) << "m";
        break;
      default:
        break;
    }

    return ostream;
  };

  std::ostream& operator<<(std::ostream& ostream, Weight weight) noexcept
  {
    switch(weight)
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
    switch(italic)
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
    switch(strike)
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
    switch(underline)
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
    switch(overline)
    {
      case Overline::None:
        return ostream << "\033[55m";
      case Overline::Single:
        return ostream << "\033[53m";
      default:
        return ostream;
    }
  };

  std::ostream& operator<<(std::ostream& ostream, Format format) noexcept
  {
    ostream << format.foreground;
    ostream << format.background;
    ostream << format.weight;
    ostream << format.italic;
    ostream << format.strike;
    ostream << format.underline;
    ostream << format.overline;

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
