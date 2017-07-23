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
 * \file cccc_extent_utils.h
 */

#ifndef __CCCC_EXTENT_UTL_H
#define __CCCC_EXTENT_UTL_H

#include <iostream>
using std::ostream;
using std::istream;


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


enum Visibility {
  vPUBLIC='0',vPROTECTED='1',vPRIVATE='2',vIMPLEMENTATION='3',
  vDONTKNOW='?',vDONTCARE='X',vINVALID='*'
};

ostream& operator << (ostream&, Visibility);

istream& operator >> (istream&, Visibility&);

enum AugmentedBool {
  abFALSE='F', abTRUE='T', abDONTKNOW='?', abDONTCARE='X', abINVALID='*'
};
ostream& operator << (ostream& os, AugmentedBool ab);
istream& operator >> (istream& is, AugmentedBool& ab);

enum UseType {
  utDECLARATION='D', utDEFINITION='d',  // of methods and classes
  utINHERITS='I',                       // inheritance, including Java
  // extends and implements relations
  utHASBYVAL='H', utHASBYREF='h',       // class data member
  utPARBYVAL='P', utPARBYREF='p',       // method parameter or return value
  utVARBYVAL='V', utVARBYREF='v',       // local variable within a method
  utTEMPLATE_NAME='T',                  // typedef alias for a template
  utTEMPLATE_TYPE='t',                  // type over which a template is
  // instantiated
  utINVOKES='i',                        // C function invocation
  utREJECTED='r',                       // for extents rejected by the parser
  utWITH='w',                           // Ada 'with' keyword context
  utDONTKNOW='?', utDONTCARE='X', utINVALID='*'
};



// the parse state object consists of a number of strings representing
// knowledge about the identification of the source code object currently
// being processed, a number of flags of type AugmentedBool, and
// items representing knowledge about the
// concerning the object's nature, and also its visibility

enum PSString {
  pssFILE, pssRULE, pssFLAGS, // the context of the parse
  pssMODTYPE, pssMODULE,      // the syntactic class and name of the module
  pssUTYPE,                    // unqualified type of the current member
  pssINDIR,                   // indirection associated with the type above
  pssITYPE,                   // type qualified with indirection
  pssMEMBER, pssPARAMS,       // name, parameter list of a member
  pssDESCRIPTION,             // textual description of the relationship type
  pssLAST                     // used to dimension the array
};

enum PSFlag {
  psfCONST, psfSTATIC, psfEXTERN, psfVIRTUAL, // AugmentedBool
  psfVISIBILITY,                              // Visibility
  psfLAST                                     // used to dimension the array
};

enum PSVerbosity { psvSILENT, psvQUIET, psvLOUD };



#endif


