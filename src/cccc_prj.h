/*
 * cccc_prj.h
 */
#ifndef CCCC_PRJ_H
#define CCCC_PRJ_H

#include "cccc_rec.h"

// forward declarations
class CCCC_Item;
class CCCC_Record;
class CCCC_Project;
class CCCC_Module;
class CCCC_Member;
class CCCC_UseRelationship;
class CCCC_Extent;

static const string REJEXT_PREFIX="CCCC_RejExt";

enum RelationshipMaskElements 
{ 
  rmeCLIENT=0x01, rmeSUPPLIER=0x02, 
  rmeHIDDEN=0x10, rmeVISIBLE=0x20, rmeHIDDEN_OR_VISIBLE=0x30,
  rmeABSTRACT=0x40, rmeCONCRETE=0x80, rmeABSTRACT_OR_CONCRETE=0xC0
};

class CCCC_Project : public CCCC_Record 
{
  friend class CCCC_Html_Stream;
  friend class CCCC_Module;
  friend class CCCC_Member;
  friend class CCCC_UseRelationship;
  friend class CCCC_Extent;

  CCCC_Table<CCCC_Module>          module_table;
  CCCC_Table<CCCC_Member>          member_table;
  CCCC_Table<CCCC_UseRelationship> userel_table;
  CCCC_Table<CCCC_Extent>          rejected_extent_table;

  std::map<string, CCCC_Item> OptionTable;
  

 public: // because MSVC++ version of STL needs it to be...

  // we need a record of which extents came from which files
  // so that when we implement persistence, we can purge
  // extent records from each file as we re-analyze it
  struct ExtentTableEntry
  {
    CCCC_Table<CCCC_Extent> *table_ptr;
    CCCC_Extent *extent_ptr;
    ExtentTableEntry() : table_ptr(NULL), extent_ptr(NULL) {}
  };
  typedef std::multimap<string, ExtentTableEntry> FileExtentTable;
  FileExtentTable file_extent_table;

 public:
  CCCC_Project(const string& name="");

  // these functions are used in both the analyzer 
  // and the load side of the persistence code
  // to add entities to the project
  void add_module(CCCC_Item& module_data_line);
  void add_member(CCCC_Item& member_data_line);    
  void add_userel(CCCC_Item& use_data_line);
  void add_rejected_extent(CCCC_Item& rejected_data_line);

  // this function is used after loading and/or analysis 
  // has been completed to (re)create the maps owned by
  // each module of its members and relationships
  void reindex();

  int get_count(const char *count_tag);

  string name(int level) const;

  int FromFile(ifstream& infile);
  int ToFile(ofstream& outfile);

  void set_option(string key, CCCC_Item& option_data_line);
  int get_option(string key, CCCC_Item& option_data_line);
};

#endif // CCCC_PRJ_H











