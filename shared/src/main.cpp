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
 November 24, 2014 putting together main file / some class structure
 December  4, 2014 shared implementation started in this file, Ryan moved
                   distributed method into new "branch"
 December     
 @endverbatim
 *
 ******************************************************************************/

/******************************************************************************
 *
 * INCLUDE
 *
 ******************************************************************************/

#include <omp.h>
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
double timeOMP(double start, double finish);
void Usage(char* prog_name);

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

  int thread_count;  
  string temp;

  int chunk_size = 1;
  
  if ( argc < 2 ) 
    Usage(argv[0]);
  thread_count = strtoll(argv[1], NULL, 10);
  if (thread_count < 1 ) 
    Usage(argv[0]);
  if ( argc > 2 )
    chunk_size = strtoll(argv[2], NULL, 10);

  //default is 10, but can input table size 
  HashTable * ht1 = new HashTable();
  //HashTable * ht2 = new HashTable(10000000);
  HashTable * ht2 = new HashTable(2);


  ifstream fin;
  start = omp_get_wtime( );
ht2->AddString("adsfskfjds");
ht2->AddString("fdadasd");
ht2->AddString("jhjkj");
ht2->AddString("ansdf");
ht2->AddString("aasn");
ht2->AddString("fdaa");
ht2->AddString("asdfaa");
ht2->AddString("anddsaffdfdaa");
ht2->AddString("jdfssfdaaaa");
ht2->AddString("andafnaadggfdgaaaa");
ht2->AddString("gfdsgfgfgaaaa");
ht2->AddString("hfghfhfghaaaaaaaaaa");

#pragma omp parallel for schedule(dynamic, chunk_size) num_threads(thread_count) \
  shared(ht2)
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 1; j++)
    {   
        string t = "q";//RandomString( rng(1,50) );
        //printf("%s\n", t.c_str());
        //string t = s;
        //cout << s;
        ht2->AddString(t);
        //    printf("failed to add: %s\n", t.c_str());
        //else
        //    printf("added: %s\n", t.c_str());
    }
    fin.close();
  }
  finish = omp_get_wtime( );
  dynamic_t = finish - start;

  c3 = ht2->GetTableCount();

  

  cout << "counted " << c3 << " item(s) in this table" << endl;
  //cout << "counted " << c4 << " item(s) in this table" << endl;
  
  ///ptr1 should succeed, ptr2 & ptr3 fail.
  cout << "found " << getPtr(ht2->LookupString("jhjkj")) << endl;
  cout << "found " << getPtr(ht2->LookupString("andAgain")) << endl;
  cout << "found " << getPtr(ht2->LookupString("aga")) << endl;
  cout << "found " << getPtr(ht2->LookupString("q")) << endl;
  //cout << "found " << getPtr(ptr2) << endl;
  
  
  fin.open("reg.out");

    while(fin >> temp)
    {  
        ptr4 = ht2->LookupString(temp);
        if(getPtr(ptr4)!=("NULL"))
        cout << "found " << getPtr(ptr4) << endl;
    }

  printf("Elapsed Time: %f\n", dynamic_t);
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
   string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxy";
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
 *  *
 *   * @par Description:
 *    * Get the Clock value.
 *     *
 *      * @returns time.
 *       *
 *        *****************************************************************************/
double timeOMP(double start, double finish)
{
    return finish - start;
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

/*************************************************************************//**
 * @author Christer Karlsson
 *  *
 *   * @par Description:
 *    * Print a message explaining how to run the program.
 *     *
 *      * @param[in] arg - prog_name
 *       *
 *        * @returns none
 *         *
 *          *****************************************************************************/
void Usage(char* prog_name)
{
  ///Print usage statement here.
  fprintf(stderr, "usage: %s <n> <ch>\n", prog_name);
  fprintf(stderr, "   n is the number of threads >= 1\n");
  fprintf(stderr, "   ch is the chunk size for each thread.\n");
  fprintf(stderr, "   param n is REQUIRED.\n");
  fprintf(stderr, "   param ch defaults to 1 if not provided.\n");
  exit(0);
}
