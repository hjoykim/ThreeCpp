# Install script for directory: E:/OpenGLWorkSpace/pcre-8.44

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/PCRE")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcred.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcre.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcre.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcre.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcred.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcre.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcre.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcre.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcreposixd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcreposix.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcreposix.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcreposix.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcreposixd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcreposix.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcreposix.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcreposix.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcrecppd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcrecpp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcrecpp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcrecpp.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcrecppd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcrecpp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcrecpp.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcrecpp.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcregrep.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcregrep.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcregrep.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcregrep.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcretest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcretest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcretest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcretest.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcrecpp_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcrecpp_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcrecpp_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcrecpp_unittest.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcre_scanner_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcre_scanner_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcre_scanner_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcre_scanner_unittest.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Debug/pcre_stringpiece_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/Release/pcre_stringpiece_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/MinSizeRel/pcre_stringpiece_unittest.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/OpenGLWorkSpace/pcre-8.44/build/RelWithDebInfo/pcre_stringpiece_unittest.exe")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "E:/OpenGLWorkSpace/pcre-8.44/build/pcre.h"
    "E:/OpenGLWorkSpace/pcre-8.44/pcreposix.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "E:/OpenGLWorkSpace/pcre-8.44/pcrecpp.h"
    "E:/OpenGLWorkSpace/pcre-8.44/pcre_scanner.h"
    "E:/OpenGLWorkSpace/pcre-8.44/build/pcrecpparg.h"
    "E:/OpenGLWorkSpace/pcre-8.44/build/pcre_stringpiece.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre-config.1"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcregrep.1"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcretest.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man3" TYPE FILE FILES
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre16.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre32.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_assign_jit_stack.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_compile.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_compile2.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_config.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_copy_named_substring.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_copy_substring.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_dfa_exec.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_exec.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_free_study.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_free_substring.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_free_substring_list.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_fullinfo.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_get_named_substring.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_get_stringnumber.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_get_stringtable_entries.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_get_substring.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_get_substring_list.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_jit_exec.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_jit_stack_alloc.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_jit_stack_free.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_maketables.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_pattern_to_host_byte_order.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_refcount.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_study.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_utf16_to_host_byte_order.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_utf32_to_host_byte_order.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcre_version.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcreapi.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrebuild.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrecallout.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrecompat.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrecpp.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcredemo.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrejit.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrelimits.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrematching.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrepartial.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrepattern.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcreperform.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcreposix.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcreprecompile.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcresample.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcrestack.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcresyntax.3"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/pcreunicode.3"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/pcre/html" TYPE FILE FILES
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/index.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre-config.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre16.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre32.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_assign_jit_stack.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_compile.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_compile2.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_config.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_copy_named_substring.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_copy_substring.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_dfa_exec.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_exec.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_free_study.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_free_substring.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_free_substring_list.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_fullinfo.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_get_named_substring.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_get_stringnumber.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_get_stringtable_entries.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_get_substring.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_get_substring_list.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_jit_exec.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_jit_stack_alloc.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_jit_stack_free.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_maketables.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_pattern_to_host_byte_order.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_refcount.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_study.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_utf16_to_host_byte_order.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_utf32_to_host_byte_order.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcre_version.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcreapi.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrebuild.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrecallout.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrecompat.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrecpp.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcredemo.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcregrep.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrejit.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrelimits.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrematching.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrepartial.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrepattern.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcreperform.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcreposix.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcreprecompile.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcresample.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcrestack.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcresyntax.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcretest.html"
    "E:/OpenGLWorkSpace/pcre-8.44/doc/html/pcreunicode.html"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES
      "E:/OpenGLWorkSpace/pcre-8.44/build/pcre.pdb"
      "E:/OpenGLWorkSpace/pcre-8.44/build/pcreposix.pdb"
      )
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES
      "E:/OpenGLWorkSpace/pcre-8.44/build/pcred.pdb"
      "E:/OpenGLWorkSpace/pcre-8.44/build/pcreposixd.pdb"
      )
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "E:/OpenGLWorkSpace/pcre-8.44/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
