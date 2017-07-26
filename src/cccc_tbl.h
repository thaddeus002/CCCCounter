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
 * \file cccc_tbl.h
 * \brief Defines the database used by CCCC to generate a report.
 */

#ifndef CCCC_TBL_H
#define CCCC_TBL_H

#include <iostream>
#include <string>
#include <map>

using std::string;

/**
 * CCCC_Table started its life as an array of pointers to CCCC_Records.
 * It will ultimately become identical to a std::map from string to T*.
 * In the mean time we are supporting a legacy API.
 * T can be a CCCC_Extent, CCCC_Module,
 * CCCC_Member or CCCC_UseRelationship.
 */
template <class T> class CCCC_Table : public std::map<string,T*>
{
  typedef std::map<string,T*> map_t;
  typename map_t::iterator iter_;

  /**
   * Put the map iterator to the beginning.
   */
  void reset_iterator();


 public:
  CCCC_Table();
  virtual ~CCCC_Table();

  /**
   * \return the number of records in the table
   */
  int records();

  /**
   * \return the item corresponding to "name" or NULL if it not exist.
   */
  T* find(string name);

  /**
   * Insert a new object in the table.
   */
  T* insert(T* new_item_ptr);

  /**
   * \return the objet in table with the same key of the parameter. If there is not, insert
   *  and return the parameter
   */
  T* find_or_insert(T* new_item_ptr);

  /**
   * Remove the item of the table if is present in it.
   *
   * \return true if the item have been removed
   */
  bool remove(T* old_item_ptr);

  /**
   * \return the first item of the table.
   */
  T* first_item();

  /**
   * \return the next item of the table.
   */
  T* next_item();

  /**
   * \return the sum of the values of "count_tag" for all items in the
   * table
   */
  virtual int get_count(const char *count_tag);
};


#endif // CCCC_TBL_H

