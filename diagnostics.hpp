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

#ifndef DIAGNOSTICS_HPP
#define DIAGNOSTICS_HPP


////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <queue>
#include <string>
#include <vector>

#include "short-primatives.h"
#include "upper-diagonal-square-matrix.tpp"

////////////////////////////////////////////////////////////////////////
//NAMESPACE USING///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::queue;
using std::string;
using std::vector;

////////////////////////////////////////////////////////////////////////
//GLOBAL FUNCTION DEFINITIONS///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void printCoincidenceMatrix(UpperDiagonalSquareMatrix<u8> *matrix, 
                                cu8 maxMatch, const vector<string> TFs);




////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#endif
