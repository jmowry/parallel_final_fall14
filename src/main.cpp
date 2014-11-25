/*************************************************************************//**
 * @file main.cpp
 *
 * @brief SOURCE - main file. Contains Table initialization and other stuff
 *
 * @mainpage Program 4 - Parallel Hash Table
 *
 * @section course_section CSC 433
 *
 * @author Charles Bonn, Julian Brackins, Ryan Feather, & Joe Mowry
 *
 * @date Something SOmething Something
 *
 * @par Professor:
 *         Dr. Karlsson
 *
 * @par Course:
 *         CSC 433 - M001 - 3:00pm
 *
 * @par Location:
 *         McLaury - 313
 *
 * @section program_section Program Information
 *
 * @details
 * Hmmmm
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      (Linux) - make
 *
 * @par Usage:
 @verbatim
 ./hash
 @endverbatim
 *
 * @par Modifications and Development Timeline:
 @verbatim
 Date              Modification
 ----------------  --------------------------------------------------------------
 November 20, 2014 Repo set up by Joe
 November 24, 2014  putting together main file / some class structure
 @endverbatim
 *
 ******************************************************************************/

/******************************************************************************
 *
 * INCLUDE
 *
 ******************************************************************************/

#include "../inc/main.h"
#include "../inc/hashtable.h"

/******************************************************************************
 *
 * NAMESPACES
 *
 ******************************************************************************/

using namespace std;

/******************************************************************************
 *
 * GLOBALS
 *
 ******************************************************************************/

// global vars

/******************************************************************************
 *
 * PROTOTYPES
 *
 ******************************************************************************/

string getPtr(node *ptr);

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * This is the starting point to the program.  
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 0 - Program ends
 *
 *****************************************************************************/
int main(int argc, char ** argv)
{
  node *ptr1, *ptr2, *ptr3, *ptr4;
  int  c1,c2,c3,c4;
  
  //default is 10, but can input table size 
  HashTable * ht1 = new HashTable();
  HashTable * ht2 = new HashTable(20);
  

  ht1->AddString("hello");
  
  ptr1 = ht1->LookupString("hello");
  ptr2 = ht1->LookupString("hi");
  ptr3 = ht2->LookupString("hi");

  
  c1 = ht1->GetTableCount();
  
  ht1->AddString("goodbye");
  
  c2 = ht1->GetTableCount();
  c3 = ht2->GetTableCount();
 
  ht1->DeleteString("hello");
  ptr4 = ht1->LookupString("hello");

  c4 = ht1->GetTableCount();
  
  //c1 = 1, c2 = 2, c3 = 0, c4 = 1  
  cout << "counted " << c1 << " item(s) in this table" << endl;
  cout << "counted " << c2 << " item(s) in this table" << endl;
  cout << "counted " << c3 << " item(s) in this table" << endl;
  cout << "counted " << c4 << " item(s) in this table" << endl;
  
  ///ptr1 should succeed, ptr2 & ptr3 fail.
  cout << "found " << getPtr(ptr1) << endl;
  cout << "found " << getPtr(ptr2) << endl;
  cout << "found " << getPtr(ptr3) << endl;
  cout << "found " << getPtr(ptr4) << endl;
  
  delete ht1;
  delete ht2; 
  return 0; 
}

string getPtr(node *ptr)
{
  if(ptr!=NULL)
    return ptr->item;
  else
    return "NULL";
}
