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
#include <algorithm>
#include <list>

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
  //srand(time(NULL));
  double start, finish, serial_t, static_t, dynamic_t;  //Timing variables
  //node *ptr1, *ptr2, *ptr3, *ptr4;
  //int  count;
  int listSIZE = 5000;
  int i;
  int thread_count;  
  string temp;
  string t;
  int chunk_size = 1;
  string strList[5000]; 
  if ( argc < 2 ) 
    Usage(argv[0]);
  thread_count = strtoll(argv[1], NULL, 10);
  if (thread_count < 1 ) 
    Usage(argv[0]);
  if ( argc > 2 )
    chunk_size = strtoll(argv[2], NULL, 10);

  HashTable * ht1 = new HashTable(listSIZE);
  HashTable * ht2 = new HashTable(listSIZE);
  HashTable * ht3 = new HashTable(listSIZE);
  for(i = 0; i < listSIZE; i++)
    strList[i] = RandomString( rng(1, 10 ) ) + to_string(i);
  //Serial solution
  start = omp_get_wtime( );
#pragma omp parallel for schedule(dynamic, 1) num_threads(1)
  for( i = 0; i < listSIZE; i++)
  {
   // t = RandomString( rng(1, 50) );
  //  while(ht1->LookupString(t) != NULL)
  //    ht1->AddString(to_string(i));//RandomString(rng( 1, 50 ) );
    ht1->AddString(strList[i]);
  }
  finish = omp_get_wtime();
  serial_t = finish-start;
  //delete ht1;

  //Static
  start = omp_get_wtime();
#pragma omp parallel for schedule(static, chunk_size) num_threads(thread_count)
  for( i = 0; i < listSIZE; i++)
  {
    ht2->AddString(strList[i]);
  }
  finish = omp_get_wtime();
  static_t = finish - start;

  //Dynamic
  start = omp_get_wtime( );
#pragma omp parallel for schedule(dynamic, chunk_size) num_threads(thread_count) 
  for( i = 0; i < listSIZE; i++)
  {
  // if(ht2->GetTableCount() < 5000)
  // {
   // do
   // {
   //   t = RandomString( rng(1, 50) );
   // }while(ht->LookupString(t) != NULL);
    //while(ht2->LookupString(t) != NULL)
      //ht2->AddString(to_string(i));//RandomString(rng (1,50 ));
      ht3->AddString(strList[i]);
  //  }
  }
  finish = omp_get_wtime( );
  dynamic_t = finish - start;

  //count = ht2->GetTableCount();
 
  printf("counted %d items in Serial  Table\n", ht1->GetTableCount());
  printf("counted %d items in Static  Table\n", ht2->GetTableCount());
  printf("counted %d items in Dynamic Table\n", ht3->GetTableCount());


  

  printf("Elapsed Serial  Time: %f\n",  serial_t);
  printf("Elapsed Static  Time: %f (%d Threads, Chunk Size: %d)\n",
          static_t, thread_count, chunk_size); 
  printf("Elapsed Dynamic Time: %f (%d Threads, Chunk Size: %d)\n", 
          dynamic_t, thread_count, chunk_size);
  //delete ht1;
  delete ht1;
  delete ht2; 
  delete ht3;
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
   while((int)str.size() != len) 
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
