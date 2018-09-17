/**
 * Dynamic array
 * Jack Andersen
 */

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <deque>

using namespace std;

namespace Lab5
{

typedef int* IntArrayPtr;

void fill_array(int a[], int size);
//Precondition: size is the size of the array a.
//Postcondition: a[0] through a[size-1] have been
//filled with values read from the keyboard.

void sort(int a[], int size);
//Precondition: size is the size of the array a.
//The array elements a[0] through a[size-1] have values.
//Postcondition: The values of a[0] through a[size-1] have been rearranged
//so that a[0] <= a[1] <= ... <= a[size-1].

void swap_values(int& v1, int& v2);
//Interchanges the values of v1 and v2.

int index_of_smallest(const int a[], int start_index, int number_used);
//Precondition: 0 <= start_index < number_used. References array elements have
//values.
//Returns the index i such that a[i] is the smallest of the values
//a[start_index], a[start_index + 1], ..., a[number_used - 1].

void fill_array(int a[], int size)
{
  using namespace std;
  cout << "Enter " << size << " integers.\n";
  for (int index = 0; index < size; index++)
    cin >> a[index];
}

void merge(deque<int>& out, deque<int>& left, deque<int>& right)
{
  out.clear();

  while (!left.empty() && !right.empty()) {
    if (left.front() <= right.front()) {
      out.push_back(left.front());
      left.pop_front();
    } else {
      out.push_back(right.front());
      right.pop_front();
    }
  }

  while (!left.empty()) {
    out.push_back(left.front());
    left.pop_front();
  }
  while (!right.empty()) {
    out.push_back(right.front());
    right.pop_front();
  }
}

void merge_sort(deque<int>& list)
{
  if (list.size() <= 1)
    return;
  deque<int> left, right;
  deque<int>::iterator it;
  int i;
  for (it = list.begin(), i = 0; it != list.end(); ++it, ++i) {
    if (i < list.size() / 2)
      left.push_back(*it);
    else
      right.push_back(*it);
  }
  merge_sort(left);
  merge_sort(right);
  merge(list, left, right);
}

void sort(int a[], int size)
{
  deque<int> the_list;
  for (int i = 0; i < size; ++i)
    the_list.push_back(a[i]);
  merge_sort(the_list);
  int *out_ptr = a;
  for (auto it = the_list.begin(); it != the_list.end(); ++it)
    *out_ptr++ = *it;
}

void swap_values(int& v1, int& v2)
{
  int tmp = v1;
  v1 = v2;
  v2 = tmp;
}

int index_of_smallest(const int a[], int start_index, int number_used)
{
  int ret = -1;
  int min_val = INT_MAX;
  for (int i = start_index; i < number_used; ++i) {
    if (a[i] < min_val) {
      min_val = a[i];
      ret = i;
    }
  }
  return ret;
}

}

int main(int argc, char** argv)
{
  cout << "This program sorts numbers from lowest to highest.\n";

  int array_size;
  cout << "How many numbers will be sorted? ";
  cin >> array_size;

  Lab5::IntArrayPtr a;
  a = new int[array_size];

  Lab5::fill_array(a, array_size);
  Lab5::sort(a, array_size);

  cout << "In sorted order the numbers are:\n";
  for (int index = 0; index < array_size; index++)
    cout << a[index] << " ";
  cout << endl;

  cout << "Press any key to continue\n";
  cin.get();
  delete [] a;
  cout << "Press any key to continue\n";
  cin.get();

  return 0;
}
