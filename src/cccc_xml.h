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
 * \file cccc_xml.h
 */


#ifndef __CCCC_XML_H
#define __CCCC_XML_H


#include <fstream>
#include <time.h>


// Ideally, this class would be defined to reuse common
// facilities abstracted from CCCC_Html_Stream.
// This may happen in the future, for the moment, the implementation
// has been created by cut and paste.

// We need to include cccc_htm.h because it defines the enumeration
// ReportType.
#include "cccc_htm.h"


class CCCC_Xml_Stream {
  friend CCCC_Xml_Stream& operator <<(CCCC_Xml_Stream& os,
               const string& stg);
  friend CCCC_Xml_Stream& operator <<(CCCC_Xml_Stream& os,
               const CCCC_Metric& mtc);

  ofstream fstr;
  static string libdir;
  static string outdir;
  static CCCC_Project* prjptr;

  void Timestamp();
  void Project_Summary();
  void Procedural_Summary();
  void Procedural_Detail();
  void Structural_Summary();
  void Structural_Detail();
  void OO_Design();
  void Other_Extents();
  void Separate_Modules();


  void Module_Summary(CCCC_Module *module_ptr);
  void Module_Detail(CCCC_Module *module_ptr);
  void Procedural_Detail(CCCC_Module *module_ptr);
  void Structural_Detail(CCCC_Module *module_ptr);

  void Separate_Module_Link(CCCC_Module *module_ptr);

  void Put_Label_Node(string nodeTag, string label, int width=0,
          string ref_name="", string ref_href="",
          CCCC_Record *rec_ptr=0);
  void Put_Metric_Node(string nodeTag, const CCCC_Metric& metric);
  void Put_Metric_Node(string nodeTag, int count, string tag);
  void Put_Metric_Node(string nodeTag, int num, int denom, string tag);
  void Put_Extent_URL(const CCCC_Extent& extent);
  void Put_Extent_Node(const CCCC_Extent& extent, int width=0, bool withDescription=false);
  void Put_Extent_List(CCCC_Record& record,bool withDescription=false);
  void Put_Structural_Details_Node(CCCC_Module *mod,
           CCCC_Project *prj,
           int mask,
           UserelNameLevel nl);

 public:
  static void GenerateReports(CCCC_Project* project, int report_mask,
            const string& outfile, const string& outdir);

  // general-purpose constructor with standard preamble
  CCCC_Xml_Stream(const string& fname, const string& info);

  // destructor with standard trailer
  ~CCCC_Xml_Stream();
};

CCCC_Xml_Stream& operator <<(CCCC_Xml_Stream& os, const string& stg);
CCCC_Xml_Stream& operator <<(CCCC_Xml_Stream& os, const CCCC_Metric& mtc);
CCCC_Xml_Stream& operator <<(CCCC_Xml_Stream& os, const CCCC_Extent& ext);


#endif /* __CCCC_XML_H */


