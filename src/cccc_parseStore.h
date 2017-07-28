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


#ifndef __CCCC_PARSE_STORE_H
#define __CCCC_PARSE_STORE_H


#include "cccc_extent_utils.h"
#include "cccc_utl.h"
#include "cccc_itm.h"


/**
 * This class is used by the lexer.
 *
 * The ParseStore class encapsulates all information storage
 * requirements related to the parser, and also manages
 * the process of feeding that information to the database
 * when it is complete.
 * In particular, the class is responsible for receiving and
 * retaining counts of the lexical metrics (LOC, COM,
 * MVG) on a line-by-line basis.  These are counted in the
 * lexical analyzer, and the line-by-line counts must be
 * integrated to allocate the counts to the extents identified
 * by the parser as belonging to significant declarations and
 * definitions.
 */
class ParseStore
{
 public:
  ParseStore(const string& filename);
  ~ParseStore();

  void IncrementCount(LexicalCount lc) { pendingLexicalCounts[lc]++; }
  void endOfLine(int line);


  // each of the functions below writes one or more records into
  // the database of code

  void record_module_extent(int startLine, int endLine,
          const string& moduleName,
          const string& moduleType,
          const string& description,
          UseType ut);

  void record_function_extent(int startLine, int endLine,
            const string& returnType,
            const string& moduleName,
            const string& memberName,
            const string& paramList,
            const string& description,
            Visibility visibility,
            UseType ut);

  void record_userel_extent(int startLine, int endLine,
          const string& clientName,
          const string& memberName,
          const string& serverName,
          const string& description,
          Visibility visibility,
          UseType ut);

  void record_other_extent(int startLine, int endLine,
            const string& description);

  void record_file_balance_extent(string);

  // Each of the record_XXX methods above uses this function to
  // add an extent record.
  void insert_extent(CCCC_Item&, int, int,
         const string&, const string&,
         UseType, bool allocate_lexcounts);

  // the class maintains a number of strings and flags which reflect
  // the most recently recognized module, member, type (with and without
  // indirection) etc, and the visibility of items occuring at the current
  // context
  int get_flag(PSFlag) const;
  void set_flag(PSFlag,int);
  void set_flag(Visibility);
  Visibility get_visibility();
  string filename();

  char *flags() { return &(*flag.begin()); }

  // We also need the automatically generated copy constructor
  // and assignment operator to allow us to save state in the
  // parser.

  /**
   * Only one instance of this class should exist at any time.
   * This method allows the parsers and lexers to access the instance.
   */
  static ParseStore *currentInstance() { return theCurrentInstance; }

 private:
  static ParseStore *theCurrentInstance;

  string theFilename;

  typedef std::vector<int> LexicalCountArray;
  LexicalCountArray pendingLexicalCounts;

  typedef std::map<int,LexicalCountArray> LineLexicalCountMatrix;
  LineLexicalCountMatrix lineLexicalCounts;

  typedef std::vector<char> CharArray;
  CharArray flag;

  // copy constructor and assignment operator are private to
  // prevent unexpected copying
  ParseStore(const ParseStore&);
  const ParseStore& operator=(const ParseStore&);
};

#endif


