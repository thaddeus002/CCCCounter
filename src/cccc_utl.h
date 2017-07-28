/*
    CCCC - C and C++ Code Counter
    Copyright (C) 1994-2005 Tim Littlefair (tim_littlefair@hotmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

/**
 * \file cccc_utl.h
 *
 * this file declares all enumeration datatypes used in the project, and
 * also the parse state class, which is used to capture information in the
 * parse and transfer it to the code database for later report generation
 *
 * for each enumeration, a single character code is defined for each member
 * these codes are shown in the inline comments
 *
 * the enumerations are designed to support resolution of incomplete
 * knowledge about several sections of code which relate to the same
 * object to give the most complete picture available
 */

#ifndef __CCCC_UTL_H
#define __CCCC_UTL_H

#include <map>
#include <vector>
#include <string>
using std::string;


#include "cccc_tok.h"

// The -gd option generates uncompilable code with a missing
// variable called zzTracePrevRuleName if the generated
// files cccc.cpp, java.cpp, ada.cpp don't include a version
// of AParser.h seen with zzTRACE_RULES defined.
// I'm not sure how this is supposed to work, but for the moment
// I am including it here which should make all three files OK.
// Note that this could break again if the header files shift around
// and AParser.h gets read before zzTRACE_RULES is defined.
// Another option is turning -gd off, but its the way we do the
// cccc -dp debug output which is very useful.
#define zzTRACE_RULES
#include "AParser.h"

class ANTLRAbstractToken;
class ANTLRTokenPtr;
class AST;

/**
 * the languages which can be parsed
 * only C and C++ are implemented as yet
 */
enum Language { lAUTO, lCPLUSPLUS, lANSIC, lJAVA, lADA };

extern Language global_language, file_language;


#define MAX_STACK_DEPTH 1000

/**
 * I have moved some actions originally embedded within the C++ grammar
 * out of the grammar into the class ParseUtility defined below, so that
 * other grammars can use them as well for consistency and efficiency.
 * The ParseUtility::resynchronize() method provides a standardised way
 * of 1) resynchronising the parser, and 2) reporting the parse error
 * which caused the problem.  Unfortunately, to do the resynchronisation
 * it requires access to protected functions of ANTLRParser.
 * The class ANTLR_Assisted_Parser below is a hack to enable ParseUtility
 * to violate the protection of the functions required: ParseUtility is
 * passed a pointer to a real parser which is of a subclass of ANTLRParser,
 * and casts it to this artificial subclass, so as to give ParseUtility
 * friend rights and to access the protected functions.
 * This hack is necessary because the class definition we need to affect
 * is generated by PCCTS: I am not proud of it and if anyone can suggest
 * a way of doing without modifying PCCTS or its support code, I will be
 * very happy to hear about it.
 */
class ANTLR_Assisted_Parser : public ANTLRParser
{
  ANTLR_Assisted_Parser(ANTLRParser& parser) : ANTLRParser(parser) {}
  friend class ParseUtility;
};

/**
 * The parse utility class is intended to assist the parser in a number
 * of ways.  In earlier versions, this class had at least two distinct
 * roles:
 * 1) as a place for common functions which each parser might call
 *    for diagnostics, resynchronisation etc; and
 * 2) as a general storage area for state which needs to be remembered
 *    for any length of time during the parsing process.
 * The class ParseStore has been added to support the second role,
 * and it is hoped that the amount of stored state can be reduced
 * in the near future.
 */
class ParseUtility {

 public:
  ParseUtility(ANTLRParser *parser);
  ~ParseUtility();

  // the following methods are used to service the standard tracein/traceout
  // and syntax error reporting calls generated by PCCTS
  void tracein(const char *rulename, int guessing, ANTLRAbstractToken *tok);
  void traceout(const char *rulename, int guessing, ANTLRAbstractToken *tok);
  void syn(_ANTLRTokenPtr tok, ANTLRChar *egroup, SetWordType *eset,
     ANTLRTokenType etok, int k);

  // this method consolidates the text of the next n tokens of lookahead
  string lookahead_text(int n);

  // this method searches for a string of tokens at the specified nesting
  // depth from the specified token class, and uses them as a marker to
  // resynchronise the parser
  void resynchronize(
         int initial_nesting, SetWordType *resync_token_class,
         ANTLRTokenPtr& resync_token);

  // This utility function is used to create
  // a composite scope name from a qualifier scope
  // and a relative name.
  string scopeCombine(const string& baseScope, const string& name);

  // Only one instance of this class should exist at any time.
  // This method allows the parsers and lexers to access the instance.
  static ParseUtility *currentInstance() { return theCurrentInstance; }

 private:
  static ParseUtility *theCurrentInstance;

  ANTLR_Assisted_Parser *parser;
  int trace_depth;
  static int stack_depth;
  static string stack_tokentext[MAX_STACK_DEPTH];
  static int    stack_tokenline[MAX_STACK_DEPTH];
  static string stack_rules[MAX_STACK_DEPTH];

  // copy constructor and assignment operator are private to
  // prevent unexpected copying
  ParseUtility(const ParseUtility&);
  const ParseUtility& operator=(const ParseUtility&);
};

/**
 * LOC, COM and MVG are all counted by the lexical analyzer,
 * but the counts must be apportioned after the parser has
 * identified the extents of the various declarations and definitions
 * they belong to.
 * This is achieved by the lexer maintaining counts of each
 * which are reported to the ParseUtility class on a line by line
 * basis. ParseUtility uses this data to create a store which is
 * used to apportion counts as the parser reports extents.
 */
enum LexicalCount { tcCOMLINES, tcCODELINES, tcMCCABES_VG, tcLAST };



#endif


