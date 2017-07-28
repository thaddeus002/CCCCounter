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


#include "cccc.h"
#include "cccc_prj.h"
#include "cccc_parseStore.h"


ParseStore* ParseStore::theCurrentInstance=NULL;

ParseStore::ParseStore(const string& filename)
: theFilename(filename)
, pendingLexicalCounts(static_cast<int>(tcLAST),0)
, flag(static_cast<int>(psfLAST)+1,'?')
{
  // This is designed as a serial-singleton class (e.g. many
  // instances may exist over time but no more than one at a
  // time).
  // For the lifetime of an instance, the static member theCurrentInstance
  // points to it. When no instance exists, this pointer is null.
  assert(theCurrentInstance==NULL);
  theCurrentInstance=this;
  flag[psfLAST]='\0';
}

ParseStore::~ParseStore()
{
  // If the current object came from the default constructor
  // it is the primary singleton instance and we wish to
  // set the static pointer to itself back to null.  Otherwise,
  // it was a cached copy, and we don't really care.
  if(theCurrentInstance==this)
    {
      theCurrentInstance=NULL;
    }
}

int ParseStore::get_flag(PSFlag psf) const {
  return int(flag[psf]);
}

void ParseStore::set_flag(PSFlag psf, int value) {
  flag[psf]=value;
}

void ParseStore::set_flag(Visibility value) {
  MAKE_STRSTREAM(ofstr);
  ofstr << value;
  flag[psfVISIBILITY]=(ofstr.str())[0];
  RELEASE_STRSTREAM(ofstr);
}

Visibility ParseStore::get_visibility()
{
  return static_cast<Visibility>(flag[psfVISIBILITY]);
}

string ParseStore::filename()
{
  return theFilename;
}

void
ParseStore::
insert_extent(CCCC_Item& os, int startLine, int endLine,
        const string& description, const string& flags,
        UseType ut, bool allocate_lexcounts)
{
  os.Insert(theFilename);
  os.Insert(startLine);
  os.Insert(description);
  os.Insert(flags);
  int i;
  int lexical_counts_for_this_extent[tcLAST];
  for(i=0; i<tcLAST; i++)
    {
      lexical_counts_for_this_extent[i]=0;
    }

  if(allocate_lexcounts==true)
    {
      LineLexicalCountMatrix::iterator extentStartIter =
  lineLexicalCounts.lower_bound(startLine);
      LineLexicalCountMatrix::iterator extentEndIter =
  lineLexicalCounts.upper_bound(endLine-1);
      LineLexicalCountMatrix::iterator llcmIter;
      for(llcmIter=extentStartIter;
    llcmIter!=extentEndIter;
    ++llcmIter)
  {
    // This record relates to a line within the current
    // extent.
    for(i=0; i<tcLAST; i++)
      {
        lexical_counts_for_this_extent[i]+=(*llcmIter).second[i];
      }
  }
      // The lexical occurrences mentioned in the records processed
      // above are now been accounted for in the database, so we
      // purge these records.  This has the effect of allowing
      // accurate accounting on nested extents (i.e. the outer
      // extent will only be reported as containing lines which
      // are not already listed in the inner extent).
      lineLexicalCounts.erase(extentStartIter,extentEndIter);

      ostringstream lexcount_str;

      lexcount_str << "LOC:" << lexical_counts_for_this_extent[tcCODELINES]
       << " COM:" << lexical_counts_for_this_extent[tcCOMLINES]
       << " MVG:" << lexical_counts_for_this_extent[tcMCCABES_VG]
       << std::ends;

      os.Insert(lexcount_str.str().c_str());

    }
  else
    {
      os.Insert("*");
    }
  os.Insert((char)flag[psfVISIBILITY]);
  os.Insert((char)ut);
}



void ParseStore::record_module_extent(int startLine, int endLine,
              const string& moduleName,
              const string& moduleType,
              const string& description,
              UseType ut)
{
  // See the lengthy comment in record_userel_extent about
  // why we are filtering for empty module names.
  if(moduleName.size()>0)
  {
    CCCC_Item module_line;
    module_line.Insert(moduleName);
    module_line.Insert(moduleType);
    insert_extent(module_line,startLine,endLine,
    description,flags(),ut,true);
    prj->add_module(module_line);
  }
}

void ParseStore::record_function_extent(int startLine, int endLine,
          const string& returnType,
          const string& moduleName,
          const string& memberName,
          const string& paramList,
          const string& description,
          Visibility visibility,
          UseType ut)
{
  // We require every call to this function to specify a member
  // function name and a parameter list.
  if(memberName.size()>0)
  {
    // If the moduleName is an empty string, we remap this to the
    // string "anonymous".  This implies that we treat all
    // C-style functions as belonging to a single module.
    string mappedModuleName = moduleName;
    if(mappedModuleName.size()==0)
    {
       mappedModuleName = "anonymous";
    }

    CCCC_Item function_line;
    function_line.Insert(mappedModuleName);
    function_line.Insert(memberName);
    function_line.Insert(returnType);
    function_line.Insert(paramList);

    string baseFlags=flags();
    baseFlags[psfVISIBILITY]=visibility;

    insert_extent(function_line,startLine,endLine,
     description,baseFlags,ut,true);
    prj->add_member(function_line);
  }
}

void ParseStore::record_userel_extent(int startLine, int endLine,
              const string& clientName,
              const string& memberName,
              const string& serverName,
              const string& description,
              Visibility visibility,
              UseType ut)
{
  CCCC_Item userel_line;

  // This function should not be invoked unless the clientName
  // and serverName are non-empty strings, however it appears
  // that in test case prn16.java the parser does execute the
  // actions of the 'implementsClause' rule, even though there
  // is no 'implements' keyword outside comments in the program
  // text.
  // I don't understand this, but as a workaround, I filter at
  // this point and ensure that if either clientName or serverName
  // is empty, no action is taken.
  if(clientName.size()>0 && serverName.size()>0)
  {
    userel_line.Insert(clientName);
    userel_line.Insert(memberName);
    userel_line.Insert(serverName);

    // for data member definitions, we record lexical data for the
    // extent,
    // for inheritance and parameter relationships we do not
    bool record_lexcounts=false;
    switch(ut)
    {
    case utHASBYVAL:
    case utHASBYREF:
      record_lexcounts=true;
      break;
    default:
      record_lexcounts=false;
    }

    string baseFlags=flags();
    baseFlags[psfVISIBILITY]=visibility;
    insert_extent(userel_line,startLine,endLine,
      description,baseFlags,ut,record_lexcounts);
    prj->add_userel(userel_line);
  }
}

void ParseStore::record_other_extent(int startLine, int endLine,
            const string& description)
{
  CCCC_Item rejext_line;
  insert_extent(rejext_line,startLine,endLine,description,flags(),utREJECTED,true);
  prj->add_rejected_extent(rejext_line);
}


void ParseStore::endOfLine(int line)
{
  // We only do the processing below if the line which has just
  // ended contained at least one non-skippable token
  // The flag which tells us whether this is true is set in the
  // token constructor
  if(ANTLRToken::bCodeLine)
  {
    pendingLexicalCounts[tcCODELINES]++;
    LineLexicalCountMatrix::value_type vt(line,LexicalCountArray(static_cast<int>(tcLAST),0));

    for(int i=0; i<tcLAST; i++)
    {
      vt.second[i]=pendingLexicalCounts[i];
      pendingLexicalCounts[i]=0;
    }

    lineLexicalCounts.insert(vt);

    // reset the flat for next time
    ANTLRToken::bCodeLine=false;
  }
}

