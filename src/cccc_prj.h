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
 * \file cccc_prj.h
 * \brief Define class CCCC_project
 */

#ifndef CCCC_PRJ_H
#define CCCC_PRJ_H

#include "cccc_rec.h"
#include "cccc_mod.h"
#include "cccc_mem.h"
#include "cccc_use.h"


static const string REJEXT_PREFIX="CCCC_RejExt";

enum RelationshipMaskElements
{
  rmeCLIENT=0x01, rmeSUPPLIER=0x02,
  rmeHIDDEN=0x10, rmeVISIBLE=0x20, rmeHIDDEN_OR_VISIBLE=0x30,
  rmeABSTRACT=0x40, rmeCONCRETE=0x80, rmeABSTRACT_OR_CONCRETE=0xC0
};

/**
 * A CCCC_Project is a CCCC_Record with four tables :
 * modules, members, useRelationship and rejected extents
 */
class CCCC_Project : public CCCC_Record
{
  friend class CCCC_Html_Stream;
  friend class CCCC_Xml_Stream;


  CCCC_Table<CCCC_Module>          module_table;
  CCCC_Table<CCCC_Member>          member_table;
  CCCC_Table<CCCC_UseRelationship> userel_table;
  CCCC_Table<CCCC_Extent>          rejected_extent_table;

  std::map<string, CCCC_Item> OptionTable;


 public: // because MSVC++ version of STL needs it to be...

  // we need a record of which extents came from which files
  // so that when we implement persistence, we can purge
  // extent records from each file as we re-analyze it
  struct ExtentTableEntry
  {
    CCCC_Table<CCCC_Extent> *table_ptr;
    CCCC_Extent *extent_ptr;
    ExtentTableEntry() : table_ptr(NULL), extent_ptr(NULL) {}
  };

  typedef std::multimap<string, ExtentTableEntry> FileExtentTable;
  FileExtentTable file_extent_table;

 public:
  CCCC_Project(const string& name="");

  // these functions are used in both the analyzer
  // and the load side of the persistence code
  // to add entities to the project
  void add_module(CCCC_Item& module_data_line);
  void add_member(CCCC_Item& member_data_line);
  void add_userel(CCCC_Item& use_data_line);
  void add_rejected_extent(CCCC_Item& rejected_data_line);

  /**
   * this function is used after loading and/or analysis
   * has been completed to (re)create the maps owned by
   * each module of its members and relationships
   */
  void reindex();

  int get_count(const char *count_tag);


  int FromFile(ifstream& infile);
  int ToFile(ofstream& outfile);

  void set_option(string key, CCCC_Item& option_data_line);
  int get_option(string key, CCCC_Item& option_data_line);

  // declared in CCCC_Record
  virtual string name(int level) const;

  /**
   * Add a new module in module_table if is not already in.
   */
  CCCC_Module *find_or_insert_module(CCCC_Module *module);

  /**
   * Find a module in module_table by his name.
   */
  CCCC_Module *find_module(string module_name);

  /**
   * Add a new member in member_table if is not already in.
   */
  CCCC_Member *find_or_insert_member(CCCC_Member *member);

  /**
   * Add a new relationship in userel_table if is not already in.
   */
  CCCC_UseRelationship *find_or_insert_userel(CCCC_UseRelationship *relationship);
};

#endif // CCCC_PRJ_H


