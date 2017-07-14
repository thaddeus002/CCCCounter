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
 * \file cccc_ext.h
 */

#ifndef CCCC_EXT_H
#define CCCC_EXT_H

#include <string>
using std::string;

#include "cccc_utl.h"
#include "cccc_itm.h"


/**
 * Identifie a parametre of the objet.
 * To use with CCCC_Extent::name()
 */
enum ExtentNameLevel { nlFILENAME=-1, nlLINENUMBER=-2, nlDESCRIPTION=-3};


/**
 * This may be an object representation of an code Item.
 */
class CCCC_Extent
{
  // item data
  string filename;
  string linenumber;
  string description;
  string flags;
  /** ? */
  string count_buffer;
  UseType ut;
  Visibility v;

  // internal data
  /** this is for initialyse extkey */
  static unsigned int nextkey;
  /** instance index */
  unsigned int extkey;

 public:
  CCCC_Extent();
  /** read the variable in a item */
  CCCC_Extent(CCCC_Item& is);

  /**
   * Get an element of the objet
   * \param level which element to return : ExtentNameLevel, nlRANK, nlSEARCH (enum NameLevel)
   */
  string name(int level) const;

  /**
   * \return an unique key for this object.
   */
  string key() const;

  /** This is the same than the constructor */
  int GetFromItem(CCCC_Item& item);

  int AddToItem(CCCC_Item& item);


  int get_count(const char *count_tag);

  // GETTERS

  Visibility get_visibility() const { return v; }

  UseType get_usetype() const { return ut; }

  const char* get_description() const { return description.c_str(); }
};

#endif // CCCC_EXT_H



