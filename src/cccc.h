/**
 * \file cccc.h
 * \brief Diagnostic and portability facilities for the cccc project
 */

#ifndef _CCCC_H__
#define _CCCC_H__


// I am trying to standardise on using the ANSI C++ names
// for the ANSI C header files, and bringing all of
// the includes of these libraries into this file.
// I have not yet attempted to purge includes for these
// files from the other source files.
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

#include <string>
using std::string;
#include <iostream>
#include <sstream>
#include <fstream>
using std::ostream;
using std::istream;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::stringstream;
using std::endl;
using std::cout;
using std::cerr;

// debugging facilities
extern int DebugMask;
enum DebugFlags {
  LEXER=0x01,
  PARSER=0x02,
  COUNTER=0x04,
  MEMORY=0x08,
  EXTENT=0x10,
  DATABASE=0x20
};
#define DbgMsg(DF,OS,X) if(DebugMask&DF) { OS << X ; }

// the global database to which stuff is added...
class CCCC_Project;
extern CCCC_Project *prj;

// a nasty global array of identifiers we want the lexer to ignore
#define SKIP_IDENTIFIERS_ARRAY_SIZE 256
extern char *skip_identifiers[SKIP_IDENTIFIERS_ARRAY_SIZE];

// These macros were used to cover differences between the way the
// old strstream classes were used in Win32 and GNU builds.
// The differences are no longer necessary.
#define MAKE_STRSTREAM(X)     stringstream X;
#define CONVERT_STRSTREAM(X)  (X)
#define RELEASE_STRSTREAM(X)


#endif

