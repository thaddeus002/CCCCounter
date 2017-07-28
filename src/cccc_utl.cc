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
 * \file cccc_utl.cc
 *
 * implementation of enumerations and utility classes for CCCC
 * includes the Parse_Utility class which is a helper to centralise
 * error recovery and recording facilities across the three parsers
 */

#include "cccc.h"

#include "cccc_itm.h"
#include "cccc_utl.h"
#include "cccc_parseStore.h"
#include "cccc_db.h"
#include "cccc_tok.h"
#include "AParser.h"
#include "ATokPtr.h"

#define DEBUG_EXTENT_STREAMS 1

#include <cassert>
#include <iomanip>
using std::ios;
using std::ends;
using std::setw;
using std::setiosflags;
using std::resetiosflags;


#define FS "@"
#define RS "\n"

string ParseUtility::stack_rules[MAX_STACK_DEPTH];
int    ParseUtility::stack_tokenline[MAX_STACK_DEPTH];
string ParseUtility::stack_tokentext[MAX_STACK_DEPTH];
int    ParseUtility::stack_depth;

ParseUtility* ParseUtility::theCurrentInstance=NULL;

// insertion and extraction functions intended to support enumerations
void insert_enum(ostream& os, int e)
{
  os << (char) e;
}

void extract_enum(istream& is, int& e)
{
  e=0;
  is >> (char&) e;
}


ostream& operator<<(ostream& os, AugmentedBool ab) {
  insert_enum(os,ab);
  return os;
}

istream& operator>>(istream& is, AugmentedBool& ab) {
  extract_enum(is,(int&)ab);
  return is;
}

ostream& operator<<(ostream& os, Visibility v) {
  insert_enum(os,v);
  return os;
}

istream& operator>>(istream& is, Visibility& v) {
  extract_enum(is,(int&)v);
  return is;
}

ostream& operator<<(ostream& os, UseType ut) {
  insert_enum(os,ut);
  return os;
}

istream& operator>>(istream& is, UseType& ut) {
  extract_enum(is,(int&)ut);
  return is;
}

string ParseUtility::lookahead_text(int n)
{
  static string retval;
  retval="";
  int i;
  for(i=1; i<=n; i++)
    {
      if(parser->LT(i) != NULL)
  {
    retval=retval+parser->LT(i)->getText();
    retval=retval+" ";
  }
    }
  return retval;
}

void ParseUtility::resynchronize(int initial_nesting,
         SetWordType *resync_token_class,
         ANTLRTokenPtr& resync_token)
{
  // the interface for resynchronisation is as follows:
  // the caller supplies a nesting level at which the resynchronisation must
  // occur, and a token class containing all of the tokens which can
  // be accepted to delimit the resynchronisation
  // this function will scan until it finds that it is at the correct level and
  // the next token of lookahead is in the resynchronisation set
  // it will then accept as many tokens from the resynchronisation set as
  // are available, consolidating the text of the tokens accepted
  // as the text associated with the last token
  string resync_text="...";

  string string1=parser->LT(1)->getText();
  int line1=parser->LT(1)->getLine();
  string string2;
  int line2=0;

  int resynchronising=1;
  while(resynchronising)
  {
    parser->consumeUntil(resync_token_class);
    if(
  (MY_TOK(parser->LT(1))->getNestingLevel() > initial_nesting) &&
        (parser->LT(2) != NULL)
        )
    {
      parser->consume();
    }
    else
    {
      // we are ready to resynchronise
      resynchronising=0;
      string2=parser->LT(1)->getText();
      line2=parser->LT(1)->getLine();
    }
  }

  // we now consume a succession of tokens from the resynchronisation token
  // class until we come across a token which is not in the set, or the
  // nesting level changes
  resync_token=parser->LT(1);
  while(
  parser->set_el(parser->LT(1)->getType(),resync_token_class) &&
  ( MY_TOK(parser->LT(1))->getNestingLevel() == initial_nesting)
  )
    {
      string2=parser->LT(1)->getText();
      line2=parser->LT(1)->getLine();

      resync_text+=parser->LT(1)->getText();
      resync_text+=" ";
      resync_token=parser->LT(1);
      resync_token->setText(resync_text.c_str());
      parser->consume();
    }

  cerr << "Unrecognized section from "
       << string1.c_str() << " on line " << line1 << " to "
       << string2.c_str() << " on line " << line2 << endl
       << "=====ignored section begins=====" << endl
       << resync_text.c_str() << endl
       << "===== ignored section ends =====" << endl;
}


