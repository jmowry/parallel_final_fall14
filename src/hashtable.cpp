/*************************************************************************//**
 * @file hashtable.cpp
 *
 * @author Charles Bonn, Julian Brackins, Ryan Feather, & Joe Mowry
 *
 * @brief SOURCE - Hashtable class file. Contains hash table implementation
 *
 *****************************************************************************/
 
 /******************************************************************************
 *
 * INCLUDE
 *
 ******************************************************************************/

#include "../inc/hashtable.h"
#include <omp.h>

/******************************************************************************
 *
 * NAMESPACES
 *
 ******************************************************************************/

using namespace std;

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Initialize Hash Table.
 *
 * @returns none
 *
 *****************************************************************************/
HashTable::HashTable() : HashTable(10)
{
}

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Initialize Hash Table.
 *
 * @returns none
 *
 *****************************************************************************/
HashTable::HashTable(int sz) : size(sz), hash_ptr( CreateHashTable() )
{
  //cout << "HashTable Created with size of " << GetTableSize() << endl;
	omp_init_lock(&write_lock);
}

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Destructor for hashtable.
 *
 * @returns none
 *
 *****************************************************************************/
HashTable::~HashTable()
{
  FreeTable();
  //cout << "HashTable Deleted" << endl;
}


/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Create the Hash Table
 *
 * @returns none
 *
 *****************************************************************************/
hash_t *HashTable::CreateHashTable()
{ 
  ///Set up the Hash Table, return a pointer to it. Don't access this pointer
  ///Anywhere outside of methods (hence why it's private in the class)
  hash_t *new_hash;
  int i;
  

  if(GetTableSize() < 1)
    return NULL;

  if ( (new_hash = 
        new hash_t[sizeof(unsigned int)] ) == NULL) 
          return NULL;

    
  if( ( new_hash->table = 
        new node*[sizeof(node *) * GetTableSize()] ) == NULL ) 
          return NULL;

  for( i = 0 ; i < GetTableSize(); i++) 
    new_hash->table[i] = NULL;

  return new_hash;
}
  
/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Free up the Table.
 *
 * @returns none
 *
 *****************************************************************************/ 
void HashTable::FreeTable()
{
  ///Free up the table. I think this is wrong because 
  ///my computer was dumping some gross seg faults earlier...
  int i;
  node *hash, *temp_hash;

  if (hash_ptr==NULL) 
    return;

  ///Free each item in table.
  for(i=0; i< GetTableSize(); i++) 
  {
      hash = hash_ptr->table[i];
      while(hash!=NULL) 
      {
          temp_hash = hash;
          hash = hash->next;

          ///Program won't let me do this, might be causing mem leaks idk man
          ///delete temp_hash;
          temp_hash = NULL;
      }
  }

  ///Wipe the table
  delete hash_ptr->table;
  delete hash_ptr;
}
  
/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Hashing method.This is just a placeholder so if someone else has a better
 * idea then definitely change this. I'm guessing this is where the 
 * parallelism stuff will be inserted.
 *
 * @returns hash table value % table size so that it fits within table range
 *
 *****************************************************************************/
unsigned int HashTable::Hash(std::string str)
{
  unsigned int hashval;
    
  /// initialize hash to 0
  hashval = 0;
  char* t = const_cast<char*>(str.c_str());
  
  /// Multiply old hash value by 31.
  /// hashval << 5 = multiplying by 2^5 (32) minus hashval = 31.
  /// Shifts are faster than multiplication while still doing what we
  /// want to do. 
  for(; *t != '\0'; t++) 
    hashval = *t + (hashval << 5) - hashval;

  /// mod the hash value by the table size so it fits in the range.
  return hashval % GetTableSize();
}

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Look up a string in the table.
 *
 * @returns pointer to the string in the table. 
 * @returns NULL if string isn't in the list
 *
 *****************************************************************************/
node *HashTable::LookupString(std::string str)
{
  node *node;
  unsigned int hashval = Hash(str);

  ///Look for the item in the table
  for(node = hash_ptr->table[hashval]; node != NULL; node = node->next) \
  {
      if ( str.compare(node->item) == 0) 
        return node;
  }

  return NULL;
}

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Add a string to the hash table
 *
 * @returns true - sucessfully inserted string
 * @returns false - string failed to be insterted in hash table
 *
 *****************************************************************************/
bool HashTable::AddString(std::string str)
{
  node *new_node;
  node *curr_node;
  
  ///Add the string to the hash table
  unsigned int hashval = Hash(str);
  
  if (( new_node = new node[ sizeof( node ) ] ) == NULL )
    return false;
 
  curr_node = LookupString(str);
  
  omp_set_lock(&write_lock);
  if (curr_node != NULL)
  {
    curr_node = LookupString(str);
    if(curr_node != NULL)
    {
        omp_unset_lock(&write_lock);
        return false;
    }
  }

  new_node->item = str;
  new_node->next = hash_ptr->table[hashval];
  hash_ptr->table[hashval] = new_node;

  omp_unset_lock(&write_lock);

  return true;
}
  
/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Remove a string in the hash table
 *
 * @returns true - sucessfully removed string
 * @returns false - string failed to be removed in hash table
 *
 *****************************************************************************/
bool HashTable::DeleteString(std::string str)
{
  node *prev_node;
  node *curr_node;
  
  unsigned int hashval = Hash(str);
  
  for(prev_node = NULL, curr_node = hash_ptr->table[hashval]; 
      curr_node != NULL && str.compare(curr_node->item);
      prev_node = curr_node, curr_node = curr_node->next);  
      
  if(curr_node == NULL )
    return false;
    
  if(prev_node == NULL)
    hash_ptr->table[hashval] = curr_node->next;
  else
    prev_node->next = curr_node->next;
  
  ///free memory


  return true;
}
/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Get Table Size
 *
 * @returns table size
 *
 ******************************************************************************/  
int HashTable::GetTableSize() { return size; }

/**************************************************************************//**
 * @author Julian Brackins
 *
 * @par Description:
 *
 * Get number of items in the hash table.
 *
 * @returns total # of items in Hash table
 * @returns -1 if hashtable doesn't exist
 *
 ******************************************************************************/  
int HashTable::GetTableCount() 
{

  int count = 0;
  
  node* ptr;
  
  ///Check if table exists
  if( hash_ptr == NULL)
    return -1;
  
  ///Count existing values in the table  
  for( int i = 0; i < GetTableSize(); i++)
  {
    for( ptr = hash_ptr->table[i]; ptr != NULL; ptr = ptr->next)
      count++;
  }
    
  return count; 
}

