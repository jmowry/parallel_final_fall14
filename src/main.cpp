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
#include <iostream>
#include <sstream>
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
string RandomString(int len);
int rng(int min, int max);

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
  srand(time(NULL));
  double start, finish, static_t, dynamic_t;  //Timing variables
  node *ptr1, *ptr2, *ptr3, *ptr4;
  int  c1,c2,c3,c4;
  
  string temp;

  //default is 10, but can input table size 
  HashTable * ht1 = new HashTable();
  HashTable * ht2 = new HashTable(10000000);
  

    ifstream fin;
  start = omp_get_wtime( );
  
#pragma omp parallel for num_threads(14) \
  shared(ht2)
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 10000000; j++)
    {   
        string t = RandomString( rng(1,15) );
        //printf("%s\n", t.c_str());
        //string t = s;
        //cout << s;
        if(!ht2->AddString(t))
            printf("failed to add: %s\n", t.c_str());
        else
            printf("added: %s\n", t.c_str());
    }
    fin.close();
  }
  finish = omp_get_wtime( );
  static_t = finish - start;

  c3 = ht2->GetTableCount();
 
  

  cout << "counted " << c3 << " item(s) in this table" << endl;
  //cout << "counted " << c4 << " item(s) in this table" << endl;
  
  ///ptr1 should succeed, ptr2 & ptr3 fail.
  //cout << "found " << getPtr(ptr1) << endl;
  //cout << "found " << getPtr(ptr2) << endl;
    fin.open("reg.out");

    while(fin >> temp)
    {  
        ptr4 = ht2->LookupString(temp);
        if(getPtr(ptr4)!=("NULL"))
        cout << "found " << getPtr(ptr4) << endl;
    }

  
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


string RandomString(int len)
{
   ///generate a random string here
   string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   int pos;
   while(str.size() != len) 
   {
    pos = ((rand() % (str.size() - 1)));
    str.erase (pos, 1);
   }
   return str;
}

/*************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * Random number generator between two values
 *
 * @param[in] min - minimum value
 * @param[in] max - maximum value
 *
 * @returns random number between min and max
 *
 *****************************************************************************/
int rng(int min, int max)
{
  return rand() % (max-min) +min;
}