ParseUtility::ParseUtility(ANTLRParser *parser)
{
  // This is designed as a serial-singleton class (e.g. many
  // instances may exist over time but no more than one at a
  // time).
  // For the lifetime of an instance, the static member theCurrentInstance
  // points to it. When no instance exists, this pointer is null.
  assert(theCurrentInstance==NULL);
  theCurrentInstance=this;

  trace_depth=0;
  stack_depth=0;
  this->parser=(ANTLR_Assisted_Parser*)parser;

}

ParseUtility::~ParseUtility()
{
  theCurrentInstance=NULL;
}

// This utility function is used to create
// a composite scope name from a qualifier scope
// and a relative name.
string ParseUtility::scopeCombine(const string& baseScope, const string& name)
{
  // I am presently (as at 3.pre44) experimenting with
  // how I handle scopes.  The present code has a policy
  // of discarding scope information altogether and defining
  // modules based solely on the final component of the
  // fully qualified name.
  // This variable may become a parameter to control policy in this
  // area.
  bool bIgnoreScope=true;
  string retval;
  if(bIgnoreScope)
  {
    retval=name;
  }
  else if(baseScope.size()>0 && name.size()>0)
  {
    retval=baseScope+"::"+name;
  }
  else
  {
    retval=baseScope+name;
  }

  return retval;
}

static void toktrace(ANTLRAbstractToken *tok)
{
  // at the LHS we put out information about the current token
  if(tok != NULL)
  {
    DbgMsg(PARSER,cerr,
       std::setw(6) << tok->getLine()
       << std::setw(4) << (int)tok->getType()
       << std::setiosflags(ios::left)
       << std::resetiosflags(ios::right)
       << std::setw(20) << tok->getText()
       );
  }
  else
  {
    DbgMsg(PARSER,cerr,std::setw(30)<<"");
  }
}

static void rectrace(const char *rulename,
         const char *dir_indic,
         int guessing,
         ANTLRAbstractToken *tok)
{
  static int trace_depth=0;
  if(guessing)
  {
    DbgMsg(PARSER,cerr,
       setw(trace_depth*4+1) << "" << dir_indic
       << "?" << rulename << endl);
  }
  else
  {
    trace_depth=((ANTLRToken*) tok)->getNestingLevel();
    DbgMsg(PARSER,cerr,
     setw(trace_depth*4)<< "" << dir_indic << rulename << endl);
  }
}

void ParseUtility::tracein(
         const char *rulename, int guessing,
         ANTLRAbstractToken *tok)
{
  if(guessing == 0)
  {
    stack_tokentext[stack_depth]=tok->getText();
    stack_tokenline[stack_depth]=tok->getLine();
    stack_rules[stack_depth]=rulename;
    stack_depth++;
  }

  // first put out the token details
  toktrace(tok);

  // then the indented recognition trace
  rectrace(rulename,"-> ",guessing,tok);
}

void ParseUtility::traceout(const char *rulename, int guessing,
          ANTLRAbstractToken *tok)
{
  if(guessing == 0)
  {
    stack_depth--;
    // some error checking...
    if(stack_depth<0)
    {
      cerr << "ParseUtility::traceout negative stack depth - "
         << "exiting from rule " << rulename
         << " at " << tok->getText() << " on line " << tok->getLine()
         << endl;
    }
    else if(rulename!=stack_rules[stack_depth])
    {
      cerr << "ParseStore::traceout rule name mismatch - "
         << rulename << "!=" << stack_rules[stack_depth] << endl;
    }

    stack_tokentext[stack_depth]="";
    stack_tokenline[stack_depth]=0;
    stack_rules[stack_depth]="";
  }
  // first put out the token details
  toktrace(tok);
  rectrace(rulename,"<- ",guessing,tok);
}

void ParseUtility::syn(
  _ANTLRTokenPtr tok, ANTLRChar *egroup, SetWordType *eset,
  ANTLRTokenType etok, int k)
{
  string filename=ParseStore::currentInstance()->filename();
  if(tok != NULL)
    {
      cerr << filename << '(' << tok->getLine() << "):"
     << " syntax error at token " << tok->getText() << endl;
    }
  else
    {
      cerr << filename << "(0): syntax error at null token" << endl;
    }

#if 1
    // The logic in the other half of this #if section
    // generated too much noise for some people's taste.
    // It's only really useful to myself (TJL) or anyone
    // else with a taste for debugging cccc.g/java.g etc.
    int i=stack_depth-1;
      cerr << filename << '(' << stack_tokenline[i]
     << "): trying to match " << stack_rules[i]
     << " at '" << stack_tokentext[i] << "'"
     << endl;
#else
  cerr << "Parser context:" << endl;
  for(int i=stack_depth-1; i>=0; i--)
    {
      cerr << filename << '(' << stack_tokenline[i]
     << "): trying to match " << stack_rules[i]
     << " at '" << stack_tokentext[i] << "'"
     << endl;
    }
  cerr << endl;
#endif
}

