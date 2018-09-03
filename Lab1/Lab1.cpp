/**
 * 3n+1 Problem
 * Jack Andersen
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

namespace Lab1
{

/**
 * @brief State for 3n+1 algorithm
 * Iterative state machine for stepping through 3n+1 algorithm until termination
 */
class ThreeNPlusOne
{
  int m_value; /**< Current n value */
public:
  /**
   * Initializing constructor for 3n+1 algorithm
   * @param input initial n value for state machine
   */
  explicit ThreeNPlusOne(int input) : m_value(input) {}

  /**
   * @brief Execute one iteration of 3n+1 algorithm
   * @return True if further steps remain, False on termination
   */
  bool step() {
    if (m_value == 1)
      return false;
    if (m_value & 1) {
      /* Odd - perform 3n+1 */
      m_value = m_value * 3 + 1;
    } else {
      /* Even - perform n/2 */
      m_value /= 2;
    }
    return true;
  }
};

/**
 * @brief Maximum cycle counter for intervals of 3n+1 trials
 */
class Counter
{
  int m_maxCycleCount = 0; /**< Maximum cycle tracker */
public:
  /**
   * @brief Run 3n+1 cycles for a specified interval of n values
   * @param i Minimum n value, inclusive
   * @param j Maximum n value, inclusive
   * @return Maximum cycle count
   */
  int run(int i, int j) {
    for (int k = i; k <= j; ++k) {
      ThreeNPlusOne m(k);
      int cycleCount = 1; /* Include terminating state */
      while (m.step()) { ++cycleCount; }
      m_maxCycleCount = max(m_maxCycleCount, cycleCount);
    }
    return m_maxCycleCount;
  }
};

/**
 * Interval of n values
 */
struct InputPair
{
  int i, j;
};

}

/**
 * Driver for running 3n+1 cycle intervals from user input
 */
int main(int argc, char** argv) {
  cout << "Enter 2 values \"i j\" per line, empty line to finish" << endl;

  vector<Lab1::InputPair> inPairs;
  string s;
  /* Format on a per-line basis */
  while (getline(cin, s))
  {
    if (s.empty()) {
      /* Empty line begins executing pairs */
      for (const Lab1::InputPair& p : inPairs) {
        Lab1::Counter c;
        cout << p.i << ' ' << p.j << ' ' << c.run(p.i, p.j) << endl;
      }
      break;
    } else {
      /* Non-empty line attempts to extract additional pair */
      istringstream tmp(s);
      int i, j;
      tmp >> i >> j;
      if (i > j) {
        cout << "i must be <= j" << endl;
        continue;
      }
      if (tmp.fail()) {
        cout << "Invalid input format" << endl;
        continue;
      }
      inPairs.push_back(Lab1::InputPair{i, j});
    }
  }

  cout << "Press any key to exit" << endl;
  cin.get();

  return 0;
}
