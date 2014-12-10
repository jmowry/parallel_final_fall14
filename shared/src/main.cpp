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
 ./hash <num_threads> <chunk_size = 1>
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
 December  8, 2014 Shared Finished
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

// None

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
double speedup(double t_ser, double t_par);
double efficiency(double speedup, int thread_count);

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
  ///We Don't Seed Rand so that the 
  ///List of "Random" numbers is the 
  ///Same for every run.
  //srand(time(NULL));
  ///Initialize Timing Variables
  double start, finish, serial_t, static_t, dynamic_t;
  ///Initialize Speedup and Efficiency Variables
  double static_s, static_e, dynamic_s, dynamic_e; 
  ///listSIZE variable dictates how large of a data set you'll work with
  int listSIZE = 50000;
  int i;
  int thread_count;  
  string temp;
  string t;
  int chunk_size = 1;


  string strList[listSIZE]; 

  ///Check number of arguments, 
  ///If not enough then print Usage. 
  ///If only 2 arguments (prog + thread_count), 
  ///Then chunk_size is set to 1. If there 
  ///Are 3 total arguments, the 3rd becomes 
  ///chunk_size.
  if ( argc < 2 ) 
    Usage(argv[0]);
  thread_count = strtoll(argv[1], NULL, 10);
  if (thread_count < 1 ) 
    Usage(argv[0]);
  if ( argc > 2 )
    chunk_size = strtoll(argv[2], NULL, 10);

  ///Set up the Hash Tables.
  HashTable * ht1 = new HashTable(listSIZE);
  HashTable * ht2 = new HashTable(listSIZE);
  HashTable * ht3 = new HashTable(listSIZE);
 
  ///Set up the random Strings in the array. 
  ///We're gonna "cheat" a little in the random 
  ///String gen and append the number of the string 
  ///To the end of the string to ensure that each 
  ///one is completely random and different from 
  ///the other. This hash map uses its string as the 
  ///hash "key" so each one needs to be unique.
# pragma omp parallel for schedule(static, chunk_size) num_threads(thread_count)
  for(i = 0; i < listSIZE; i++)
    strList[i] = RandomString( rng(1, 10 ) ) + to_string(i);
 
  ///Run and time the Serial solution
  start = omp_get_wtime( );
# pragma omp parallel for schedule(dynamic, 1) num_threads(1)
  for( i = 0; i < listSIZE; i++)
    ht1->AddString(strList[i]);
  finish = omp_get_wtime();
  serial_t = finish-start;
  

  ///Run and time the Parallel (Static) solution
  start = omp_get_wtime();
# pragma omp parallel for schedule(static, chunk_size) num_threads(thread_count)
  for( i = 0; i < listSIZE; i++)
    ht2->AddString(strList[i]);
  finish = omp_get_wtime();
  static_t = finish - start;

  ///Run and time the Parallel (Dynamic) solution
  start = omp_get_wtime( );
# pragma omp parallel for schedule(dynamic, chunk_size) num_threads(thread_count) 
  for( i = 0; i < listSIZE; i++)
    ht3->AddString(strList[i]);
  finish = omp_get_wtime( );
  dynamic_t = finish - start;

  ///Calculate Speedup and Efficiency
  static_s = speedup(serial_t, static_t);
  static_e = efficiency(static_s, thread_count);
  dynamic_s = speedup(serial_t, dynamic_t);
  dynamic_e = efficiency(dynamic_s, thread_count);

  ///Print Thread Count, Chunk Size
  printf("Thread Count: %d | Chunk Size: %d\n", thread_count, chunk_size);
  ///Count the values in each table.
  printf("counted %d items in Serial  Table\n", ht1->GetTableCount());
  printf("counted %d items in Static  Table\n", ht2->GetTableCount());
  printf("counted %d items in Dynamic Table\n", ht3->GetTableCount());

  ///Print Timing Analysis
  printf("Elapsed Serial  Time: %f\n",  serial_t);
  printf("Elapsed Static  Time: %f\n",  static_t); 
  printf("Elapsed Dynamic Time: %f\n\n", dynamic_t);

  printf("Static Statistics:\n");
  printf(" Observed    Speedup: %f\n",  static_s);
  printf(" Observed Efficiency: %f\n",  static_e); 

  printf("Dynamic Statistics:\n");
  printf(" Observed    Speedup: %f\n", dynamic_s);
  printf(" Observed Efficiency: %f\n", dynamic_e);
  ///delete tables
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


/*************************************************************************//**
* @author Julian Brackins
* *
* * @par Description:
* * Calculation of the program's speedup by comparing the parallel solution
* * to the serial solution.
* *
* * @param[in] t_ser - serial solution
* * @param[in] t_par - parallel solution
* *
* * @return speedup of the program.
* *****************************************************************************/
double speedup(double t_ser, double t_par)
{
    return t_ser/t_par;
}

/*************************************************************************//**
* @author Julian Brackins
* *
* * @par Description:
* * Calculation of the program's efficiency by taking the speedup calculated
* * with the speedup() function and dividing it by the number of threads the
* * program was run on. This indicates whether or not adding more threads gives
* * a reasonable speedup.
* *
* * @param[in] speedup - calculated speedup of the program
* * @param[in] thread_count - number of threads program ran on.
* *
* * @return efficiency of the program.
* *****************************************************************************/
double efficiency(double speedup, int thread_count)
{
    return (speedup)/( (double)thread_count );
}
                                  
