/*---author-------------------------------------------------------------------------------------------------------------

Justin Asselin (juste-injuste)
justin.asselin@usherbrooke.ca
https://github.com/juste-injuste/Paleta

-----licence------------------------------------------------------------------------------------------------------------
 
MIT License

Copyright (c) 2024 Justin Asselin (juste-injuste)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
-----versions-----------------------------------------------------------------------------------------------------------

-----description--------------------------------------------------------------------------------------------------------

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
  struct Format;

  template<typename Specifiers>
  auto terminal_format(Specifiers specifiers) -> Format;

  struct {} reset;

  struct {} clear;

  enum class Colors
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
    
  enum class Weight
  {
    Faint,
    Normal,
    Bold
  };

  enum class Italic
  {
    False,
    True
  };

  enum class Strike
  {
    None,
    Single
  };

  enum class Underline
  {
    None,
    Single,
    Double
  };

  enum class Overline
  {
    None,
    Single
  };

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
  std::ostream& operator<<(std::ostream& ostream, decltype(reset)) noexcept;

  inline
  std::ostream& operator<<(std::ostream& ostream, decltype(clear)) noexcept;

  namespace _version
  {
#   define FMZ_VERSION_MAJOR  000
#   define FMZ_VERSION_MINOR  000
#   define FMZ_VERSION_PATCH  000
#   define FMZ_VERSION_NUMBER ((FMZ_VERSION_MAJOR * 1000 + FMZ_VERSION_MINOR) * 1000 + FMZ_VERSION_PATCH)
    constexpr long MAJOR  = FMZ_VERSION_MAJOR;
    constexpr long MINOR  = FMZ_VERSION_MINOR;
    constexpr long PATCH  = FMZ_VERSION_PATCH;
    constexpr long NUMBER = FMZ_VERSION_NUMBER;
  }
//----------------------------------------------------------------------------------------------------------------------
  namespace _impl
  {
    struct _RGB final
    {
      uint_fast8_t r, g, b;
    };

    struct _color final
    {
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
      
      constexpr
      _color(const size_t red_, const size_t green_, const size_t blue_) noexcept :
        _type(_color_type::RGB),
        _data(_impl::_RGB{
          static_cast<uint_fast8_t>(red_),
          static_cast<uint_fast8_t>(green_),
          static_cast<uint_fast8_t>(blue_)
        })
      {}

      constexpr
      _color(const Colors basic_color_) noexcept :
        _type(_color_type::BASIC),
        _data(basic_color_)
      {}

      constexpr inline
      _color(decltype(reset)) noexcept;

      constexpr
      _color() noexcept :
        _type(_color_type::KEEP)
      {}

      enum class _color_type
      {
        KEEP,
        DEFAULT,
        BASIC,
        RGB
      } _type;

      union _color_data
      {
        constexpr _color_data(Colors basic_color) noexcept : _basic(basic_color) {}
        constexpr _color_data(_impl::_RGB    rgb) noexcept : _rgb(rgb)           {}
        constexpr _color_data()                   noexcept : _rgb{0, 0, 0}       {}
        Colors      _basic;
        _impl::_RGB _rgb;
      } _data;
    };

    struct _backdoor;
  }
//----------------------------------------------------------------------------------------------------------------------
  struct Background
  {
    template<typename... C>
    inline constexpr
    Background(C... color) noexcept;
      
    inline explicit
    operator const char*() const noexcept;

  private:
    friend _impl::_backdoor;
    _impl::_color _color;
  };

  struct Foreground
  {
    template<typename... C>
    inline constexpr
    Foreground(C... color) noexcept;
      
    inline explicit
    operator const char*() const noexcept;

  private:
    friend _impl::_backdoor;
    _impl::_color _color;
  };

  struct Format
  {
  private:
    template<typename... F>
    inline
    Format(F... formats) noexcept;
    Foreground _foreground = _impl::_color();
    Background _background = _impl::_color();
    Weight     _weight     = static_cast<Weight>(-1);
    Italic     _italic     = static_cast<Italic>(-1);
    Strike     _strike     = static_cast<Strike>(-1);
    Underline  _underline  = static_cast<Underline>(-1);
    Overline   _overline   = static_cast<Overline>(-1);

    template<typename F, typename... F_>
    inline void _format_dispatch(F format, F_... remaining_formats) noexcept;
    inline void _format_dispatch()                                  noexcept {};
    inline void _format(decltype(reset))                   noexcept;
    inline void _format(Foreground    color)     noexcept;
    inline void _format(Background    color)     noexcept;
    inline void _format(Weight        weight)    noexcept;
    inline void _format(Italic        italic)    noexcept;
    inline void _format(Strike        strike)    noexcept;
    inline void _format(Underline     underline) noexcept;
    inline void _format(Overline      overline)  noexcept;
    inline void _format(const Format& format)    noexcept;
    friend _impl::_backdoor;
  };

  template<typename Specifiers>
  auto terminal_format(const Specifiers specifiers_) -> Format
  {
    return specifiers_();
  }

# define terminal_format(...)                             \
    terminal_format([]() -> fmz::Format {                 \
      using namespace fmz;                                \
      return _impl::_backdoor::_make_Format(__VA_ARGS__); \
    })
//----------------------------------------------------------------------------------------------------------------------
  namespace _impl
  {
    constexpr
    _color::_color(decltype(reset)) noexcept :
      _type(_color_type::DEFAULT)
    {}

#define _fmz_impl_MAKE_TYPE_BACKDOOR(INTO, TYPE) using TYPE = INTO::TYPE;
#define _fmz_impl_MAKE_DATA_BACKDOOR(INTO, DATA) \
    static constexpr                             \
    auto DATA(const INTO& into_) noexcept        \
      -> decltype(INTO::DATA)                    \
    {                                            \
      return into_.DATA;                         \
    }

#define _fmz_impl_MAKE_CONS_BACKDOOR(INTO) \
    template<typename... A>                \
    static constexpr                       \
    INTO _make_##INTO(A... a) noexcept     \
    {                                      \
      return INTO(a...);                   \
    }

    struct _backdoor : public Format
    {
      _fmz_impl_MAKE_CONS_BACKDOOR(Format)

      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _foreground)
      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _background)
      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _weight)
      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _italic)
      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _strike)
      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _underline)
      _fmz_impl_MAKE_DATA_BACKDOOR(Format, _overline)

      _fmz_impl_MAKE_DATA_BACKDOOR(Foreground, _color)

      _fmz_impl_MAKE_DATA_BACKDOOR(Background, _color)
    };
  }
//----------------------------------------------------------------------------------------------------------------------
  template<typename... C>
  constexpr
  Background::Background(C... color_) noexcept :
    _color(color_...)
  {}
  
  Background::operator const char *() const noexcept
  {
    static char buffer[16];
    switch (_color._type)
    {
    case _impl::_color::_color_type::DEFAULT:
      return "\033[49m";
    case _impl::_color::_color_type::BASIC:
      std::sprintf(buffer, "\033[%dm", static_cast<int>(_color._data._basic) + 10);
      return buffer;
    case _impl::_color::_color_type::RGB:
      std::sprintf(buffer, "\033[48;2;%d;%d;%dm", _color._data._rgb.r, _color._data._rgb.g, _color._data._rgb.b);
      return buffer;
    default:
      return "";
    }
  }
  
  template<typename... C>
  constexpr
  Foreground::Foreground(C... color_) noexcept :
    _color(color_...)
  {}
  
  Foreground::operator const char *() const noexcept
  {
    static char buffer[16];
    switch (_color._type)
    {
    case _impl::_color::_color_type::DEFAULT:
      return "\033[39m";
    case _impl::_color::_color_type::BASIC:
      std::sprintf(buffer, "\033[%dm", static_cast<int>(_color._data._basic));
      return buffer;
    case _impl::_color::_color_type::RGB:
      std::sprintf(buffer, "\033[48;2;%d;%d;%dm", _color._data._rgb.r, _color._data._rgb.g, _color._data._rgb.b);
      return buffer;
    default:
      return "";
    }
  }

  template<typename... F>
  Format::Format(F... formats) noexcept
  {
    _format_dispatch(formats...);
  }

  template<typename F, typename... F_>
  void Format::_format_dispatch(F format_, F_... remaining_formats) noexcept
  {
    _format(format_);
    _format_dispatch(remaining_formats...);
  }

  void Format::_format(decltype(reset)) noexcept
  {
    _foreground = decltype(reset)();
    _background = decltype(reset)();
    _weight     = Weight::Normal;
    _italic     = Italic::False;
    _strike     = Strike::None;
    _underline  = Underline::None;
    _overline   = Overline::None;
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
    if (_impl::_backdoor::_color(format._foreground)._type != _impl::_color::_color_type::KEEP)
    {
      _foreground = format._foreground;
    }
    
    if (_impl::_backdoor::_color(format._background)._type != _impl::_color::_color_type::KEEP)
    {
      _background = format._background;
    }
    
    if (format._weight != static_cast<Weight>(-1))
    {
      _weight = format._weight;
    }
    
    if (format._italic != static_cast<Italic>(-1))
    {
      _italic = format._italic;
    }
    
    if (format._strike != static_cast<Strike>(-1))
    {
      _strike = format._strike;
    }
    
    if (format._underline != static_cast<Underline>(-1))
    {
      _underline = format._underline;
    }
    
    if (format._overline != static_cast<Overline>(-1))
    {
      _overline = format._overline;
    }
  }

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Foreground foreground_) noexcept
  {
    return ostream_ << static_cast<const char*>(foreground_);
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Background background_) noexcept
  {
    return ostream_ << static_cast<const char*>(background_);
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Weight weight_) noexcept
  {
    switch (weight_)
    {
    case Weight::Faint:
      return ostream_ << "\033[2m";
    case Weight::Normal:
      return ostream_ << "\033[22m";
    case Weight::Bold:
      return ostream_ << "\033[1m";
    default:
      return ostream_;
    }
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Italic italic_) noexcept
  {
    switch (italic_)
    {
    case Italic::True:
      return ostream_ << "\033[3m";
    case Italic::False:
      return ostream_ << "\033[23m";
    default:
      return ostream_;
    }
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Strike strike_) noexcept
  {
    switch (strike_)
    {
    case Strike::Single:
      return ostream_ << "\033[9m";
    case Strike::None:
      return ostream_ << "\033[29m";
    default:
      return ostream_;
    }
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Underline underline_) noexcept
  {
    switch (underline_)
    {
    case Underline::None:
      return ostream_ << "\033[24m";
    case Underline::Single:
      return ostream_ << "\033[4m";
    case Underline::Double:
      return ostream_ << "\033[21m";
    default:
      return ostream_;
    }
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Overline overline_) noexcept
  {
    switch (overline_)
    {
    case Overline::None:
      return ostream_ << "\033[55m";
    case Overline::Single:
      return ostream_ << "\033[53m";
    default:
      return ostream_;
    }
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const decltype(reset)) noexcept
  {
    return ostream_ << "\033[0m";
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Format& format_) noexcept
  {
    ostream_ << _impl::_backdoor::_foreground(format_);
    ostream_ << _impl::_backdoor::_background(format_);
    ostream_ << _impl::_backdoor::_weight(format_);
    ostream_ << _impl::_backdoor::_italic(format_);
    ostream_ << _impl::_backdoor::_strike(format_);
    ostream_ << _impl::_backdoor::_underline(format_);
    ostream_ << _impl::_backdoor::_overline(format_);

    return ostream_;
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const decltype(clear)) noexcept
  {
    if ((ostream_.rdbuf() == std::cout.rdbuf()) ||
        (ostream_.rdbuf() == std::cerr.rdbuf()) ||
        (ostream_.rdbuf() == std::clog.rdbuf()))
    {
      ostream_ << "\033[H\033[J";
    }

    return ostream_;
  }
//----------------------------------------------------------------------------------------------------------------------
}
#else
#error "Paleta: Support for ISO C++11 is required"
#endif
#endif