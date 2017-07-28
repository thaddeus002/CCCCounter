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
 * \file cccc_rec.h
 * \brief Defines the database used by CCCC to generate a report
 */

#ifndef CCCC_REC_H
#define CCCC_REC_H

#include "cccc_tbl.h"
#include "cccc_ext.h"


/**
 * The entities held within the database need to be able to return a variety
 * of kinds of name including a simple name (typically one word), a fully
 * qualified local name (i.e as used within a class), and a fully
 * qualified global name.
 * Subclasses may also have particular other names, which should be defined
 * using negative indexes.
 */
enum NameLevel { nlRANK, nlSEARCH, nlSIMPLE, nlLOCAL, nlGLOBAL };
typedef  enum NameLevel NameLevel_t;

/**
 * The various FromFile functions need to indicate to their
 * caller their status, particularly because the caller will have
 * allocated an instance of the incoming class on the heap, and needs
 * to know whether it has to delete it.  There are two 'normal' outcomes
 * plus a potentially infinite range of possible application error
 * conditions.  The normal conditions are where the new instance is
 * the first encountered of this module/member/relationship/whatever
 * and the allocated item has been added to the database and must not
 * be deleted, and when the new instance is of a previously encountered
 * entity, and the information from the new record has been transcribed
 * and merged into the instance in the database, and the locally allocated
 * instance must be deleted.
 */
enum GeneralFromFileStatuses
{
  RECORD_TRANSCRIBED = 0,
  RECORD_ADDED       = 1,
  RECORD_ERROR       = 2,
  // error conditions may return RECORD_ERROR, or may use a distinctive
  // value defined as a literal
  /** Status specific to CCCC_Member objects */
  MEMBER_RECORD_NO_PARENT_FOUND=3
};
typedef enum GeneralFromFileStatuses GeneralFromFileStatuses_t;

class CCCC_Record
{
  friend class CCCC_Html_Stream;
  friend class CCCC_Xml_Stream;
  static CCCC_Project *active_project;

 protected:
  typedef CCCC_Table<CCCC_Extent> Extent_Table;
  Extent_Table extent_table;
  /** PSFlags as AugmentedBool : a char by flags */
  string flags;
  virtual void merge_flags(string& new_flags);

 public:

  virtual ~CCCC_Record() {}

  virtual string name(int level) const = 0;

  virtual string key() const;

  AugmentedBool get_flag(PSFlag psf) { return (AugmentedBool) flags[psf]; }

  /**
   * Add an entry to extend_table if is not already in.
   * \param is the data to create the extend to add
   */
  virtual void add_extent(CCCC_Item& is);

  virtual int get_count(const char *count_tag)=0;


  // getters et setters

  static CCCC_Project* get_active_project();
  static void set_active_project(CCCC_Project* prj);
};



#endif // CCCC_REC_H


