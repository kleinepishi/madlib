/*Copyright 2016-2017 Josh Marshall************************************/

/***********************************************************************
    This file is part of TF-Cluster.

    TF-Cluster is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TF-Cluster is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TF-Cluster.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef UPPER_DIAGONAL_SQUARE_MATRIX_HPP
#define UPPER_DIAGONAL_SQUARE_MATRIX_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <utility>

#include "short-primatives.h"

////////////////////////////////////////////////////////////////////////
//CLASS DEFINITION//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template<typename T> class UpperDiagonalSquareMatrix{
  private:
  T *oneDMatrix;
  size_t n;
  
  public:

/***********************************************************************
 * 
 **********************************************************************/
  size_t XYToW(size_t z, size_t y);

/***********************************************************************
 * 
 **********************************************************************/
  std::pair<size_t, size_t> WToXY(csize_t w);

/***********************************************************************
 * 
 **********************************************************************/
  size_t numberOfElements();
  

/***********************************************************************
 * 
 **********************************************************************/
  UpperDiagonalSquareMatrix();
  

/***********************************************************************
 * 
 **********************************************************************/
  UpperDiagonalSquareMatrix(size_t sideLength);
  

/***********************************************************************
 * 
 **********************************************************************/
  ~UpperDiagonalSquareMatrix();
  

/***********************************************************************
 * 
 **********************************************************************/
  T getValueAtIndex(size_t x, size_t y);
  

/***********************************************************************
 * 
 **********************************************************************/
  T* getReferenceForIndex(size_t x, size_t y);
  

/***********************************************************************
 * 
 **********************************************************************/
  void setValueAtIndex(size_t x, size_t y, T value);
  
  
/***********************************************************************
 * 
 **********************************************************************/
    size_t getSideLength();
  
  
/***********************************************************************
 * 
 **********************************************************************/
    void fill(T value);
  
  
/***********************************************************************
 * 
 **********************************************************************/
    void zeroData();
  
  
};

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif