#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <benchmark/benchmark.h>
using std::stringstream;

int linear_search(std::vector<int> haystack, int needle, int& compCount)
{
  for (int i = 0; i < haystack.size(); i++)
  {
    compCount++;
    if (haystack[i] == needle) {
      compCount++;
      return i;
    }
  }
  return -1;
}

// linear search that breaks if if reaches a value on the haystack higher than the target. haystack must be sorted
int op_linear_search(std::vector<int> haystack, int needle, int& compCount)
{
  // greaten than max value
  compCount++;
  if (needle > haystack.back())
    return -1;

  for (int i = 0; i < haystack.size(); i++)
  {
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

int binary_search(std::vector<int> array, int size, int value, int& compCount)
{
  int first = 0,       // First array element
      last = size - 1, // Last array element
      middle,          // Mid point of search
      position = -1;   // Position of search value
  bool found = false;  // Flag
  compCount++;
  while (!found && first <= last)
  { 
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

void search(int search_type, std::vector<int> first_line, std::vector<int> second_line, bool loud = false) {
  int comparisonCount = 0;  

  switch(search_type) {
    case 1:
      // linear search
      for (const auto &item : second_line)
      {
        int needle_pos = linear_search(first_line, item, comparisonCount);
      }
      if (loud) {
        std::cout << "\n";
        std::cout << "num of comps linear: " << comparisonCount;  
      }
      break;
    case 2:
      // optimized linear
      std::sort(first_line.begin(), first_line.end());

      for (const auto &item : second_line)
      {
        int needle_pos = op_linear_search(first_line, item, comparisonCount);
      }
      if (loud) {
        std::cout << "num of comps optimized: " << comparisonCount;
        std::cout << "\n";
      }

      break;
    default:
      // binary
      std::sort(first_line.begin(), first_line.end());
        for (const auto &item : second_line)
      {
        int needle_pos = binary_search(first_line, first_line.size(), item, comparisonCount);
      }
      if (loud) {
        std::cout << "\n";
        std::cout << "num of comps binary: " << comparisonCount;
      }
      break;
  }
}

static void BM_StringCopy(benchmark::State& state) {
  std::ifstream file("./entries/5.txt");
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

  for (auto _ : state)
    search(state.range(0), first_line, second_line);
}
BENCHMARK(BM_StringCopy)->Arg(1)->Arg(2)->Arg(3);

BENCHMARK_MAIN();