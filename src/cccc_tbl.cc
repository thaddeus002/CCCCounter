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
 * \file cccc_tbl.cc
 *
 * Manage records.
 */

#ifndef _CCCC_TBL_BODY
#define _CCCC_TBL_BODY


#include "cccc_tbl.h"

#include "cccc_ext.h"
#include "cccc_mod.h"
#include "cccc_use.h"
#include "cccc_mem.h"


#define LINE_BUFFER_SIZE 1000

/** Constructor */
template <class T> CCCC_Table<T>::CCCC_Table()
{
  iter_ = map_t::end();
}

/** Destructor */
template <class T> CCCC_Table<T>::~CCCC_Table()
{
  // the container should manage the destruction of its own
  // nodes correctly, we just need to get rid of the
  // objects to which we hold pointers.
  // NB Although CCCC_Table holds pointers, it owns the
  // objects they point to and is responsible for their disposal.
  T* itemptr=first_item();
  while(itemptr!=NULL)
    {
      delete itemptr;
      itemptr=next_item();
    }
}

template<class T> int CCCC_Table<T>::get_count(const char* count_tag)
{
  int retval=0;
  T* itemptr=first_item();
  while(itemptr!=NULL)
    {
      retval+=itemptr->get_count(count_tag);
      itemptr=next_item();
    }

  return retval;
}

/**
 * \return the item corresponding to "name" or NULL if it not exist.
 */
template<class T> T* CCCC_Table<T>::find(string name)
{
  T *retval=NULL;
  typename map_t::iterator value_iterator=map_t::find(name);
  if(value_iterator!=map_t::end())
  {
    retval=(*value_iterator).second;
  }
  return retval;
}


template<class T> T* CCCC_Table<T>::insert(T* new_item_ptr)
{
  if(new_item_ptr!=NULL)
  {
    typename map_t::value_type new_pair(new_item_ptr->key(), new_item_ptr);
    map_t::insert(new_pair);
  }
  return new_item_ptr;
}


/**
 * \return the objet in table with the same key of the paramètre. If there is not, insert
 *  and return the parameter
 */
template<class T> T* CCCC_Table<T>::find_or_insert(T* new_item_ptr)
{
  string new_key=new_item_ptr->key();
  T *retval=find(new_key);
  if(retval==NULL)
  {
    typename map_t::value_type new_pair(new_key,new_item_ptr);
    map_t::insert(new_pair);
    retval=new_item_ptr;
  }
  return retval;
}

/**
 * Remove the item of the table if is present in it.
 *
 * \return true if the item have been removed
 */
template<class T> bool CCCC_Table<T>::remove(T* old_item_ptr)
{
  bool retval=false;
  typename map_t::iterator value_iterator=map_t::find(old_item_ptr->key());
  if(value_iterator!=map_t::end())
    {
      map_t::erase(value_iterator);
      retval=true;
    }
  return retval;
}

/**
 * Put iter_ to the beginning.
 */
template <class T> void CCCC_Table<T>::reset_iterator()
{
  iter_=map_t::begin();
}

/**
 * \return the first item of the table.
 */
template <class T> T* CCCC_Table<T>::first_item()
{
  reset_iterator();
  return next_item();
}

/**
 * \return the next item of the table or NULL if there are no more.
 */
template <class T> T* CCCC_Table<T>::next_item()
{
  T* retval=NULL;
  if(iter_!=map_t::end())
    {
      retval=(*iter_).second;
      iter_++;
    }
  return retval;
}

/**
 * \return the number of records in the table
 */
template <class T> int CCCC_Table<T>::records()
{
  return map_t::size();
}

// explicit template instantiations for the project are collected here

template class CCCC_Table<CCCC_Extent>;
template class CCCC_Table<CCCC_Module>;
template class CCCC_Table<CCCC_UseRelationship>;
template class CCCC_Table<CCCC_Member>;


#endif // _CCCC_TBL_BODY


