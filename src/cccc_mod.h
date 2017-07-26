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
 * \file cccc_mod.h
 */

#ifndef CCCC_MOD_H
#define CCCC_MOD_H


#include "cccc_rec.h"
#include "cccc_prj.h"
#include "cccc_use.h"
#include "cccc_mem.h"


static const string MODULE_PREFIX="CCCC_Module";
static const string MODEXT_PREFIX="CCCC_ModExt";


enum ModuleNameLevel
{ nlMODULE_TYPE=-1, nlMODULE_NAME=-2, nlMODULE_TYPE_AND_NAME=-3 };

class CCCC_Module : public CCCC_Record
{
  friend class CCCC_Project;
  friend class CCCC_Html_Stream;
  friend class CCCC_Xml_Stream;

  CCCC_Project *project;

  string module_name, module_type;

  typedef std::map<string,CCCC_Member*> member_map_t;
  member_map_t member_map;

  typedef std::map<string,CCCC_UseRelationship*> relationship_map_t;
  relationship_map_t client_map;
  relationship_map_t supplier_map;

  CCCC_Module();

public:

  /**
   * Create a new module with a given name.
   */
  CCCC_Module(string name);

  string name(int name_level) const;

  GeneralFromFileStatuses_t FromFile(ifstream& infile);
  int ToFile(ofstream& outfile);

  virtual int get_count(const char *count_tag);

  /**
   * \return true if this module is a simple type like
   * native(int, char, ...), enum, struct, ...
   */
  int is_trivial();

  /**
   * Add an objet in member_map, indexed by its key.
   */
  void putInMemberMap(CCCC_Member *member);

  /**
   * Define a relationship with a supplier.
   */
  void addSupplier(string key, CCCC_UseRelationship *userel_ptr);

  /**
   * Define a relationshi with a client.
   */
  void addClient(string key, CCCC_UseRelationship *userel_ptr);
};

#endif // CCCC_MOD_H




