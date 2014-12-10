/*************************************************************************//**
 * @file main.cpp
 *
 * @brief SOURCE - main file. Contains The Shared Memory Solution.
 *
 * @mainpage Program 4 - Parallel Hash Table (Shared)
 *
 * @section course_section CSC 433
 *
 * @author Charles Bonn, Julian Brackins, Ryan Feather, & Joe Mowry
 *
 * @date Something December 10, 2014
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
 * This file is for running the shared memory solution for our parallelized
 * Hash Table. Three Hash Tables are created to simulate the act of inserting
 * values into our hash table. The first method is performed in serial, to 
 * create a benchmark speed for our timings. The next run uses Static scheduling 
 * and the third run uses Dynamic scheduling. Each run is timed to evaluate the 
 * speedup and efficiency of the two parallel solutions observed in this program. 
 * The program also verifies the amount of entries in each Hash Table. They should 
 * all be the same, the only way they wouldn't be is if one of the Hash Tables failed 
 * to add a certain value (which would happen if the value already exists in the hash 
 * table). This is due to the fact that our hash keys are based off of the value being 
 * read in to the table itself.
 *
 * @section compile_section Compiling and printUsage
 *
 * @par Compiling Instructions:
 *      (Linux) - make
 *
 * @par Usage:
 @verbatim
 ./hash <num_values_to_hash> <num_threads> <chunk_size = 1>
 Please see printUsage() for specifics relating to runing the program
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
 December  8, 2014 Working out kinks in Shared solution
 December  9, 2014 Shared Completed...
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
 * PROTOTYPES
 *
 ******************************************************************************/

string getPtr(node *ptr);
string randomString(int len);
int rng(int min, int max);
double timeOMP(double start, double finish);
void printUsage(char* prog_name);
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
  
  ///list_size variable dictates how large of a data set you'll work with
  ///thread_count is how many threads the program will spawn 
  ///chunk_size is how many tasks each thread will take on (default is 1)
  int list_size;
  int thread_count;  
  int chunk_size = 1;

  ///i = iterator
  int i;

  ///Check number of arguments, 
  ///If not enough then print printUsage. 
  ///If only 3 arguments (prog + thread_count + list_size ), 
  ///Then chunk_size is set to 1. If there 
  ///Are 4 total arguments, the 4th becomes 
  ///chunk_size.
  if ( argc < 3 ) 
    printUsage(argv[0]);
  list_size = strtoll(argv[1], NULL, 10);
  thread_count = strtoll(argv[2], NULL, 10);
  if ( thread_count < 1 || list_size < 0 ) 
    printUsage(argv[0]);
  if ( argc > 3 )
    chunk_size = strtoll(argv[3], NULL, 10);

  string strList[list_size]; 
  ///Set up the Hash Tables.
  HashTable * ht1 = new HashTable(list_size);
  HashTable * ht2 = new HashTable(list_size);
  HashTable * ht3 = new HashTable(list_size);
 
  ///Set up the random Strings in the array. 
  ///We're gonna "cheat" a little in the random 
  ///String gen and append the number of the string 
  ///To the end of the string to ensure that each 
  ///one is completely random and different from 
  ///the other. This hash map uses its string as the 
  ///hash "key" so each one needs to be unique.
# pragma omp parallel for schedule(static, chunk_size) num_threads(thread_count)
  for(i = 0; i < list_size; i++)
    strList[i] = randomString( rng(1, 10 ) ) + to_string(i);
 
  ///Run and time the Serial solution
  start = omp_get_wtime( );
# pragma omp parallel for schedule(dynamic, 1) num_threads(1)
  for( i = 0; i < list_size; i++)
    ht1->AddString(strList[i]);
  finish = omp_get_wtime();
  serial_t = timeOMP(start, finish);
  
  ///Run and time the Parallel (Static) solution
  start = omp_get_wtime();
# pragma omp parallel for schedule(static, chunk_size) num_threads(thread_count)
  for( i = 0; i < list_size; i++)
    ht2->AddString(strList[i]);
  finish = omp_get_wtime();
  static_t = timeOMP(start, finish);

  ///Run and time the Parallel (Dynamic) solution
  start = omp_get_wtime( );
