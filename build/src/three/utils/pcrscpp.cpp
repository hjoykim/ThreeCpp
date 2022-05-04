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

#include <three/utils/pcrscpp.h>
#include <list>
#include <vector>
#include <string>
#include <sstream>

// bit-dependent defines:
// Triggers 8 bit by default, unless
// PCRSCPP16 or PCRSCPP32, which trigger
// 16 and 32 bit builds respectively
#include "pcrscpp_macros.h"

namespace pcrscpp_namespace { // evaluates to pcrscpp|pcrscpp16|pcrscpp32

namespace backref_types {
enum backref_type {
     backref_numerical, // $1, $2, ..., $n
     backref_named,     // $+{name}
     backref_all,       // $& and $0: refer to the whole match
     backref_btick,     // $`: subject before the match
     backref_tick,      // $': subject after the match
     backref_dummy,     // no backref
     backref_bad        // replacement backref, missing from the pattern:
                        // invalid number range, or wrong name
};
}

namespace replace_flags {
enum replace_flag {
    global = 1,
    trivial = 2,
    discard_non_matching = 4,
    multiline = 8,
    notempty = 16,
    success = 32
};
}

struct backref_t {
  pcrscpp_namespace::backref_types::backref_type type;
  int n;              // for backref_numerical
  pstring_impl name;       // for backref_named
};

// Plain text, backref series of the substitute
struct text_backref_pair {
    pstring_impl text;
    backref_t backref;
};

struct substitute_job {
    // PCRE pattern to replace
    pcrscpp_pcre* pattern;
    // PCRE hints
    pcrscpp_pcre_extra* hints;
    // replacement: consists of "plain text"-backreference pairs series
    std::vector<text_backref_pair> replacement;
    // replacement flags
    int flags;
    // backrefs hits: a number of occurrences of each
    // backref in the replace expression
    std::vector<int> br_hits;
    int br_all_hits, br_btick_hits, br_tick_hits; // hits for "$&"|"$0", "$`" and "$'",
                                                  // respectively
};

class replace_impl {
public:
    ~replace_impl();
    std::list<substitute_job> jobs;
    std::ostringstream error_stream;
    void reset_error_stream (const char[]);
    void reset_error_stream (const std::string&);
    void reset_error_stream ();
    std::vector<size_t> replace_count;
    pcrscpp_error_codes::error_code
        add_job_internal (const pstring_impl& pattern,
                          const pstring_impl& substitute,
                          const pstring_impl& options);
    pcrscpp_error_codes::error_code
        add_job_internal (const pstring_impl& command);
    void add_match (pstring_impl::size_type& n, const pstring_impl::size_type& plaintext_n,
                    const bool& discard, std::vector<int>& new_match, substitute_job& job,
                    const int* offsets, const int& offset,
                    const pstring_impl& source);
    void replace_inplace (pstring_impl& target);
private:
    bool parse_flags (const pstring_impl& optstring, int& pcre_flags, int& pcrs_flags);
    bool parse_backref (pstring_impl::const_iterator& start,
                         const pstring_impl::const_iterator& end, backref_t& output);
    bool
        parse_substitute (const pstring_impl& substitute, bool trivial_flag,
                     std::vector<text_backref_pair> &replacement);
    int pstring_impl_to_int (const pstring_impl& str);
};

// error stream reset methods
void replace_impl::reset_error_stream (const char str[]) {
#if __cplusplus >= 201103L
// C++11
    std::ostringstream(str).swap(error_stream);
#else
// older
    error_stream.str(std::string(str));
#endif
}

void replace_impl::reset_error_stream (const std::string& str) {
#if __cplusplus >= 201103L
// C++11
    std::ostringstream(str).swap(error_stream);
#else
// older
    error_stream.str(str);
#endif
}

void replace_impl::reset_error_stream () {
#if __cplusplus >= 201103L
// C++11
    std::ostringstream().swap(error_stream);
#else
// older
    error_stream.str(std::string());
#endif
}

// Simple tests for letters, digits,
// locale unaware
inline bool pcrscpp_isdigit (const pchar_impl c) {
    return (c >= _T_('0') && (c <= _T_('9')));
}

inline bool pcrscpp_isalpha (const pchar_impl c) {
    return ((c >= _T_('A')) && (c <= _T_('Z'))) ||
            ((c >= _T_('a')) && (c <= _T_('z')));
}

inline bool pcrscpp_isalnum (const pchar_impl c) {
    return pcrscpp_isalpha(c) || pcrscpp_isdigit(c);
}

int replace_impl::pstring_impl_to_int (const pstring_impl& str){
    std::string buff;
    // str passed here is expected to consist of digits only,
    // which are ASCII symbols, so we can narrow string blindly
    // (if not narrow already)
    buff.assign(str.begin(), str.end());
    int n;
#if __cplusplus >= 201103L
// C++11 way
    n = std::stoi(buff);
#else
// The old way: using string streams
    std::istringstream iss(buff);
    iss >> n;
#endif

    return n;
}

// Add match and grow size.
// Note: new_match is expected to be previously initialized
// with the right size
void replace_impl::add_match (pstring_impl::size_type& n, const pstring_impl::size_type& plaintext_n, const bool& discard,
                std::vector<int>& new_match, substitute_job& job,
                const int* offsets, const int& offset,
                const pstring_impl& source)
{
    n += plaintext_n                                    // plaintext replacement part size
      + (offsets[1] - offsets[0])*job.br_all_hits       // "$&"|"$0": all match
      + offsets[0]*job.br_btick_hits                    // "$`": source before offset[0]
      + std::distance(source.begin() + offsets[1],
                      source.end())*job.br_tick_hits;   // "$'": source from offset[1]
    if (!discard)
        n += offsets[0] - offset;                       // not matching part
    {
        bool even = false; // don't want these in outer scope
        int i = 0;
        for (std::vector<int>::iterator it = new_match.begin(), end = new_match.end();
             it != end; *it = offsets[i], ++it, ++i, even = !even)
        {
            // count size, skipping offsets[0] and offsets[1],
            // which are entire match offsets
            if ((i > 1) && (even)) // i is actually odd -- iteration number is even
                n += (offsets[i] - offsets[i-1])* // (i-3)/2 subpattern length
                      job.br_hits[(i-3)/2];       // (i-3)/2 subpattern hits
        }
    }
}

/******************************************************************************

Parse options flags string. Following flags are accepted:
 1) Perl-compatible flags
    g: global replace, not just the first match
    i: case insensitive match
       (PCRE_CASELESS)
    m: multi-line mode: '^' and '$' additionally match positions
       after and before newlines, respectively
       (PCRE_MULTILINE)
    s: let the scope of the '.' metacharacter include newlines
       (treat newlines as ordinary characters)
       (PCRE_DOTALL)
    x: allow extended regular expression syntax,
       enabling whitespace and comments in complex patterns
       (PCRE_EXTENDED)
 2) PHP-compatible flags
    A: "anchor" pattern: look only for "anchored" matches: ones that
       start with zero offset. In single-line mode is identical to
       prefixing all pattern alternative branches with "^"
       (PCRE_ANCHORED)
    D: treat dollar "$" as subject end assertion only, overriding the default:
       end, or immediately before a newline at the end.
       Ignored in multi-line mode
       (PCRE_DOLLAR_ENDONLY)
    U: invert '*' and '+' greediness logic: make ungreedy by default,
       '?' switches back to greedy. (?U) and (?-U) pattern switches
       remain unaffected
       (PCRE_UNGREEDY)
    u: Unicode mode. Treat pattern and subject as UTF8/UTF16/UTF32 string.
       Unlike in PHP, also affects newlines, '\R', '\d', '\w', etc. matching
       ((PCRE_UTF8/PCRE_UTF16/PCRE_UTF32)|
         PCRE_NEWLINE_ANY|PCRE_BSR_UNICODE|PCRE_UCP)
 3) PCRSCPP own flags:
    N: skip empty matches
       (PCRE_NOTEMPTY)
    T: treat substitute as a trivial string, i.e., make no backreference
       and escape sequences interpretation
    n: discard non-matching portions of the string to replace
       Note: PCRSCPP does _not_ automatically add newlines,
       the replacement result is plain concatenation of matches,
       be specifically aware of this in multiline mode

******************************************************************************/
bool replace_impl::parse_flags (const pstring_impl& optstring, int& pcre_flags,
                                int& pcrs_flags) {
    bool is_utf = false;
    pcre_flags = pcrs_flags = 0;
    for(pstring_impl::const_iterator it = optstring.begin(); it != optstring.end(); ++it)
        switch(*it) {
            case _T_('g'):
                pcrs_flags |= replace_flags::global;
                break;
            case _T_('i'):
                pcre_flags |= PCRE_CASELESS;
                break;
            case _T_('m'):
                pcre_flags |= PCRE_MULTILINE;
                pcrs_flags |= replace_flags::multiline;
                break;
            case _T_('n'):
                pcrs_flags |= replace_flags::discard_non_matching;
                break;
            case _T_('s'):
                pcre_flags |= PCRE_DOTALL;
                break;
            case _T_('u'): // Enable Unicode mode
                pcre_flags |= PCRE_NEWLINE_ANY      // Unicode newlines,
                           | PCRE_BSR_UNICODE       // '\R'
                           |  PCRE_UCP              // '\d', '\w', etc. matching
                           |  PCRSCPP_PCRE_UNICODE; // PCRE_UTF8/PCRE_UTF16/PCRE_UTF32
                is_utf = true;
                break;
            case _T_('x'):
                pcre_flags |= PCRE_EXTENDED;
                break;
            case _T_('A'):
                pcre_flags |= PCRE_ANCHORED;
                break;
            case _T_('D'):
                pcre_flags |= PCRE_DOLLAR_ENDONLY;
                break;
            case _T_('N'):
                pcrs_flags |= replace_flags::notempty;
                break;
            case _T_('U'):
                pcre_flags |= PCRE_UNGREEDY;
                break;
            case _T_('T'):
                pcrs_flags |= replace_flags::trivial;
                break;
            default:
                // illegal flag
                {
                    reset_error_stream();
                    error_stream << "Options parsing error: unknown flag on position "
                        << std::distance(optstring.begin(), it) + 1 << " in options string";
                }
                return false;
        }
    if (!is_utf)
        pcre_flags |= PCRE_NEWLINE_ANYCRLF | // treat any of '\r', '\n' or '\r\n'
                                             // as newlines
                      PCRE_BSR_ANYCRLF;      // + same for '\R' escape sequence matches
    return true;
}


bool replace_impl::parse_backref (pstring_impl::const_iterator& start,
                         const pstring_impl::const_iterator& end, backref_t& output) {
    long long length = std::distance(start, end);
    if ((length < 2) || (*start != _T_('$')))
        return false;
    switch(*(start + 1)) {
        case _T_('0'):
        case _T_('&'):
            output.type = backref_types::backref_all;
            // consumed "$0" or "$&": 2 characters,
            // set to last consumed: start + 1
            ++start;
            return true;
        case _T_('`'):
            output.type = backref_types::backref_btick;
            ++start;
            return true;
        case _T_('\''):
            output.type = backref_types::backref_tick;
            ++start;
            return true;
        default:
            break;
        }
    if (pcrscpp_isdigit(*(start + 1))) { // numerical_reference
        pstring_impl buff;
        buff.reserve(length);
        pstring_impl::const_iterator it;
        for (it = start + 1; (it != end) && (pcrscpp_isdigit(*it)); ++it)
            buff += *it;
        output.n = pstring_impl_to_int (buff);
        output.type = backref_types::backref_numerical;
        // set start iterator to last 'consumed' character
        start = it - 1;

        return true;
    }

    if ((length < 4) || (*(start + 1) != _T_('{')) ||
        !((pcrscpp_isalpha(*(start + 2)))||(*(start + 2) == _T_('_'))))
        // not a named reference too for sure,
        // so fail:
        // named references are accepted in format $+{name},
        // where name consists of up to 32 alphanumeric characters and
        // underscores, but must start with a non-digit
        // (PCRE named subpatterns requirement)
        return false;
    // can be a proper named subpattern:
    // trying to get name from expected "$+{name}"
    pstring_impl buff;
    buff.reserve(32); // mentioned max length
    buff.append(start + 2, start + 3);
    for (pstring_impl::const_iterator it = start + 3;
         (it != end) &&
         ((pcrscpp_isalnum(*it)) || (*it == _T_('_')) || (*it == _T_('}'))); ++it) {
        if (*it == _T_('}')) {
            // all good: name extracted
            output.type = backref_types::backref_named;
            output.name = buff;
            // last consumed character
            start = it;

            return true;
        }
        if (buff.length() == 32) // name too long
            return false;
        // collecting name characters
        buff += *it;
    }
    // if we got here, we didn't meet expected '}'
    return false;
}

/******************************************************************************

Pars substitute escape sequences and extract backreferences, unless trivial set

Following escape sequences get interpreted:
    \n: newline
    \r: carriage return
    \t: horizontal tab
    \f: form feed
    \b: backspace
    \a: alarm, bell
    \e: escape
    \0: binary zero

Backreferences syntax is similar to Perl's regex replace:
    $1 ... $n: nth capturing subpattern matched.
    $& and $0: the whole match
    $+{label}: labled subpattern matched. 'label' is up to 32 alphanumerical
               ('a'-'z','A'-'Z','0'-'9')|underscore ('_') characters,
               first character must be alphabetical
    $` and $': refer to the areas of the subject before and after the match,
     backtick: respectively. As in Perl, the unmodified subject is used, even
     and tick: if a global substitution previously matched.

******************************************************************************/

bool
replace_impl::parse_substitute (const pstring_impl& substitute, bool trivial_flag,
                      std::vector<text_backref_pair> &replacement) {
    // clear passed vector
    replacement.clear();
    if (trivial_flag) {
        replacement.push_back (text_backref_pair());
        std::vector<text_backref_pair>::iterator newly_added = replacement.begin();
        newly_added->text = substitute;
        newly_added->backref.type = backref_types::backref_dummy;

        return true;
    }

    // count required vector capacity
    bool escaped = false;
    std::vector<text_backref_pair>::size_type n = 1;
    for(pstring_impl::const_iterator it = substitute.begin(); it != substitute.end(); ++it) {
        if (escaped) {
            escaped = false;
            continue;
        }
        if (*it == _T_('$')) {
            ++n; // one more entry for each backref
            continue;
        }
        if (*it == _T_('\\'))
            escaped = true;
    }
    pstring_impl text;

    // allocate sufficient storage
    replacement.reserve(n);
    text.reserve(substitute.size());

    escaped = false;
    for(pstring_impl::const_iterator it = substitute.begin(); it != substitute.end(); ++it) {
        if (*it == _T_('\\')) {
            if (escaped) {
                text += *it; // *it == '\\'
                escaped = false;
            }
            else {
                escaped = true;
            }
            continue;
        }
        // *it != '\\'
        if (escaped) {
            escaped = false;
            switch(*it) {
                case _T_('t'):
                    text += (pchar_impl)(_T_('\t'));
                    break;
                case _T_('n'):
                    text += (pchar_impl)(_T_('\n'));
                    break;
                case _T_('r'):
                    text += (pchar_impl)(_T_('\r'));
                    break;
                case _T_('f'):
                    text += (pchar_impl)(_T_('\f'));
                    break;
                case _T_('a'):
                    text += (pchar_impl)(_T_('\a'));
                    break;
                case _T_('b'):
                    text += (pchar_impl)(_T_('\b'));
                    break;
                case _T_('e'):
                    text += (pchar_impl)(_T_('\e'));
                    break;
                case _T_('0'):
                    text += (pchar_impl)(_T_('\0'));
                    break;
                default:
                    text += *it;
                    break;
        }
        continue;
        }
        // *it != '\\', !escaped
        if (*it != _T_('$')) {
            text += *it;
            continue;
        }
        // backreference: *it != '\\', !escaped
        backref_t br;
        replacement.push_back (text_backref_pair());
        std::vector<text_backref_pair>::iterator newly_added = --(replacement.end());
        if (!(parse_backref (it, substitute.end(), newly_added->backref))) {
            // failed to extract backreference
            replacement.clear();
            reset_error_stream();
            error_stream << "Substitute parsing error: wrong backreference syntax on position "
                << std::distance(substitute.begin(), it) + 1 << " in substitute string";
            return false;
        }
        // backreference extracted successfully:
        // copy plaintext part,
        // and clear the buffer
        newly_added->text = text;
        text.clear();
    }
    if (!text.empty()) {
        // don't miss trailing text
        replacement.push_back (text_backref_pair());
        std::vector<text_backref_pair>::iterator newly_added = --(replacement.end());
        // A dummy backref means to backref after text
        newly_added->backref.type = backref_types::backref_dummy;
        newly_added->text = text;
    }

    return true;
}

/******************************************************************************

Add job by pattern, substitute and options strings

******************************************************************************/

pcrscpp_error_codes::error_code
replace_impl::add_job_internal (const pstring_impl& pattern, const pstring_impl& substitute,
                                const pstring_impl& options) {
    // allocate new job
    jobs.push_back (substitute_job());
    // iterator to access allocated job
    std::list<substitute_job>::iterator newjob = --(jobs.end());
    // parse flags
    int pcre_flags;
    if (!parse_flags (options, pcre_flags, newjob->flags)) {
        // Note: error message set by replace_impl::parse_flags
        return pcrscpp_error_codes::err_illegal_options_flag;
    }
    // parse_substitute
    if (!parse_substitute (substitute, (newjob->flags & replace_flags::trivial) != 0,
                     newjob->replacement)) {
        jobs.pop_back();

        // Note: error message set by replace_impl::parse_substitute
        return pcrscpp_error_codes::err_badrefsyntax;
    }
    // compile pattern
    const char* errptr;
    int erroroffset, error;
    if ((newjob->pattern =
        pcrscpp_pcre_compile (pattern.c_str(), pcre_flags, &errptr,
                        &erroroffset, pcrscpp_nullptr)) == pcrscpp_nullptr) {
        reset_error_stream();
        error_stream << "PCRE compile error on offset " << erroroffset << ": "
            << errptr;

        jobs.pop_back();
        return pcrscpp_error_codes::err_pcre_compile;
    }
    // compile hints
    newjob->hints = pcrscpp_pcre_study (newjob->pattern, 0, &errptr);
    if (errptr != pcrscpp_nullptr) {
	reset_error_stream();
        error_stream << "PCRE study error: " << errptr;
        free (newjob->pattern);

        jobs.pop_back();
        return pcrscpp_error_codes::err_pcre_study;
    }
    // backrefs count
    int count;
    if (0 != (error = pcrscpp_pcre_fullinfo(newjob->pattern, newjob->hints,
                                       PCRE_INFO_CAPTURECOUNT, &count)))
    {
        reset_error_stream("PCRE fullinfo error: ");
        switch (error) {
            case PCRE_ERROR_NULL:
                error_stream << "the argument(s) code|where was(were) NULL";
                break;
            case PCRE_ERROR_BADMAGIC:
                error_stream << "the \"magic number\" was not found";
                break;
            case PCRE_ERROR_BADENDIANNESS:
                error_stream << "the pattern was compiled with different endianness";
                break;
            case PCRE_ERROR_BADOPTION:
                error_stream << "the value of what was invalid";
                break;
            case PCRE_ERROR_UNSET:
                error_stream << "the requested field is not set";
                break;
            default:
                error_stream << "unknown error";
                break;
        }
        free (newjob->pattern);
        if (newjob->hints != pcrscpp_nullptr)
            free (newjob->hints);

        jobs.pop_back();
        return pcrscpp_error_codes::err_pcre_fullinfo;
    }

    // check backrefs in newjob->replacement for validity,
    // replace named subpatterns with numerical,
    // and count backrefs hits
    bool backref_error = false;
    newjob->br_hits.resize(count, 0);
    newjob->br_all_hits =
          newjob->br_btick_hits =
          newjob->br_tick_hits  = 0;
    for (std::vector<text_backref_pair>::iterator it = newjob->replacement.begin();
              it != newjob->replacement.end(); ++it) {
        int n;
        switch (it->backref.type) {
            case backref_types::backref_numerical:
                if ((it->backref.n > count) ||
                    (it->backref.n < 0)){
                    backref_error = true;
                    it->backref.type = backref_types::backref_bad;
                } else if (it->backref.n == 0)
                    it->backref.type = backref_types::backref_all;
                break;
            case backref_types::backref_named:
                n = pcrscpp_pcre_get_stringnumber(newjob->pattern,
                                            it->backref.name.c_str());
                if ((n > count) || (n <= 0)){
                    backref_error = true;
                    it->backref.type = backref_types::backref_bad;
                } else {
                    it->backref.n = n;
                    it->backref.type = backref_types::backref_numerical;
                }
                break;
            case backref_types::backref_all:
                ++(newjob->br_all_hits);
                break;
            case backref_types::backref_btick:
                ++(newjob->br_btick_hits);
                break;
            case backref_types::backref_tick:
                ++(newjob->br_tick_hits);
                break;
            case backref_types::backref_bad:
                // shouldn't happen here, but still
                backref_error = true;
                break;
            default:
                break;
        }
        // count hits
        if (it->backref.type == backref_types::backref_numerical)
            ++(newjob->br_hits[it->backref.n - 1]);  // we start from 0
        // subpattern name not used anymore,
        // so release buffer, if allocated
        it->backref.name.clear();
        it->backref.name.reserve(0);
    }
    if (backref_error) {
        reset_error_stream("Warning: invalid backreference(s) in replacement string");
        return pcrscpp_error_codes::warn_badref;
    }

    return pcrscpp_error_codes::no_error;
}

/******************************************************************************

Parse "s/pattern/substitute/options" to get separate respective strings,
and call add_job_internal(pattern,substitute,options).

Any character, except backslash ('\'), can be used as delimiter instead of '/',
as long as it's escaped in input pattern, substitute, options.

Will return pcrscpp_error_codes::err_cmdsyntax in 'command' input
before calling add_job_internal() in the following cases:
    * 'command' length less than 4 (must at least be "s///")
    *  first character is not 's'
    *  delimiter (second character) is backslash ('\')
    *  delimiter gets encountered != 3 times

******************************************************************************/

pcrscpp_error_codes::error_code replace_impl::add_job_internal (const pstring_impl& command)
{
    pstring_impl pattern, substitute, options;

    if (command.size() == 0) {
        reset_error_stream("Invalid command syntax: command cannot be empty");
        return pcrscpp_error_codes::err_cmdsyntax;
    }
    if (command[0] != _T_('s')) {
        reset_error_stream("Invalid command syntax [position 1]: command must start with 's'");
        return pcrscpp_error_codes::err_cmdsyntax;
    }
    if (command.size() == 1) {
        reset_error_stream("Invalid command syntax [position 1]: unexpeted command end");
        return pcrscpp_error_codes::err_cmdsyntax;
    }
    if (command[1] == _T_('\\')) {
        reset_error_stream("Invalid command syntax [position 2]: cannot use '\\' as a delimiter");
        return pcrscpp_error_codes::err_cmdsyntax;
    }
    pchar_impl delimiter = command[1];
    int del_count;
    bool escaped;
    pstring_impl::const_iterator copy_start = command.begin(), it;
    pstring_impl buff;
    for (it = command.begin(), del_count = 0, escaped = false; (it != command.end()) && (del_count < 4); ++it) {
        buff.clear();
        if (escaped) {
            if (*it == delimiter) { // append escaped delimiter
                buff += delimiter;
            } else { // append both backslash and character
                buff += (pchar_impl)_T_('\\');
                buff += *it;
            }
            escaped = false;
        }
        else {
            // not escaped
            if (*it == _T_('\\')) {
                escaped = true;
                continue;
            }
            if (*it == delimiter) {
                ++del_count;
                continue;
            }
            // not delimiter, not '\', not escaped
            buff += *it;
        }
        // where buff content should go
        switch (del_count) {
             case 1:
                pattern.append(buff);
                break;
             case 2:
                substitute.append(buff);
                break;
             case 3:
                options.append(buff);
                break;
             default:
                break;
        }
    }

    if (del_count < 3) {
        reset_error_stream();
        error_stream << "Invalid command syntax [position " << command.size()
                               << "]: unexpeted command end";
        return pcrscpp_error_codes::err_cmdsyntax;
    }
    if (del_count > 3) {
        reset_error_stream();
        error_stream << "Invalid command syntax [position " << std::distance(command.begin(), it) + 1
                               << "]: delimiter unexpected in 'options' substring";
        return pcrscpp_error_codes::err_cmdsyntax;
    }
    return add_job_internal (pattern, substitute, options);
}

/******************************************************************************

Execute replacement jobs on source 'target', passed by reference inplace,
and store replacements counts vector

******************************************************************************/

void replace_impl::replace_inplace (pstring_impl& target) {
    replace_count.clear();
    replace_count.reserve(jobs.size());
    pstring_impl source;
    pstring_impl::size_type n,             // to store resulting size
                       plaintext_n;   // plaintext part's of replace size
    int offset, count=0, flags;
    std::list<std::vector<int> > matches;
    for (std::list<substitute_job>::iterator job = jobs.begin();
         job != jobs.end(); ++job) {
        source.swap(target);                       // source is what target used to be ;)
        offset = 0;
        plaintext_n = n = flags = 0;
        count = job->br_hits.size();               // submatches/backrefs quantity
        matches.clear();                           // clear previous matches
        const int offsets_size = (count + 1) * 3;
        std::vector<int> offsets(offsets_size);
        bool discard = (job->flags &
                        pcrscpp_namespace::replace_flags::discard_non_matching) != 0,
             global = (job->flags &
                        pcrscpp_namespace::replace_flags::global) != 0,
             multiline = (job->flags &
                        pcrscpp_namespace::replace_flags::multiline) != 0,
             notempty = (job->flags &
                        pcrscpp_namespace::replace_flags::notempty) != 0;
        if (notempty)
            flags |= PCRE_NOTEMPTY;
        for (std::vector<text_backref_pair>::iterator it = job->replacement.begin(),
             end = job->replacement.end();
             it != end; plaintext_n += it->text.size(), ++it);
        while (pcrscpp_pcre_exec(job->pattern, job->hints, source.c_str(),
                                        (int)source.size(), offset, flags, &offsets[0],
                                        (count + 1) * 3) > 0)
        {
            // store matches and calculate size
            matches.push_back(std::vector<int>((count + 1) * 2, -1)); // match offsets are stored in first 2/3
            add_match (n, plaintext_n, discard, matches.back(), *job,
                       &offsets[0], offset, source);      // update size and add match

            if (!global)
                break;                                // global replace not requested
            offset = offsets[1];                      // new offset, after current match
            if (offsets[1] > offsets[0])
                continue;                             // non-empty match
            // empty match
            if ((pcrscpp_pcre_exec(job->pattern, job->hints, source.c_str(),
                        (int)source.size(), offset, PCRE_NOTEMPTY, &offsets[0],
                                        (count + 1) * 3) > 0) && (offset == offsets[0]))
            { // don't miss non-empty match on the same offset
                matches.push_back(std::vector<int>((count + 1) * 2, -1));
                add_match (n, plaintext_n, discard, matches.back(), *job,
                           &offsets[0], offset, source);
                offset = offsets[1];
                continue;
            }
            // there's no non-empty match on the same offset,
            // so advance 1 step, unless multiline and on "\r\n":
            // advance 2 steps in the latter case
            ++offset;
            if ((multiline) && (source.begin() + offset != source.end()) &&
                   (source[offset-1] == _T_('\r')) && (source[offset] == _T_('\n')))
                ++offset;                   // was on "\r\n" on empty match
        }
        if (matches.empty()){
            if (discard)
                target.clear();
            else
                target.swap(source);
            continue;
        }

        if (!discard)
            // substring left after last match
            n += distance(source.begin() + matches.back()[1], source.end());

        // Store matches count
        replace_count.push_back(matches.size());
        // now do the replacing.
        target.clear(); // clear content, capacity untouched
        // reserve more space if needed:
        target.reserve(n);
        // the actual write to target
       {// hide from outer scope
        pstring_impl::iterator prev_match_border = source.begin();
        for (std::list<std::vector<int> >::iterator match = matches.begin();
             match != matches.end(); ++match)
        {
            if (!discard) {
                // not matching part isn't being discarded
                target.append(prev_match_border,
                              source.begin() + (*match)[0]);     // not matching part
                prev_match_border = source.begin() + (*match)[1];
            }
            for (std::vector<text_backref_pair>::iterator
                 replacement = job->replacement.begin();
                 replacement != job->replacement.end();
                 ++replacement)
            {
                target.append(replacement->text);               // plaintext part
                int br_n;
                switch (replacement->backref.type) {
                    case pcrscpp_namespace::backref_types::backref_numerical:
                        br_n = replacement->backref.n;
                        // Note: 'br_n' subpattern offsets are:
                        // (*match)[br_n*2] to (*match)[br_n*2 + 1]
                        if ((*match)[br_n*2] >= 0) // skip non-found optional subpatterns
                            target.append(source.begin() + (*match)[br_n*2],
                                          source.begin() + (*match)[br_n*2 + 1]);
                        break;
                    case pcrscpp_namespace::backref_types::backref_all:
                        // all match
                        target.append(source.begin() + (*match)[0],
                                          source.begin() + (*match)[1]);
                        break;
                    case pcrscpp_namespace::backref_types::backref_btick:
                        // source up to match
                        target.append(source.begin(),
                                          source.begin() + (*match)[0]);
                    break;
                    case pcrscpp_namespace::backref_types::backref_tick:
                        // source after match
                        target.append(source.begin() + (*match)[1],
                                          source.end());
                        break;
                    case pcrscpp_namespace::backref_types::backref_bad:
                        // bad backrefs ignored
                    case pcrscpp_namespace::backref_types::backref_dummy:
                        // dummy ones too
                    case pcrscpp_namespace::backref_types::backref_named:
                        // named ones must have already been converted to numerical
                    default:
                        break;
                }
            }
        }}
        if (!discard)
            // substring left after last match
            target.append (source.begin() + matches.back()[1], source.end());
    }
}

replace_impl::~replace_impl() {
    for (std::list<substitute_job>::iterator it = jobs.begin(); it != jobs.end(); ++it) {
        if (it->pattern != pcrscpp_nullptr)
            free (it->pattern);
        if (it->hints != pcrscpp_nullptr)
            free (it->hints);
    }
}
}

