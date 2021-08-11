#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include <string.h>
using std::stringstream;

int linear_search(std::vector<int> haystack, int needle)
{
  for (int i = 0; i < haystack.size(); i++)
  {
    if (haystack[i] == needle)
      return i;
  }
  return -1;
}

// linear search that breaks if if reaches a value on the haystack higher than the target. haystack must be sorted
int op_linear_search(std::vector<int> haystack, int needle)
{
  // greaten than max value
  if (needle > haystack.back())
    return -1;

  for (int i = 0; i < haystack.size(); i++)
  {
    if (haystack[i] == needle)
      return i;

    // if we reached a number greater than the target, the item is not there
    if (haystack[i] > needle)
      return -1;
  }
  return -1;
}

int binary_search(std::vector<int> array, int size, int value)
{
  int first = 0,       // First array element
      last = size - 1, // Last array element
      middle,          // Mid point of search
      position = -1;   // Position of search value
  bool found = false;  // Flag
  while (!found && first <= last)
  {
    middle = (first + last) / 2; // Calculate mid point
    if (array[middle] == value)  // If value is found at mid
    {
      found = true;
      position = middle;
    }
    else if (array[middle] > value) // If value is in lower half
      last = middle - 1;
    else
      first = middle + 1; // If value is in upper half
  }
  return position;
}

int main()
{
  std::ifstream file("./entries/3.txt");
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

  // unsorted, linear
  for (const auto &item : second_line)
  {
    int needle_pos = linear_search(first_line, item);
    std::cout << " " << std::to_string(needle_pos);
  }

  std::cout << "\n";

  // sort array
  std::sort(first_line.begin(), first_line.end());
  // OPTIONAL: remove duplicates
  first_line.erase(unique(first_line.begin(), first_line.end()), first_line.end());

  // sorted, linear
  for (const auto &item : first_line)
  {
    std::cout << " " << std::to_string(item);
  }

  std::cout << "\n";

  for (const auto &item : second_line)
  {
    int needle_pos = op_linear_search(first_line, item);
    std::cout << " " << std::to_string(needle_pos);
  }
  std::cout << "\n";

  for (const auto &item : second_line)
  {
    int needle_pos = binary_search(first_line, first_line.size(), item);
    std::cout << " " << std::to_string(needle_pos);
  }

  printf("Finished!");
}
