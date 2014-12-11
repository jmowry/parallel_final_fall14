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
 * @date 12/9/2014
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
 * This is the distributed version of our parallel hash table.
 * The parallelism is achieved through the use of MPI.
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      (Linux) - make
 *
 * @par Usage:
 @verbatim
 ./hash <input_file> <number_of_strings> <test_file>
 @endverbatim
 *
 * @par Modifications and Development Timeline:
 @verbatim
 Date              Modification
 ----------------  --------------------------------------------------------------
 November 20, 2014 Repo set up by Joe
 November 24, 2014  putting together main file / some class structure
 December 4, 2014 Main distributed code written
 December 9, 2014 Added lookup testing and cleaned code
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
int testTable(HashTable * table, string filename, int rank, int num_workers);

/**************************************************************************//**
 * @author Ryan Feather
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
  int rank, comm;
  int num_workers;
  int hashval;
  int input_size;
  int table_size;
  int insert_count;
  int i;

  //start and finish times
  double start, finish;

  string temp, filename;
  char temp_array[50];


  //Ensure that we have the correct number of arguments 
  if(argc < 4)
  {
    cout << "Usage: ./hash <input_file> <number_of_strings> <test_file>\n";
    return -1;
  }


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //Worker threads are any that are not rank 0
  num_workers = comm - 1;

  //If the root thread
  //    open the input file
  //    determine the table sizes
  //    read in strings
  //        hash it
  //        determine which thread to give it to
  //        send it
  if(rank == 0)
  {
    
    

    ifstream fin;
  
    filename = argv[1];

    fin.open(filename);

    if(!fin)
    {
        cout << "bad shit\n";
    }

    input_size = atoi(argv[2]);

    table_size = (input_size / (num_workers)) * 1.5;

    MPI_Bcast(&table_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    start = MPI_Wtime();
    i = 0;
    while(i < input_size && fin >> temp_array)
    {
        //cout << temp_array << endl;

        hashval = 0;
        char* t = temp_array;
        for(; *t != '\0'; t++)
            hashval = *t + (hashval << 5) - hashval;

        hashval = hashval % num_workers;
        hashval = abs(hashval);

        //cout << hashval << endl;
        MPI_Send(temp_array, 30, MPI_CHAR,1 + hashval, 0, MPI_COMM_WORLD );
        
        i++;
    }

    
    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime();

   
    cout <<"Table created in " << finish - start << " seconds\n";


    //Call the function to lookup values in the table
    filename = argv[3];

    testTable(NULL, filename, rank, num_workers);
  }


  //if a worker thread
  //    create a table based on a given size
  //    when given a string from the root thread
  //        attempt to add it to the table
  if(rank != 0)
  {

    MPI_Bcast(&table_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
   
    //cout << table_size << endl;

    HashTable * table = new HashTable(table_size);

    MPI_Request request;
    int recvflag = 0;
    bool timedout = false;


    //This loop creates a time-dependent, non-blocking recieve
    //It does this by creating a spinning loop that stops after
    //a certain timeout value
    //
    //We do this because no thread knows how many values it will
    //be given
    while(!timedout)
    {
        start = MPI_Wtime();
        MPI_Irecv(temp_array, 30, MPI_CHAR, 0 , 0 , MPI_COMM_WORLD, &request);

        while(recvflag == 0)
        {
            MPI_Test(&request, &recvflag, NULL);
            if((MPI_Wtime() - start) > 0.04)
            {
                timedout = true;
                break;
            }
        }

        recvflag = 0;
        //cout << temp_array << " recieved" << endl;
        
        //Add the value to the table
        table->AddString( string(temp_array));
    }


    insert_count = table->GetTableCount();
    cout <<"Process " << rank << " inserted this many " << table->GetTableCount() << endl;
   

    MPI_Barrier(MPI_COMM_WORLD);

    //Call the function to test value lookup
    testTable(table, "", rank, num_workers);

    delete table;
  }
  


  MPI_Finalize();

  return 0; 
}

string getPtr(node *ptr)
{
  if(ptr!=NULL)
    return ptr->item;
  else
    return "NULL";
}

/**************************************************************************//**
 * @author Ryan Feather
 *
 * @par Description:
 * Takes an existing set of parallel tables and tries to lookup
 * a file full of strings.  
 *
 * @param[in] table - A hash table filled with values
 * @param[in] filename - The file with test values
 * @param[in] rank - The thread rank
 * @param[in] num_workers - The number of worker threads
 *
 * @returns count
 *
 *****************************************************************************/
int testTable(HashTable* table, string filename, int rank, int num_workers)
{
  MPI_Request request;
  int recvflag = 0;
  double start, finish;
  bool timedout = false;
  char temp_array[30];
  int count = 0;
  int hashval;

  if(rank != 0)
  {
      while(!timedout)
      {
        start = MPI_Wtime();
        MPI_Irecv(temp_array, 30, MPI_CHAR, 0 , 0 , MPI_COMM_WORLD, &request);

        while(recvflag == 0)
        {
            MPI_Test(&request, &recvflag, NULL);
            if((MPI_Wtime() - start) > 0.04)
            {
                timedout = true;
                break;
            }
        }

        recvflag = 0;
        //cout << temp_array << " recieved" << endl;
        
        if(table->LookupString(temp_array))
        {
            count++;
        }
      } 
    MPI_Barrier(MPI_COMM_WORLD);
    cout <<"Process " << rank << " found this many " << count << endl;
  }
  else
  {
    ifstream fin;

    fin.open(filename);

    if(fin)
    {
    

        start = MPI_Wtime();
        while(fin >> temp_array)
        {
            //cout << temp_array << endl;

            count++;

            hashval = 0;
            char* t = temp_array;
            for(; *t != '\0'; t++)
                hashval = *t + (hashval << 5) - hashval;

            hashval = hashval % num_workers;
            hashval = abs(hashval);

            //cout << hashval << endl;
            MPI_Send(temp_array, 30, MPI_CHAR,1 + hashval, 0, MPI_COMM_WORLD );
        }
    } 

    MPI_Barrier(MPI_COMM_WORLD);
    
    finish = MPI_Wtime();

    cout << count << " entries looked up in " << finish - start << " seconds\n";

   

  }


  return count;
}
