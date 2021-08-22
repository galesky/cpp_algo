#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <array>
#include "benchmark/benchmark.h"
using std::stringstream;

int Partition(std::vector<int> &v, int start, int end, long& comparisonCount) {
	
	int pivot = end;
	int j = start;
	for(int i=start;i<end;++i){
    comparisonCount++;
		if(v[i]<v[pivot]){
			std::swap(v[i],v[j]);
			++j;
		}
	}
	std::swap(v[j],v[pivot]);
	return j;
	
}

void Quicksort(std::vector<int> &v, int start, int end, long& comparisonCount) {
  comparisonCount++;
	if(start<end){
		int p = Partition(v,start,end, comparisonCount);
		Quicksort(v,start,p-1, comparisonCount);
		Quicksort(v,p+1,end, comparisonCount);
	}
	
}

int linear_search(std::vector<int> haystack, int needle, long& compCount, long& loopCount)
{
  for (int i = 0; i < haystack.size(); i++)
  {
    loopCount++;
    compCount++;
    if (haystack[i] == needle) {
      compCount++;
      return i;
    }
  }
  return -1;
}

// linear search that breaks if if reaches a value on the haystack higher than the target. haystack must be sorted
int op_linear_search(std::vector<int> haystack, int needle, long& compCount, long& loopCount)
{
  // greaten than max value
  compCount++;
  if (needle > haystack.back())
    return -1;

  for (int i = 0; i < haystack.size(); i++)
  {
    loopCount++;
    compCount++;
    if (haystack[i] == needle)
      return i;

    // if we reached a number greater than the target, the item is not there
    compCount++;
    if (haystack[i] > needle)
      return -1;
  }
  return -1;
}

int binary_search(std::vector<int> array, int size, int value, long& compCount, long& loopCount)
{
  int first = 0,       // First array element
      last = size - 1, // Last array element
      middle,          // Mid point of search
      position = -1;   // Position of search value
  bool found = false;  // Flag
  compCount++; // the condition that allows the loop to run for the first time
  while (!found && first <= last)
  {
    loopCount++;
    middle = (first + last) / 2; // Calculate mid point
    compCount++;
    if (array[middle] == value)  // If value is found at mid
    {
      found = true;
      position = middle;
    }
    else if (array[middle] > value) {// If value is in lower half
      compCount++;
      last = middle - 1;
    } else {
      compCount++;
      compCount++; // twice because the comparison above also happened.
      first = middle + 1; // If value is in upper half
    }
    compCount++; // the while comparison
  }
  return position;
}

std::array<long, 5> search(int search_type, std::vector<int> first_line, std::vector<int> second_line, bool loud) {
  long comparisonCount = 0;
  long loopCount = 0;
  long sortCount = 0;
  long setLength = 0;
  long originalLength = 0;
  std::set<int> s;

  switch(search_type) {
    case 0:
      {
        std::set<int> s( first_line.begin(), first_line.end() );
        first_line.assign( s.begin(), s.end() );
        for (const auto &item : second_line)
        {
          int needle_pos = linear_search(first_line, item, comparisonCount, loopCount);
        }
        if (loud) {
          std::cout << "\n";
          std::cout << "num of comps linear: " << comparisonCount; 
        }
        break;

      }
      // linear search
    case 1:
      // optimized linear
      Quicksort(first_line,0,first_line.size()-1, sortCount);
      // removing duplicates is O(n)
      originalLength = (long long) first_line.size();
      first_line.erase( unique( first_line.begin(), first_line.end() ), first_line.end() );
      setLength = (long long) first_line.size();


      for (const auto &item : second_line)
      {
        int needle_pos = op_linear_search(first_line, item, comparisonCount, loopCount);
      }
      if (loud) {
        std::cout << "num of comps optimized: " << comparisonCount;
        std::cout << "\n";
      }

      break;
    case 2:
      // binary
      Quicksort(first_line,0,first_line.size()-1, sortCount);
            // removing duplicates is O(n)
      originalLength = (long long) first_line.size();
      first_line.erase( unique( first_line.begin(), first_line.end() ), first_line.end() );
      setLength = (long long) first_line.size();


      for (const auto &item : second_line)
      {
        int needle_pos = binary_search(first_line, first_line.size(), item, comparisonCount, loopCount);
      }
      if (loud) {
        std::cout << "\n";
        std::cout << "num of comps binary: " << comparisonCount;
      }
      break;
    default:
      break;
  }
  std::array<long, 5> counters = {comparisonCount, loopCount, sortCount, setLength, originalLength};
  return counters;
}

static void BM_vector_search(benchmark::State& state) {
  const int NUM_OF_FILES = 33;
  const int OFFSET = 1;

  std::array<std::array<std::vector<int>, 2>, NUM_OF_FILES> mem_files;

  // grab all files
  for (int i = OFFSET; i <= NUM_OF_FILES; i++) {
    std::ifstream file("./entries/" + std::to_string(i) + ".txt");
    std::vector<int> first_line;
    std::vector<int> second_line;

    std::string line;
    std::string strNumInput;
    bool is_first_line = true;

    while (getline(file, line))
    {
      std::string entry;

      if (is_first_line == true)
      {
        char *c = const_cast<char *>(line.c_str());
        char *token = strtok(c, " ");
        // loop through the string to extract all other tokens
        while (token != NULL && strcmp(token, "\r") != 0)
        {
          first_line.push_back(std::stoi(token));
          token = strtok(NULL, " ");
        }
        is_first_line = false;
      }
      else
      {
        char *c = const_cast<char *>(line.c_str());
        char *token = strtok(c, " ");
        while (token != NULL && strcmp(token, "\r") != 0)
        {
          second_line.push_back(std::stoi(token));
          // printf(" %s\n", token ); //printing each token
          token = strtok(NULL, " ");
        }
      }
    }
    std::array<std::vector<int>, 2> current_file = { first_line, second_line };
    mem_files[i-1] = current_file;
  }
  
  int comparisonCount = 0;
  std::array<long, 5> counters;
  for (auto _ : state) {
    const int file_idx = state.range(0) / 3;
    const int search_method_idx = state.range(0) % 3;

    counters = search(search_method_idx, mem_files[file_idx][0], mem_files[file_idx][1], false);
  }
  state.counters["Comparisons"] = counters[0];
  state.counters["Loops"] = counters[1];
  state.counters["Sort_comparisons"] = counters[2];
  state.counters["set_length"] = counters[3];
  state.counters["original_length"] = counters[4];
    
}
BENCHMARK(BM_vector_search)->DenseRange(0, 98, 1)->Complexity();

BENCHMARK_MAIN();