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
  bool sorted;

 public:
  CCCC_Table();
  virtual ~CCCC_Table();

  /**
   * \return the number of records in the table
   */
  int records();

  T* find(string name);

  T* find_or_insert(T* new_item_ptr);

  bool remove(T* old_item_ptr);

  void reset_iterator();

  T* first_item();

  T* next_item();

  /**
   * \return the sum of the values of "count_tag" for all items in the
   * table
   */
  virtual int get_count(const char *count_tag);

  void sort();
};


#endif // CCCC_TBL_H

