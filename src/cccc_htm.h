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
 * \file cccc_htm.h
 * \brief this file defines HTML output facilities for the CCCC project
 */

#ifndef __CCCC_HTM_H
#define __CCCC_HTM_H

#include <fstream>
#include <time.h>

#include "cccc_mod.h"
#include "cccc_rec.h"
#include "cccc_ext.h"
#include "cccc_use.h"
#include "cccc_met.h"

/**
 * Kind of report to insert in the HTML file.
 */
enum ReportType {
  /** show the table of contents */
  rtCONTENTS=0x0001,
  /** show project summary */
  rtSUMMARY=0x0002,
  rtOODESIGN=0x0004,
  /** show procedureal summary */
  rtPROC1=0x0010,
  /** show procedural report */
  rtPROC2=0x0020,
  /** show structural summary */
  rtSTRUCT1=0x0040,
  /** show structural details */
  rtSTRUCT2=0x0080,
  /** show other extents */
  rtOTHER=0x0100,
  rtSEPARATE_MODULES=0x0200,
  rtSOURCE=0x0400,
  /** To show the report's generation time */
  rtSHOW_GEN_TIME=0x800,
  /** show CCCC about */
  rtCCCC=0x8000
};

/**
 * \class CCCC_Html_Stream
 * \brief ?
 */
class CCCC_Html_Stream {
  friend CCCC_Html_Stream& operator <<(CCCC_Html_Stream& os,
               const string& stg);
  friend CCCC_Html_Stream& operator <<(CCCC_Html_Stream& os,
               const CCCC_Metric& mtc);

  ofstream fstr;
  static string libdir;
  static string outdir;
  static CCCC_Project* prjptr;

  /**
   * Create the table of contents.
   *
   * \param report_mask what is in the report
   * report_mask may content rtSHOW_GEN_TIME for testing purposes :
   * we want to be able to disable the inclusion
   * of the current time in the report. This enables us to store a
   * reference version of the report in RCS and expect the program
   * to generate an identical one at regression testing time.
   */
  void Table_Of_Contents(int report_mask);
  void Project_Summary();
  void Procedural_Summary();
  void Procedural_Detail();
  void Structural_Summary();
  void Structural_Detail();
  void OO_Design();
  void Other_Extents();
  void Separate_Modules();
  void Source_Listing();


  void Module_Summary(CCCC_Module *module_ptr);
  void Module_Detail(CCCC_Module *module_ptr);
  void Procedural_Detail(CCCC_Module *module_ptr);
  void Structural_Detail(CCCC_Module *module_ptr);

  /**
   * This function generates a separate HTML report for each non-trivial
   * module in the database.
   */
  void Separate_Module_Link(CCCC_Module *module_ptr);

  /**
   * \fn Put_Section_Heading(string, string, int)
   * \brief Put a tag <Hi> in the HTML file. Where i is the level.
   * \param heading_title title string
   * \param heading_tag name of the link to this tag
   * \param heading_level level of the H tag : H1, H2, ...
   */
  void Put_Section_Heading(string section_name,string section_tag,
         int section_level);
  void Put_Section_TOC_Entry(string section_name, string section_href,
           string section_description);

  void Put_Header_Cell(string label, int width=0);
  void Put_Label_Cell(string label, int width=0,
          string ref_name="", string ref_href="",
          CCCC_Record *rec_ptr=0);
  void Put_Metric_Cell(const CCCC_Metric& metric, int width=0);
  void Put_Metric_Cell(int count, string tag, int width=0);
  void Put_Metric_Cell(int num, int denom, string tag, int width=0);
  void Put_Extent_URL(const CCCC_Extent& extent);
  void Put_Extent_Cell(const CCCC_Extent& extent, int width=0, bool withDescription=false);
  void Put_Extent_List(CCCC_Record& record,bool withDescription=false);
  void Put_Structural_Details_Cell(CCCC_Module *mod,
           CCCC_Project *prj,
           int mask,
           UserelNameLevel nl);

  void Metric_Description(string abbreviation,
        string name,
        string description);

 public:
  static void GenerateReports(CCCC_Project* project, int report_mask,
            const string& outfile, const string& outdir);

  /**
   * \brief General-purpose constructor with standard preamble.
   * \param fname the name of file to create
   * \param info html page's title
   */
  CCCC_Html_Stream(const string& fname, const string& info);

  /**
   * this terminate the output file and close the stream.
   */
  ~CCCC_Html_Stream();
};

CCCC_Html_Stream& operator <<(CCCC_Html_Stream& os, const string& stg);
CCCC_Html_Stream& operator <<(CCCC_Html_Stream& os, const CCCC_Metric& mtc);
CCCC_Html_Stream& operator <<(CCCC_Html_Stream& os, const CCCC_Extent& ext);

/**
 * this class is added to support the generation of an HTML file
 * containing the source analysed by the run, with anchors embedded at
 * each of the lines referred to in the other parts of the report
 */
class Source_Anchor
{
  // if this looks more like a struct to you, it does to me too...
  // it could be embedded withing CCCC_Html_Stream except that this
  // might make the default constructor unavailable for the std::map
  // instantiation

  string file_;
  int line_;
 public:
  Source_Anchor():line_(0) {}
  Source_Anchor(string file, int line) : file_(file), line_(line) {}

  string get_file() const { return file_; }
  int get_line() const { return line_; }
  string key() const;

  void Emit_HREF(ofstream& fstr);

  void Emit_NAME(ofstream& fstr);

  /**
   * Add 10 spaces to stream.
   */
  void Emit_SPACE(ofstream& fstr);
  // the default copy constructor, assignment operator and destructor
  // are OK for this class
};

#endif /* __CCCC_HTM_H */


