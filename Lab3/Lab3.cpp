/**
 * World Clock arithmetic
 * Jack Andersen
 */

#include "Lab3.hpp"
#include <iostream>
#include <sstream>

using namespace std;

namespace Lab3
{

/**
 * @brief Outputs formatted world clock to ostream
 */
std::ostream& operator<<(std::ostream& out, const WorldClock& clk) {
  return out << static_cast<const Lab2::Clock&>(clk) << ' ' << clk.m_timezone << ' ' << clk.m_offset;
}

/**
 * @brief Applies extracted stream values to world clock, no change on failure
 */
std::istream& operator>>(std::istream& in, WorldClock& clk) {
  WorldClock newClock;
  in >> static_cast<Lab2::Clock&>(newClock) >> newClock.m_timezone >> newClock.m_offset;
  if (in.fail())
    return in;
  clk = newClock;
  return in;
}

}

/**
 * Driver for running clock with user input
 */
int main(int argc, char** argv) {
  cout << "Enter 5 values \"h m s tzstr offset\" per line to set clock," << endl <<
          "'a' to spring ahead, 'b' to fall back, empty line to exit" << endl;

  Lab3::WorldClock clk;
  string s;
  /* Format on a per-line basis */
  while (getline(cin, s))
  {
    if (s.empty()) {
      /* Empty line exits */
      break;
    } else if (s[0] == 'a') {
      /* 'a' for spring ahead */
      clk.springAhead();
    } else if (s[0] == 'b') {
      /* 'b' for fall back */
      clk.fallBack();
    } else {
      /* Non-empty line attempts to extract time components */
      istringstream tmp(s);
      tmp >> clk;
      if (tmp.fail()) {
        cout << "Invalid input format" << endl;
        continue;
      }
    }
    cout << clk << endl;
  }

  return 0;
}
