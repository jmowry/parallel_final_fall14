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
  int rank, comm;
  int num_workers;
  int hashval;

  string temp, filename;
  char temp_array[50];


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  num_workers = comm - 1;

  //cout << num_workers << endl;

  if(rank == 0)
  {
    ifstream fin;
  
    filename = "../big.big";


    fin.open(filename);

    MPI_Barrier(MPI_COMM_WORLD);

    while(fin >> temp_array)
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
    }
  }

  else
  {
    HashTable * table = new HashTable(1000000);

    double start;
    MPI_Request request;
    int recvflag = 0;
    bool timedout = false;

    MPI_Barrier(MPI_COMM_WORLD);

    while(!timedout)
    {
        start = MPI_Wtime();
        MPI_Irecv(temp_array, 30, MPI_CHAR, 0 , 0 , MPI_COMM_WORLD, &request);

        while(recvflag == 0)
        {
            MPI_Test(&request, &recvflag, NULL);
            if((MPI_Wtime() - start) > 4)
            {
                cout << "illuminarty" << endl;
                timedout = true;
                break;
            }
        }

        recvflag = 0;
        cout << temp_array << " recieved" << endl;
        
        table->AddString( string(temp_array));
    }

    cout <<"Process " << rank << " inserted this many " << table->GetTableCount() << endl;

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