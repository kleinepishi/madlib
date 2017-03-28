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

#ifndef QUICKMERGE_HPP
#define QUICKMERGE_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <utility>

#include "short-primatives.h"

using std::pair;
using std::size_t;


////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void sortDoubleSizeTPairHighToLow(pair<f64, size_t> *toSort,
                                                          csize_t size);


void sortDoubleSizeTPairLowToHigh(pair<f64, size_t> *toSort,
                                                          csize_t size);

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif