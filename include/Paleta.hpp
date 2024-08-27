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
#include <cstdint>  // for uint_fast8_t
#include <ostream>  // for std::ostream
#include <iostream> // for std::cout, std::clog, std::cerr
//---Paleta-------------------------------------------------------------------------------------------------------------
namespace stz
{
inline namespace fmz
//----------------------------------------------------------------------------------------------------------------------
{
  struct Format;

# define terminal_format(...)

  enum { clear };
  enum { reset };

  enum class Colors : uint_fast8_t
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
    
  struct RGB final
  {
    uint_fast8_t r, g, b;
  };

  enum class Font : uint_fast8_t
  {
    Light,
    Regular,
    Bold,
    Light_Italic,
    Italic,
    Bold_Italic
  };

  enum class Underline : uint_fast8_t
  {
    None,
    Single,
    Double
  };

  enum class Strike : uint_fast8_t
  {
    None,
    Single
  };

  enum class Overline : uint_fast8_t
  {
    None,
    Single
  };

  struct Foreground;

  struct Background;

  namespace _version
  {
#   define FMZ_VERSION_MAJOR  000
#   define FMZ_VERSION_MINOR  000
#   define FMZ_VERSION_PATCH  000
#   define FMZ_VERSION_NUMBER ((FMZ_VERSION_MAJOR  * 1000 + FMZ_VERSION_MINOR) * 1000 + FMZ_VERSION_PATCH)

    constexpr long MAJOR  = FMZ_VERSION_MAJOR;
    constexpr long MINOR  = FMZ_VERSION_MINOR;
    constexpr long PATCH  = FMZ_VERSION_PATCH;
    constexpr long NUMBER = FMZ_VERSION_NUMBER;
  }
//----------------------------------------------------------------------------------------------------------------------
  namespace _impl
  {
# if defined(__clang__)
#   define _stz_impl_PRAGMA(PRAGMA) _Pragma(#PRAGMA)
#   define _stz_impl_CLANG_IGNORE(WARNING, ...)          \
      _stz_impl_PRAGMA(clang diagnostic push)            \
      _stz_impl_PRAGMA(clang diagnostic ignored WARNING) \
      __VA_ARGS__                                        \
      _stz_impl_PRAGMA(clang diagnostic pop)
#endif

// support from clang 12.0.0 and GCC 10.1 onward
# if defined(__clang__) and (__clang_major__ >= 12)
# if __cplusplus < 202002L
#   define _stz_impl_LIKELY   _stz_impl_CLANG_IGNORE("-Wc++20-extensions", [[likely]])
# else
#   define _stz_impl_LIKELY   [[likely]]
# endif
# elif defined(__GNUC__) and (__GNUC__ >= 10)
#   define _stz_impl_LIKELY   [[likely]]
# else
#   define _stz_impl_LIKELY
# endif

// support from clang 3.9.0 and GCC 4.7.3 onward
# if defined(__clang__)
#   define _stz_impl_EXPECTED(CONDITION) (__builtin_expect(static_cast<bool>(CONDITION), 1)) _stz_impl_LIKELY
# elif defined(__GNUC__)
#   define _stz_impl_EXPECTED(CONDITION) (__builtin_expect(static_cast<bool>(CONDITION), 1)) _stz_impl_LIKELY
# else
#   define _stz_impl_EXPECTED(CONDITION) (CONDITION) _stz_impl_LIKELY
# endif

    struct _color final
    {
      constexpr
      _color(const size_t red_, const size_t green_, const size_t blue_) noexcept :
        _type(_color_type::RGB),
        _data(RGB{
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

      constexpr
      _color(decltype(reset)) noexcept :
        _type(_color_type::DEFAULT)
      {}

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
        constexpr _color_data(RGB            rgb) noexcept : _rgb(rgb)           {}
        constexpr _color_data(                  ) noexcept : _rgb{0, 0, 0}       {}
        Colors _basic;
        RGB    _rgb;
      } _data;
    };

    template<typename T1, typename T2>
    constexpr
    auto _cvt(T2 data) -> typename std::enable_if<std::is_same<T1, T2>::value == true, T1>::type
    {
      return data;
    }

    template<typename T1, typename T2>
    constexpr
    auto _cvt(T2 data) -> typename std::enable_if<std::is_same<T1, T2>::value != true, T1>::type
    {
      return T1();
    }

    template<typename type>
    constexpr
    type _get()
    {
      return type();
    }

    template<typename type, typename T, typename... types>
    constexpr
    type _get(T data_, types... data_n_)
    {
      return std::is_same<type, T>::value ? _cvt<type>(data_) : _get<type>(data_n_...);
    }

    struct _backdoor;
  }
//----------------------------------------------------------------------------------------------------------------------
  struct Background
  {
    template<typename... C>
    inline constexpr
    Background(C... color) noexcept;
    
  private:
    friend _impl::_backdoor;
    _impl::_color _color;
  };

  struct Foreground
  {
    template<typename... C>
    inline constexpr
    Foreground(C... color) noexcept;
    
  private:
    friend _impl::_backdoor;
    _impl::_color _color;
  };
//----------------------------------------------------------------------------------------------------------------------
  struct Format
  {
    template<typename... F>
    inline constexpr
    Format(F... formats) noexcept;

  private:
    friend _impl::_backdoor;
    const Foreground _foreground = _impl::_color();
    const Background _background = _impl::_color();
    const Font       _font       = static_cast<Font>(static_cast<uint_fast8_t>(-1));
    const Underline  _underline  = static_cast<Underline>(static_cast<uint_fast8_t>(-1));
    const Strike     _strike     = static_cast<Strike>(static_cast<uint_fast8_t>(-1));
    const Overline   _overline   = static_cast<Overline>(static_cast<uint_fast8_t>(-1));
  };
//----------------------------------------------------------------------------------------------------------------------
# undef terminal_format
  inline constexpr
  auto terminal_format(const Format& format_) noexcept -> Format
  {
    return format_;
  }

# define terminal_format(...)                \
    terminal_format([]() -> stz::fmz::Format \
    {                                        \
      using namespace stz::fmz;              \
      return Format(__VA_ARGS__);            \
    }())
//----------------------------------------------------------------------------------------------------------------------
  namespace _impl
  {
#define _stz_impl_MAKE_TYPE_BACKDOOR(INTO, TYPE) using TYPE = INTO::TYPE;
#define _stz_impl_MAKE_DATA_BACKDOOR(INTO, DATA)                                           \
    static constexpr                                                                       \
    auto DATA(const INTO& into_) noexcept -> std::remove_const<decltype(INTO::DATA)>::type \
    {                                                                                      \
      return into_.DATA;                                                                   \
    }

    struct _backdoor
    {
      _stz_impl_MAKE_DATA_BACKDOOR(Format, _foreground)
      _stz_impl_MAKE_DATA_BACKDOOR(Format, _background)
      _stz_impl_MAKE_DATA_BACKDOOR(Format, _font)
      _stz_impl_MAKE_DATA_BACKDOOR(Format, _underline)
      _stz_impl_MAKE_DATA_BACKDOOR(Format, _strike)
      _stz_impl_MAKE_DATA_BACKDOOR(Format, _overline)

      _stz_impl_MAKE_DATA_BACKDOOR(Foreground, _color)

      _stz_impl_MAKE_DATA_BACKDOOR(Background, _color)
    };
  }
//----------------------------------------------------------------------------------------------------------------------
  template<typename... C>
  constexpr
  Background::Background(C... color_) noexcept :
    _color(color_...)
  {}
  
  template<typename... C>
  constexpr
  Foreground::Foreground(C... color_) noexcept :
    _color(color_...)
  {}
  
  template<typename... F>
  constexpr
  Format::Format(F... formats) noexcept :
    _foreground(_impl::_get<Foreground>(formats...)),
    _background(_impl::_get<Background>(formats...)),
    _font(_impl::_get<Font>(formats...)),
    _underline(_impl::_get<Underline>(formats...)),
    _strike(_impl::_get<Strike>(formats...)),
    _overline(_impl::_get<Overline>(formats...))
  {}

  inline
  std::ostream& operator<<(std::ostream& ostream_, const Format& format_) noexcept
  {
    const auto& fg_color = _impl::_backdoor::_color(_impl::_backdoor::_foreground(format_));
    switch (fg_color._type)
    {
    case _impl::_color::_color_type::DEFAULT:
      ostream_ << "\033[39m";
      break;
    case _impl::_color::_color_type::BASIC:
      ostream_ << "\033[" << static_cast<unsigned>(fg_color._data._basic) << "m";
      break;
    case _impl::_color::_color_type::RGB:
      ostream_ << "\033[38;2;";
      ostream_ << static_cast<unsigned>(fg_color._data._rgb.r) << ";";
      ostream_ << static_cast<unsigned>(fg_color._data._rgb.g) << ";";
      ostream_ << static_cast<unsigned>(fg_color._data._rgb.b) << "m";
      break;
    default:
      break;
    }

    const auto& bg_color = _impl::_backdoor::_color(_impl::_backdoor::_background(format_));
    switch (bg_color._type)
    {
    case _impl::_color::_color_type::DEFAULT:
      ostream_ << "\033[49m";
      break;
    case _impl::_color::_color_type::BASIC:
      ostream_ << "\033[" << (static_cast<unsigned>(bg_color._data._basic) + 10) << "m";
      break;
    case _impl::_color::_color_type::RGB:
      ostream_ << "\033[48;2;";
      ostream_ << static_cast<unsigned>(bg_color._data._rgb.r) << ";";
      ostream_ << static_cast<unsigned>(bg_color._data._rgb.g) << ";";
      ostream_ << static_cast<unsigned>(bg_color._data._rgb.b) << "m";
      break;
    default:
      break;
    }

    switch (_impl::_backdoor::_font(format_))
    {
    case Font::Light:
      ostream_ << "\033[2m\033[23m";
      break;
    case Font::Regular:
      ostream_ << "\033[22m\033[23m";
      break;
    case Font::Bold:
      ostream_ << "\033[1m\033[23m";
      break;
    case Font::Light_Italic:
      ostream_ << "\033[2m\033[3m";
      break;
    case Font::Italic:
      ostream_ << "\033[22m\033[3m";
      break;
    case Font::Bold_Italic:
      ostream_ << "\033[1m\033[3m";
      break;
    default:
      break;
    }
    
    switch (_impl::_backdoor::_strike(format_))
    {
    case Strike::Single:
      ostream_ << "\033[9m";
      break;
    case Strike::None:
      ostream_ << "\033[29m";
      break;
    default:
      break;
    }

    switch (_impl::_backdoor::_underline(format_))
    {
    case Underline::None:
      ostream_ << "\033[24m";
      break;
    case Underline::Single:
      ostream_ << "\033[4m";
      break;
    case Underline::Double:
      ostream_ << "\033[21m";
      break;
    default:
      break;
    }

    switch (_impl::_backdoor::_overline(format_))
    {
    case Overline::None:
      ostream_ << "\033[55m";
      break;
    case Overline::Single:
      ostream_ << "\033[53m";
      break;
    default:
      break;
    }

    return ostream_;
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const decltype(reset)) noexcept
  {
    return ostream_ << "\033[0m";
  };

  inline
  std::ostream& operator<<(std::ostream& ostream_, const decltype(clear)) noexcept
  {
    if _stz_impl_EXPECTED(
      (ostream_.rdbuf() == std::cout.rdbuf()) ||
      (ostream_.rdbuf() == std::cerr.rdbuf()) ||
      (ostream_.rdbuf() == std::clog.rdbuf()))
    {
      ostream_ << "\033[H\033[J";
    }

    return ostream_;
  }
}
}
//----------------------------------------------------------------------------------------------------------------------
#undef _stz_impl_PRAGMA
#undef _stz_impl_CLANG_IGNORE
#undef _stz_impl_LIKELY
#undef _stz_impl_EXPECTED
#undef _stz_impl_MAKE_TYPE_BACKDOOR
#undef _stz_impl_MAKE_DATA_BACKDOOR
#undef _stz_impl_MAKE_CONS_BACKDOOR
//----------------------------------------------------------------------------------------------------------------------
#else
#error "Paleta: Support for ISO C++11 is required"
#endif
#endif