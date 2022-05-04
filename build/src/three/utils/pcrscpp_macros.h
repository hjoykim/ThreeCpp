/***********************************************************************
    Copyright (C) 2017 Alex Potapenko

    This file is part of PCRSCPP.

    PCRSCPP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PCRSCPP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PCRSCPP.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

#ifndef PCRECPP_MACROS_H_INCLUDED
#define PCRECPP_MACROS_H_INCLUDED

/* Dark magic to have one source for
   8, 16 and 32 bits                 */

#if defined(PCRSCPP16) && defined(PCRSCPP32)
# error "Will not build both 16 and 32 bits"
#endif

#undef TOKENPASTE_2
#undef TOKENPASTE2
#undef TOKENPASTE3

#define TOKENPASTE_2(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE_2(x, y)
#define TOKENPASTE3(x, y, z) TOKENPASTE2(x, TOKENPASTE2(y, z) )

#undef pcrscpp_nullptr
#undef PCRSCPP_BS // bits suffix
#undef pcrscpp_conv_char
#undef _T_
#undef PCRSCPP_PCRE_UNICODE

#if __cplusplus >= 201103L // supports C++11 features
# define pcrscpp_nullptr nullptr

# ifdef PCRSCPP16
// pcre16
#  define PCRSCPP_BS 16
#  define _T_(a) TOKENPASTE2(u,a)
#  define PCRSCPP_PCRE_UNICODE PCRE_UTF16
#  define pcrscpp_conv_char PCRE_UCHAR16
# elif defined(PCRSCPP32)
// pcre32
#  define PCRSCPP_BS 32
#  define _T_(a) TOKENPASTE2(U,a)
#  define PCRSCPP_PCRE_UNICODE PCRE_UTF32
#  define pcrscpp_conv_char PCRE_UCHAR32
# else
// pcre 8 bit
#  define PCRSCPP_BS
#  define _T_(a) a
#  define PCRSCPP_PCRE_UNICODE PCRE_UTF8
# endif
#else // no/partial C++11 features
# define pcrscpp_nullptr NULL
# ifdef PCRSCPP16
// pcre16
#  define PCRSCPP_BS 16
#  define _T_(a) (PCRE_UCHAR16)(a)
#  define PCRSCPP_PCRE_UNICODE PCRE_UTF16
# elif defined(PCRSCPP32)
// pcre32
#  define PCRSCPP_BS 32
#  define _T_(a) (PCRE_UCHAR32)(a)
#  define PCRSCPP_PCRE_UNICODE PCRE_UTF32
# else // pcre 8 bit
#  define PCRSCPP_BS
#  define _T_(a) a
#  define PCRSCPP_PCRE_UNICODE PCRE_UTF8
# endif
#endif

// bit-specific PCRE types/functions
#undef pcrscpp_pcre
#undef pcrscpp_pcre_extra
#undef pcrscpp_pcre_exec
#undef pcrscpp_pcre_compile
#undef pcrscpp_pcre_study
#undef pcrscpp_pcre_fullinfo
#undef pcrscpp_pcre_get_stringnumber
#undef pcrscpp_namespace

#define pcrscpp_pcre TOKENPASTE2(pcre, PCRSCPP_BS)
#define pcrscpp_pcre_extra TOKENPASTE3(pcre,PCRSCPP_BS,_extra)
#define pcrscpp_pcre_exec TOKENPASTE3(pcre,PCRSCPP_BS,_exec)
#define pcrscpp_pcre_compile TOKENPASTE3(pcre,PCRSCPP_BS,_compile)
#define pcrscpp_pcre_study TOKENPASTE3(pcre,PCRSCPP_BS,_study)
#define pcrscpp_pcre_fullinfo TOKENPASTE3(pcre,PCRSCPP_BS,_fullinfo)
#define pcrscpp_pcre_get_stringnumber TOKENPASTE3(pcre,PCRSCPP_BS,_get_stringnumber)
#define pcrscpp_namespace TOKENPASTE2(pcrscpp,PCRSCPP_BS)

namespace pcrscpp_templates {

// Trigger template instantiation(s) compilation
#ifdef pcrscpp_conv_char
// std::u16string/std::u32string seamless support in C++11
# define pchar_impl pcrscpp_conv_char
# define pstring_impl std::basic_string<pcrscpp_conv_char>
template class replace<pchar_impl,
pstring_impl, pcrscpp_namespace::replace_impl>;
#else
# define pstring_impl pcrscpp_namespace::pstring
# define pchar_impl pcrscpp_namespace::pchar
#endif
template class replace<pcrscpp_namespace::pchar,
pcrscpp_namespace::pstring, pcrscpp_namespace::replace_impl>;

}

#endif // PCRECPP_MACROS_H_INCLUDED
