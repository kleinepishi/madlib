/*Copyright 2016-2018 Josh Marshall********************************************/

/***********************************************************************
    This file is part of madlib.

    Madlib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Madlib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Madlib.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
@file
@brief Specialized, highly performant sorting algorithms with very low memory
overhead.

TODO: Add benchmarking, reformat, update documentation
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <deque>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef MADLIB_DEBUG
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;
#endif


namespace madlib{

////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Sort an array
 *
 * @param[in, out] toSort Array to sort
 *
 * @param[in] leftIndex Number of elements in array
 *
 * @param[in] cmp Lambda function to compare elements.  Default '<' operator \
 * for low to high
 *
 **********************************************************************/

template<
  typename ForwardIterator,
  typename Compare>
void
timsort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp = std::less_equal<>() );


template<
  typename ForwardIterator>
void
timsort(
  ForwardIterator first,
  ForwardIterator last
){
  timsort(first, last, std::less_equal<>());
}


/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from low to high.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////

template<
  typename ForwardIterator>
void
timsortHighToLow(
  ForwardIterator first,
  ForwardIterator last
){
  timsort(first, last, std::greater_equal<>() );
}


template<
  typename ForwardIterator>
void
timsortLowToHigh(
  ForwardIterator first,
  ForwardIterator last
){
  timsort(first, last);
}


/*******************************************************************************
assumes first != last
*******************************************************************************/
template<
  typename ForwardIterator>
auto
identifyMismatches(
  ForwardIterator first,
  ForwardIterator last
){

  std::deque<ForwardIterator> indicesOfInterest;
  //indicesOfInterest.reserve(std::distance(first, last));
  //indicesOfInterest.clear();

  auto i = first;
  while(std::distance(i, last) > 1){//NOTE may need to be 0, not 1
    indicesOfInterest.push_back(i);
    ++i;
    if(*std::prev(i) < *i){
      for(++i; i != last && *std::prev(i) < *i; ++i);
    }else if(*std::prev(i) > *i){
      for(++i; i != last && *std::prev(i) > *i; ++i);
    }else{
      for(++i; i != last && *std::prev(i) == *i; ++i);
    }
    i--;
  }
  indicesOfInterest.push_back(last);

  indicesOfInterest.shrink_to_fit();

  return indicesOfInterest;
}


template<
  typename ForwardIterator,
  typename Compare>
auto
groomInput(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){
  if(std::distance(first, last) <= 1){
    std::deque<ForwardIterator> early_escape = {first, last};
    exit(-1);
  }
  std::deque<ForwardIterator> indicesOfInterest = identifyMismatches(first, last);
  if(indicesOfInterest.size() == 2){
    if(!comp(*first, *std::prev(last))){
      std::reverse(first, last);
    }
    return indicesOfInterest;
  }

  std::deque<ForwardIterator> nIOI;
  //nIOI.reserve(indicesOfInterest.size()/2 + 1);
  //nIOI.clear();


  //Added to track what should be done for small portions of unsorted data.
  //If a small portion is sorted, ignore.  If a small portion is highly ordered
  //then reverse as needed and merge.  If a small portion is hgihly unordered
  //then call quick sort, insertion sort, or introsort.

  //A section is considered highly unordered if there are many indicies of
  //interest in a short iterator distance, with the particular values for this
  //decision being made as compile time and dependant on the targeted
  //architecture.  This is to take advantage of cache locality.
  constexpr const ssize_t BLOCK_SIZE = 1;//4096 / sizeof(*first);//NOTE: tunable
  //const int HIGHLY_UNORDERED_CUTOFF = 2;

  //nIOI.push_back(*(indicesOfInterest.begin()));
  for(auto i = indicesOfInterest.begin(); *i != last;){
    nIOI.push_back(*i);
    if(std::next(*i) == last || *i == last) break;
    //NOTE: identifyRuns can return an instance when two or more successive
    //groups are in order when an ascending/descending group is followed by a
    //equal group or vica versa and this repeats.  Staircase like input can only
    //be identified as in order robustly this way until identifyRuns is
    //reintegrated into this function.
    if(comp(**i, *std::next(*i))){
      //just skip everything already in order
      //while(*i != last && comp(**i, *std::next(*i))){ // std::next(*i) != last &&
      while(*i != last && std::next(*i) != last && comp(**i, *std::next(*i))){ // std::next(*i) != last &&
        ++i;
      }
    }else{
      auto misorderedStart = i;
      while(*i != last && std::distance(*misorderedStart, *i) < BLOCK_SIZE){
        ++i;
      }
    //   auto misorderedEnd = std::prev(i);
    //   if(misorderedStart != misorderedEnd){
    //   //   std::reverse(*misorderedStart, *i);
    //   // }else{
    //   //   if(*i == last){
    //   //     std::stable_sort(*misorderedStart, last, comp);//NOTE: tunable
    //   //   }else{
    //   //     std::stable_sort(*misorderedStart, *misorderedEnd, comp);//NOTE: tunable
    //   //     //--i;
    //   //   }
    //   // }
     }
  }
  nIOI.push_back(last);

  nIOI.shrink_to_fit();

  return nIOI;
}


//This handles the cases where a given range might be in order or in reverse
//order.
template<
  typename Iter,
  typename Output_Iter,
  typename Compare >
void easy_merge_wrapper(
  Iter leftStart_early,
  Iter rightStart_early,
  Iter endRange_early,
  Output_Iter &output,
  const Compare &comp
){
  bool is_left_in_order = comp(*leftStart_early,  *std::prev(rightStart_early));
  bool is_right_in_order = comp(*rightStart_early, *std::prev(endRange_early));

  //Now, check for the 4 possible cases, and each must be handled differently.
  if( is_left_in_order && is_right_in_order){
    auto left_start  = leftStart_early;
    auto left_end    = rightStart_early;
    auto right_start = rightStart_early;
    auto right_end   = endRange_early;
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }else if( !is_left_in_order && is_right_in_order){
    auto left_start  = std::make_reverse_iterator(rightStart_early);
    auto left_end    = std::make_reverse_iterator(leftStart_early);
    auto right_start = rightStart_early;
    auto right_end   = endRange_early;
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }else if( is_left_in_order && !is_right_in_order){
    auto left_start  = leftStart_early;
    auto left_end    = rightStart_early;
    auto right_start = std::make_reverse_iterator(endRange_early);
    auto right_end   = std::make_reverse_iterator(rightStart_early);
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }else if( !is_left_in_order && !is_right_in_order){
    auto left_start  = std::make_reverse_iterator(rightStart_early);
    auto left_end    = std::make_reverse_iterator(leftStart_early);
    auto right_start = std::make_reverse_iterator(endRange_early);
    auto right_end   = std::make_reverse_iterator(rightStart_early);
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }
}


template<
  typename ForwardIterator,
  typename Compare>
