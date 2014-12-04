/*************************************************************************//**
 * @file hashtable.h
 *
 * @author Charles Bonn, Julian Brackins, Ryan Feather, & Joe Mowry
 *
 * @brief HEADER - Hashtable class file. Contains hash table interface
 *
 *****************************************************************************/
 
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

/******************************************************************************
 *
 * INCLUDE
 *
 ******************************************************************************/

#include "../inc/main.h"
#include <omp.h>

/******************************************************************************
 *
 * PROTOTYPES
 *
 ******************************************************************************/


/*************************************************************************//**
* @class HashTable
*
* @author Julian Brackins
*
* @brief Hash Table class.
*
*****************************************************************************/
class HashTable
{
protected:
  int size;
  hash_t *hash_ptr;
  omp_lock_t write_lock;

public:

public:
  // CONSTRUCTORS
  HashTable();
  HashTable(int sz);

  // DESTRUCTOR
  ~HashTable();

private:
  hash_t *CreateHashTable();
  void FreeTable();
  
public:
  unsigned int Hash(std::string str);
  node *LookupString(std::string str);
  bool AddString(std::string str);
  bool DeleteString(std::string str);
  
  int GetTableSize();
  int GetTableCount();
};

#endif
