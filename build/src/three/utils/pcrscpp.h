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

#ifndef PCRSCPP_H_
#define PCRSCPP_H_

#include <pcre.h>

#include <string>
#include <vector>

namespace pcrscpp_error_codes {
enum error_code {
     // all good: zero value
     no_error = 0,
     // warnings: negative value
     warn_badref = -1,
     // errors: positive values
     err_pcre_compile = 1,
     err_pcre_study = 2,
     err_pcre_fullinfo = 3,
     err_cmdsyntax = 4,
     err_illegal_options_flag = 5,
     err_badrefsyntax = 6
};
}

namespace pcrscpp_templates {
template <typename pchar, class pstring, class replace_impl>
class replace {
public:
    pstring replace_result;
    // returns last error message,
    // or empty string, if no error happened
    std::string last_error_message();
    // free internal last error message string
    void free_error_message();
    // returns last replace count
    std::vector<size_t> last_replace_count();
    // free internal last replace count vector
    void free_counts_info();
    // add overloads for convenience sake
    pcrscpp_error_codes::error_code add_job
        (const pstring& command);
    pcrscpp_error_codes::error_code add_job
        (const pchar* command);
    pcrscpp_error_codes::error_code add_job
        (const pstring& pattern,
         const pstring& substitute,
         const pstring& options);
    pcrscpp_error_codes::error_code add_job
        (const pchar* pattern,
         const pchar* substitute,
         const pchar* options);
    // get number of jobs
    size_t jobs_n();
    // remove last job:
    // returns false if no job to pop
    bool pop_job_back();
    // remove first job
    bool pop_job_front();
    // remove job number n
    // (numbers start from 0):
    // returns false if no such job
    bool pop_job(size_t n);
    // remove all present (if any) jobs
    void remove_jobs();
    void replace_inplace (pstring&);
    void replace_store (const pstring&);
    void replace_store (const pchar*);
    pstring replace_copy (const pstring&);
    pstring replace_copy (const pchar*);
    // default constructor
    replace ();
    // destructor
    ~replace ();
    // following constructors throw errors codes
    // on failure,
    // even in case of a warning (warn_badref)
    replace (const pstring& command);
    replace (const pchar* command);
    replace (const pstring& pattern,
               const pstring& substitute,
               const pstring& options);
    replace (const pchar* pattern,
               const pchar* substitute,
               const pchar* options);

private:
    replace_impl* internal; // Opaque pointer
};
}

namespace pcrscpp {

// pcrs8 char
typedef char pchar;
// pcrs8 string
typedef std::basic_string<pchar> pstring;

// private class
class replace_impl;

// replace class
typedef pcrscpp_templates::replace<pchar, pstring, replace_impl> replace;
}

namespace pcrscpp16 {

// pcrs16 char:
// Different typedefs for at least C++11 and older,
// but we build respective template instantiation(s) for both
// char16_t and PCRE_UCHAR16 when building PCRSCPP with C++11 capable
// compiler, so that lib can be used with both C++11, and legacy code
#if __cplusplus >= 201103L
// C++11 or better
typedef char16_t pchar;
#else
typedef PCRE_UCHAR16 pchar;
#endif
// pcrs16 string
typedef std::basic_string<pchar> pstring;

// private class
class replace_impl;

// replace class
typedef pcrscpp_templates::replace<pchar, pstring, replace_impl> replace;
}

namespace pcrscpp32 {

// pcrs32 char
#if __cplusplus >= 201103L
typedef char32_t pchar;
#else
typedef PCRE_UCHAR32 pchar;
#endif
// pcrs32 string
typedef std::basic_string<pchar> pstring;

// private class
class replace_impl;

// replace class
typedef pcrscpp_templates::replace<pchar, pstring, replace_impl> replace;
}

#endif // PCRSCPP_H_
