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
 * cccc_db.h
 */

#ifndef CCCC_DB_H
#define CCCC_DB_H


#include "cccc_rec.h"
#include "cccc_prj.h"


/**
 * This function provides the ability for the persistence functions
 * defined below to do a quick peek at the first token on the stream
 * leaving the get pointer at the start of that token.
 */
bool PeekAtNextLinePrefix(ifstream& ifstr, string pfx);



// These are global variables because I don't want to have
// to pass the project, module, member, relationship down into all of the
// FromFile methods.
// There is probably a better way for a less lazy programmer than me.
extern CCCC_Project *current_loading_project;

/** this one tracks the line number in the input file */
extern int ifstr_line;

template <class T> void DisposeOfImportRecord(T *record_ptr, GeneralFromFileStatuses_t fromfile_status);

void Resolve_Fields(string& field1, string& field2);


#endif // CCCC_DB_H



