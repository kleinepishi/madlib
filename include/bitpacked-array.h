/*Copyright 2017 Josh Marshall*****************************************/

/***********************************************************************
    This file is part of "Marshall's  Datastructures and Algorithms".

    "Marshall's  Datastructures and Algorithms" is free software: you
    can redistribute it and/or modify it under the terms of the GNU
    General Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later
    version.

    "Marshall's  Datastructures and Algorithms" is distributed in the
    hope that it will be useful, but WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Marshall's  Datastructures and Algorithms".  If not, see
    <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef C_BITPACKET_ARRAY_H
#define C_BITPACKED_ARRAY_H

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <string.h>

//Always use runtime checks because this code has not been extensively
//tested
#ifndef DEBUG
#define DEBUG
#define UNDEF_DEBUG
#endif

////////////////////////////////////////////////////////////////////////
//TYPEDEF'S/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef unsigned char bitArray;

////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DEFINITIONS///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Get the bit value at the specified index.
 *
 * @param[in] input The SparseBitpackedArray which contains the set bit
 *
 * @param[in] index The index at which the bit will be read
 *
 * TODO: test
 **********************************************************************/
unsigned char getBitAtIndex(const bitArray *toRead, size_t index){
  return (toRead[index>>3] & (1 << (index & 0x7)) ) != 0;
}


/*******************************************************************//**
 * \brief Set the bit value at the specified index.
 *
 * @param[in,out] input The SparseBitpackedArray which contains the set
 * bit
 *
 * @param[in] index The index at which the bit will be read
 *
 * TODO: test
 **********************************************************************/
void setBitAtIndex(bitArray *toModify, const size_t index,
                                            const unsigned char value){
  #ifdef DEBUG
  value = (value != 0);
  #endif

  //if(value) toModify[index>>4] |= (1 << (index & 0xf));
  //else      toModify[index>>4] = ~((~toModify[index>>4]) |
  //                                              (1 << (index & 0xf)));
  toModify[index>>3] ^= (-((signed char)value) ^ toModify[index>>3]) &
                                                  (1 << (index & 0x7));
  #ifdef DEBUG
  if(value != getBitAtIndex(toModify, index){
    fprintf(stderr, "setBitAtIndex() failure\n");
    fflush(stderr);
    exit(-1);
  }
  #endif
}

#ifdef UNDEF_DEBUG
#undef DEBUG
#undef UNDEF_DEBUG
#endif

#endif
