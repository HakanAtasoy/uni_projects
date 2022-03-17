#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <iomanip>

const int KeyedHashTable::PRIME_LIST[PRIME_TABLE_COUNT] =
{
     2,    3,   5,   7,  11,  13,  17,  19,
     23,  29,  31,  37,  41,  43,  47,  53,
     59,  61,  67,  71,  73,  79,  83,  89,
     97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311,
    313, 317, 331, 337, 347, 349, 353, 359,
    367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457,
    461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541
};

int KeyedHashTable::Hash(const std::string& key) const
{
    int s = key.size(), hash = 0 ;
    for(int i = 0 ; i < s; i++)
    {
        hash += ( (int) key[i] ) * PRIME_LIST[i] ; 
    }

    hash %= tableSize ;
    return hash ;
}

void KeyedHashTable::ReHash()
{
    int new_s = FindNearestLargerPrime(2*tableSize) ;
    HashData* captain = new HashData[new_s] ;
    for(int i = 0; i < tableSize; i++)
    {
        if(table[i].key != "" || table[i].key != "_DELETED_")
        {
            captain -> Insert(table[i]) ;
        }
    }
    delete [] table ;
    table = captain ;
    tableSize = new_s ;
}

int KeyedHashTable::FindNearestLargerPrime(int requestedCapacity)
{
    for(int i = 0; i < PRIME_TABLE_COUNT; i++)
    {
        if(PRIME_LIST[i] > requestedCapacity) 
            return PRIME_LIST[i] ;
        if(PRIME_LIST[i] == requestedCapacity) 
            return PRIME_LIST[i+1] ;
    
    }
}

KeyedHashTable::KeyedHashTable()
{
    table = new HashData[2] ;
    tableSize = 2 ;
    occupiedElementCount = 0 ;
}

KeyedHashTable::KeyedHashTable(int requestedCapacity)
{
    int x = FindNearestLargerPrime(requestedCapacity) ;
    table = new HashData[x] ;
    tableSize = x ;
    occupiedElementCount = 0 ;
}

KeyedHashTable::KeyedHashTable(const KeyedHashTable& other)
{
    int ot = other.tableSize ;
    this -> tableSize = ot ;
    this -> occupiedElementCount = other.occupiedElementCount ;
    table = new HashData[ot] ;
    for(int i = 0; i < ot; i++)
    {
        table[i].key = other.table[i].key ;
        table[i].intArray = other.table[i].intArray ;
    }
}

KeyedHashTable& KeyedHashTable::operator=(const KeyedHashTable& other)
{
    int ot = other.tableSize ;
    this -> tableSize = ot;
    this -> occupiedElementCount = other.occupiedElementCount ;
    delete table ;
    table = new HashData[ot] ;
    for(int i = 0; i < ot; i++)
    {
        table[i].key = other.table[i].key ;
        table[i].intArray = other.table[i].intArray ;
    }
    return *this ;
}

KeyedHashTable::~KeyedHashTable()
{

    if (table)
        delete[] table ;
    table = NULL ;
    tableSize = 0 ;
    occupiedElementCount = 0 ;
}

bool KeyedHashTable::Insert(const std::string& key,
                            const std::vector<int>& intArray)
{
    int y = Hash(key), ind ;

    for(int i = 0; ; i++)
    {
        ind = (y + i*i) % tableSize ;
        if(table[ind].key == "")
        {
            table[ind].key = key ;
            table[ind].intArray = intArray ;
            occupiedElementCount++ ;
            break ;
        }
        if(table[ind].key == key) 
            return false ;
    }

    if(occupiedElementCount*EXPAND_THRESHOLD >= tableSize)
        this -> Rehash() ;
    
    return true ;
    
}

bool KeyedHashTable::Remove(const std::string& key)
{
    int me = Hash(key) ;
    for(int i = 0; ; i++)
    {
        ind = (y + i*i) % tableSize ;
        if(table[ind].key == "")
            return false ;
        if(table[ind].key == key) 
        {
            table[ind].key = "_DELETED_" ;
            table[ind].intArray.clear() ;
            return true ;
        }
    }

}

void KeyedHashTable::ClearTable()
{
   for(int i = 0; i < tableSize; i++)
   {
       table[i].key = "" ;
       table[i].intArray.clear() ;
   }
}

bool KeyedHashTable::Find(std::vector<int>& valueOut,
                          const std::string& key) const
{
    int y = Hash(key), ind ;
    for(int i = 0; ; i++)
    {
        ind = (y + i*i) % tableSize ;
        if(table[ind].key == "")
            return false ;
        if(table[ind].key == key) 
        {
            valueOut = table[ind].intArray ;
            return true ;
        }
    }
}

void KeyedHashTable::Print() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    std::cout << "HT:";
    if(occupiedElementCount == 0)
    {
        std::cout << " Empty";
    }
    std::cout << "\n";
    for(int i = 0; i < tableSize; i++)
    {
        if(table[i].key == "") continue;

        std::cout << "[" << std::setw(3) << i << "] ";
        std::cout << "[" << table[i].key << "] ";
        std::cout << "[";
        for(size_t j = 0; j < table[i].intArray.size(); j++)
        {
            std::cout << table[i].intArray[j];
            if((j + 1) != table[i].intArray.size())
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
    std::cout.flush();
}