namespace pcrscpp_templates {

template <typename pchar, class pstring, class replace_impl>
pcrscpp_error_codes::error_code replace<pchar, pstring, replace_impl>::add_job
(const pstring& pattern, const pstring& substitute, const pstring& options)
{
    if (internal == pcrscpp_nullptr)
        internal = new replace_impl(); // should never be needed unless messed up

    pstring_impl pattern_impl, substitute_impl, options_impl;

    pattern_impl.assign(pattern.begin(), pattern.end());
    substitute_impl.assign(substitute.begin(), substitute.end());
    options_impl.assign(options.begin(), options.end());

    return internal->add_job_internal(pattern_impl, substitute_impl, options_impl);
}

template <typename pchar, class pstring, class replace_impl>
pcrscpp_error_codes::error_code replace<pchar, pstring, replace_impl>::add_job
(const pchar* pattern, const pchar* substitute, const pchar* options)
{

    return add_job(pstring(pattern), pstring(substitute), pstring(options));
}

template <typename pchar, class pstring, class replace_impl>
pcrscpp_error_codes::error_code replace<pchar, pstring, replace_impl>::add_job (const pchar* command)
{
    return add_job((pstring)command);
}

template <typename pchar, class pstring, class replace_impl>
pcrscpp_error_codes::error_code replace<pchar, pstring, replace_impl>::add_job
(const pstring& command)
{
    if (internal == pcrscpp_nullptr)
        internal = new replace_impl(); // should never be needed unless messed up

    pstring_impl command_impl;

    command_impl.assign(command.begin(), command.end());

    return internal->add_job_internal(command_impl);
}

template <typename pchar, class pstring, class replace_impl>
size_t replace<pchar, pstring, replace_impl>::jobs_n() {
    if (internal == pcrscpp_nullptr) {
        return 0;
    }
    return internal->jobs.size();
}

template <typename pchar, class pstring, class replace_impl>
bool replace<pchar, pstring, replace_impl>::pop_job_back() {
    if ((internal == pcrscpp_nullptr) || (internal->jobs.size() == 0)) {
        return false;
    }
    internal->jobs.pop_back();
    return true;
}

template <typename pchar, class pstring, class replace_impl>
bool replace<pchar, pstring, replace_impl>::pop_job_front() {
    if ((internal == pcrscpp_nullptr) || (internal->jobs.size() == 0)) {
        return false;
    }
    internal->jobs.pop_front();
    return true;
}

template <typename pchar, class pstring, class replace_impl>
bool replace<pchar, pstring, replace_impl>::pop_job(size_t n) {
    if (internal == pcrscpp_nullptr) {
        return false;
    }

    std::list<pcrscpp_namespace::substitute_job>::iterator
               it = internal->jobs.begin(),
               end = internal->jobs.end();

    for (size_t i = 0; i < n; ++it) {
        if (it == end)
            return false;
    }
    internal->jobs.erase(it);

    return true;
}

template <typename pchar, class pstring, class replace_impl>
void replace<pchar, pstring, replace_impl>::remove_jobs() {
    if (internal != pcrscpp_nullptr) {
        for (std::list<pcrscpp_namespace::substitute_job>::iterator
               it = internal->jobs.begin();
               it != internal->jobs.end(); ++it) {
            if (it->pattern != pcrscpp_nullptr)
                free (it->pattern);
            if (it->hints != pcrscpp_nullptr)
                free (it->hints);
        }
        internal->jobs.clear();
    }
}

/******************************************************************************

Return a copy of last error message

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
std::string replace<pchar, pstring, replace_impl>::last_error_message() {
    if (internal != pcrscpp_nullptr)
        return internal->error_stream.str();
    else
        return std::string();
}

/******************************************************************************

Default constructor. Allocates internal class. Should never throw

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
replace<pchar, pstring, replace_impl>::replace() {
    internal = new replace_impl();
}

/******************************************************************************

Allocate internal class and add job by command. Throws on errors/warning

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
replace<pchar, pstring, replace_impl>::replace (const pstring& command) {
    internal = new replace_impl();
    pcrscpp_error_codes::error_code error;
    if ( (error = add_job (command)) != pcrscpp_error_codes::no_error)
            throw error;
}

template <typename pchar, class pstring, class replace_impl>
replace<pchar, pstring, replace_impl>::replace (const pchar* command)
{
    internal = new replace_impl();
    pcrscpp_error_codes::error_code error;
    if ( (error = add_job (command)) != pcrscpp_error_codes::no_error)
            throw error;
}

template <typename pchar, class pstring, class replace_impl>
replace<pchar, pstring, replace_impl>::replace
(const pstring& pattern, const pstring& substitute, const pstring& options)
{
    internal = new replace_impl();
    pcrscpp_error_codes::error_code error;
    if ( (error = add_job (pattern, substitute, options)) != pcrscpp_error_codes::no_error)
        throw error;
}

/******************************************************************************

Allocate internal class and add job by pattern, substitute and options strings.
Throws on errors/warning

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
replace<pchar, pstring, replace_impl>::replace
(const pchar* pattern, const pchar* substitute, const pchar* options)
{
    internal = new replace_impl();
    pcrscpp_error_codes::error_code error;
    if ( (error = add_job (pattern, substitute, options)) != pcrscpp_error_codes::no_error)
        throw error;
}

template <typename pchar, class pstring, class replace_impl>
replace<pchar, pstring, replace_impl>::~replace() {
    if (internal != pcrscpp_nullptr) {
        for (std::list<pcrscpp_namespace::substitute_job>::iterator
            it = internal->jobs.begin(); it != internal->jobs.end(); ++it) {
            if (it->pattern != pcrscpp_nullptr)
                free (it->pattern);
            if (it->hints != pcrscpp_nullptr)
                free (it->hints);
        }
        internal->jobs.clear();
        free (internal);
    }
}

/******************************************************************************

Execute replacement jobs on source, store result in public replace_result field

Calls replace_inplace internally

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
void replace<pchar, pstring, replace_impl>::replace_store (const pstring& source) {
    replace_result = source;
    replace_inplace (replace_result);
}

template <typename pchar, class pstring, class replace_impl>
void replace<pchar, pstring, replace_impl>::replace_store (const pchar* source) {
    replace_result = source;
    replace_inplace (replace_result);
}

/******************************************************************************

Execute replacement jobs on source, return result as copy

Calls replace_inplace internally

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
pstring replace<pchar, pstring, replace_impl>::replace_copy (const pstring& source) {
    pstring buff = source;
    replace_inplace (buff);

    return buff;
}

template <typename pchar, class pstring, class replace_impl>
pstring replace<pchar, pstring, replace_impl>::replace_copy (const pchar* source) {
    pstring buff = source;
    replace_inplace (buff);

    return buff;
}

/******************************************************************************

Return replace count vector copy. Vector size equals to executed job list size,
fields equal to replace counts for respective jobs

******************************************************************************/
template <typename pchar, class pstring, class replace_impl>
std::vector<size_t> replace<pchar, pstring, replace_impl>::last_replace_count() {
    if (internal == pcrscpp_nullptr)
        return std::vector<size_t>();

    return internal->replace_count;
}

/******************************************************************************

Execute replacement jobs on source 'target', passed by reference inplace,
and store replacements counts vector

******************************************************************************/

template <typename pchar, class pstring, class replace_impl>
void replace<pchar, pstring, replace_impl>::replace_inplace (pstring& target) {
    if (internal == pcrscpp_nullptr)
        return;

    // a dirty drick, but we don't want to copy values,
    // and pstring_impl can differ from pstring
    pstring_impl target_impl;
    target_impl.swap (*((pstring_impl*)&target));

    internal->replace_inplace(target_impl);

    target_impl.swap (*((pstring_impl*)&target));
}

template <typename pchar, class pstring, class replace_impl>
void replace<pchar, pstring, replace_impl>::free_error_message () {
    if (internal != pcrscpp_nullptr)
        internal->reset_error_stream();
}

template <typename pchar, class pstring, class replace_impl>
void replace<pchar, pstring, replace_impl>::free_counts_info () {
    if (internal != pcrscpp_nullptr)
        std::vector<size_t>().swap(internal->replace_count);
}
}