void timsort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){
  typedef typename std::iterator_traits<ForwardIterator>::value_type T;

  if(first == last ||
     std::next(first) == last) return;


  auto tmpIndicesOfInterest = groomInput(first, last, comp);
  //TODO: Here, we can know all of the merge groups and they should be
  //calculated out in advance.  It will require the start, middle, and end
  //indexes as a tuple.  The multithreaded variant will need to tweak this.

  std::vector<T> workspaceIn(std::distance(first, last));


  //First merge out from input data structure///////////////////////////////////
  //Perform first, initial split out loop.  Some of the logic here is every so
  //slightly from the main loop, which is why it needs to be broken out.  This
  //is for the reason that the input data needs to be moved (merged) into a
  //a local work vector, and because this is the only step where it will be
  //possible to have reverse ordered sequences.

  workspaceIn.clear();
  auto end_iter = std::back_inserter(workspaceIn);
  //Tracking actual iterators has been stopped because they care too much about
  //contents and not relative positions.
  std::deque< typename std::vector<T>::difference_type > inIndicesOfInterest;

  while(tmpIndicesOfInterest.size() >= 3){//Remember that the last entry is before end()
    auto one = tmpIndicesOfInterest.front(); tmpIndicesOfInterest.pop_front();
    auto two = tmpIndicesOfInterest.front(); tmpIndicesOfInterest.pop_front();
    auto three = tmpIndicesOfInterest.front();

    easy_merge_wrapper(one, two, three, end_iter, comp);
    inIndicesOfInterest.push_back(std::distance(first, one));
  }
  //handle the trailing elements; make sure they are also in order
  if(tmpIndicesOfInterest.front() != last){
    if(comp(*tmpIndicesOfInterest.front(), *std::prev(last))){
      std::copy(tmpIndicesOfInterest.front(), last, end_iter);
    }else{
      std::copy(std::make_reverse_iterator(last), std::make_reverse_iterator(tmpIndicesOfInterest.front()), end_iter);
    }
  }

  while(tmpIndicesOfInterest.size()){
    inIndicesOfInterest.push_back(std::distance(first, tmpIndicesOfInterest.front()));
    tmpIndicesOfInterest.pop_front();
  }


  //Allocation and reallocation/////////////////////////////////////////////////
  tmpIndicesOfInterest.clear();
  tmpIndicesOfInterest.shrink_to_fit();

  decltype(inIndicesOfInterest) outIndicesOfInterest;

  //In the case we only do two merges, the main merge loop will not run and the
  //second workspace is not needed.  In this case, skip its allocation.  This
  //particularly helps with almost exactly sorted data.
  std::vector< T > workspaceOut;
  if(inIndicesOfInterest.size() > 3){
     workspaceOut.reserve(workspaceIn.size());
  }


  //Primary merging/////////////////////////////////////////////////////////////
  while(inIndicesOfInterest.size() >= 4){//Remember that the last entry is before end()
    outIndicesOfInterest.clear();
    workspaceOut.clear();

    auto inserter = std::back_inserter(workspaceOut);
    while(inIndicesOfInterest.size() >= 3){//Remember that the last entry is before end()
      outIndicesOfInterest.push_back(inIndicesOfInterest.front());
      auto start_iter  = workspaceIn.begin() + inIndicesOfInterest.front(); inIndicesOfInterest.pop_front();
      auto middle_iter = workspaceIn.begin() + inIndicesOfInterest.front(); inIndicesOfInterest.pop_front();
      auto finish_iter = workspaceIn.begin() + inIndicesOfInterest.front();
      //easy_merge_wrapper(start_iter, middle_iter, finish_iter, inserter, comp);
      //We know that in this portion if the number of segments to merge is odd,
      //we will always skip the last one, and every other segment has been
      //handled by easy_merge_handler() which makes sure each of these goupings
      //is already in order and so the overhead of easy_merge_handler is not
      //nessicary.  However, this waiting for a odd merge case is actually
      //harmful for performance, ensuring this case always has an additional
      //loop to merge rather than a copy.
      //TODO: convert odd merge case to evenmerge case.
      //TODO: prioritize small merges first.
      std::merge(start_iter, middle_iter, middle_iter, finish_iter, inserter, comp);
    }

    std::copy(workspaceIn.begin() + inIndicesOfInterest.front(), workspaceIn.end(), inserter);
    std::move(inIndicesOfInterest.begin(), inIndicesOfInterest.end(), std::back_inserter(outIndicesOfInterest));

    workspaceIn.swap(workspaceOut);
    inIndicesOfInterest.swap(outIndicesOfInterest);
  }


  //Here, the merge operation is outputted to a singe work vector, and in order
  //to be efficient, a second work vector is eschewed since the output can be
  //the final array.  This is different from above because this is still more
  //complex and time consuming than a move.

  workspaceOut.clear();
  workspaceOut.shrink_to_fit();

  if(inIndicesOfInterest.size() == 2){
    std::copy(workspaceIn.begin(), workspaceIn.end(), first);
  }else if(inIndicesOfInterest.size() == 3){
    auto start_iter = workspaceIn.begin();
    auto middle_iter = workspaceIn.begin() + inIndicesOfInterest[1];
    auto finish_iter = workspaceIn.end();

    std::merge(start_iter, middle_iter, middle_iter, finish_iter, first, comp);
  }
}



};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