# pragma omp parallel for schedule(dynamic, chunk_size) num_threads(thread_count) 
  for( i = 0; i < list_size; i++)
    ht3->AddString(strList[i]);
  finish = omp_get_wtime( );
  dynamic_t = timeOMP(start, finish);

  ///Calculate Speedup and Efficiency
  static_s = speedup(serial_t, static_t);
  static_e = efficiency(static_s, thread_count);
  dynamic_s = speedup(serial_t, dynamic_t);
  dynamic_e = efficiency(dynamic_s, thread_count);

  ///Print Thread Count, Chunk Size
  printf("Values in Hash: %d | Thread Count: %d | Chunk Size: %d\n\n", list_size, thread_count, chunk_size);
  ///Count the values in each table.
  printf("Counted %d items in Serial  Table\n", ht1->GetTableCount());
  printf("Counted %d items in Static  Table\n", ht2->GetTableCount());
  printf("Counted %d items in Dynamic Table\n", ht3->GetTableCount());
  if( ht1->GetTableCount() == list_size &&
      ht1->GetTableCount() == list_size &&
      ht1->GetTableCount() == list_size )
    printf("All values successfully added to Tables\n\n");
  else
    printf("Not all values added to Hash Tables...\n\n");
    
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

/*************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * Extract the string value a pointer is pointing at...
 *
 * @param[in] ptr - node pointer containing item.
 *
 * @returns value of "item" the nodepointer is pointing at, or NULL if nothing.
 *
 *****************************************************************************/
string getPtr(node *ptr)
{
  if(ptr!=NULL)
    return ptr->item;
  else
    return "NULL";
}

/*************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * Generate a random string!
 *
 * @param[in] len - the length of the string being generated.
 *
 * @returns str - a random string
 *
 *****************************************************************************/
string randomString(int len)
{
   ///generate a random string here
   string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxy";
   int pos;
   while( (int)str.size() != len ) 
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

/*************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * Get the Clock value.
 *
 * @returns time.
 *
 *****************************************************************************/
double timeOMP(double start, double finish)
{
    return finish - start;
}

/*************************************************************************//**
 * @author Christer Karlsson, Julian Brackins
 *
 * @par Description:
 * Print a message explaining how to run the program.
 *
 * @param[in] arg - prog_name
 *
 * @returns none
 *
 *****************************************************************************/
void printUsage(char* prog_name)
{
  ///Print printUsage statement here.
  fprintf(stderr, "printUsage: %s <num_values_to_hash> <num_threads> <chunk_size = 1>\n", prog_name);
  fprintf(stderr, "   num_values_to_hash is how many values program will add to table.\n");
  fprintf(stderr, "   num_threads is the number of threads >= 1\n");
  fprintf(stderr, "   chunk_size is the size of each chunk a thread works on.\n");
  fprintf(stderr, "   params <num_values_to_hash> & <num_threads>  is REQUIRED.\n");
  fprintf(stderr, "   param <chunk_size> defaults to 1 if not provided.\n");
  exit(0);
}


/*************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * Calculation of the program's speedup by comparing the parallel solution
 * to the serial solution.
 *
 * @param[in] t_ser - serial solution
 * @param[in] t_par - parallel solution
 *
 * @return speedup of the program.
 *****************************************************************************/
double speedup(double t_ser, double t_par)
{
    return t_ser/t_par;
}

/*************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 * Calculation of the program's efficiency by taking the speedup calculated
 * with the speedup() function and dividing it by the number of threads the
 * program was run on. This indicates whether or not adding more threads gives
 * a reasonable speedup.
 *
 * @param[in] speedup - calculated speedup of the program
 * @param[in] thread_count - number of threads program ran on.
 *
 * @return efficiency of the program.
 *****************************************************************************/
double efficiency(double speedup, int thread_count)
{
    return (speedup)/( (double)thread_count );
}
                                  
