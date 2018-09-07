/**
 * Clock arithmetic
 * Jack Andersen
 */

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Lab2.hpp"

using namespace std;

namespace Lab2
{

/**
 * @brief signed->unsigned cumulative modulo helper with arbitrary divisor
 * @param quot Reference to pre-set quotient, input may be negative, output may be negative
 * @param rem Reference to pre-set remainder, input may be negative, output always positive
 * @param divisor Positive integer serving as divisor
 */
static void ModDiv(int& quot, int& rem, int divisor) {
  auto theDiv = div(rem, divisor);
  quot += theDiv.quot;
  if (theDiv.rem < 0) {
    --quot;
    rem = theDiv.rem + divisor;
  } else {
    rem = theDiv.rem;
  }
}

/**
 * @brief Applies chain of modulo arithmetic to ensure positive attributes
 */
void Clock::fixup() {
  ModDiv(m_minutes, m_seconds, 60);
  ModDiv(m_hours, m_minutes, 60);
  int days = 0; /* Clock overflows/underflows into this temporary days component */
  ModDiv(days, m_hours, 24);
}

/**
 * @brief Outputs formatted clock to ostream
 */
std::ostream& operator<<(std::ostream& out, const Clock& clk) {
  return out << right << setfill('0') <<
         setw(2) << clk.m_hours << ':' <<
         setw(2) << clk.m_minutes << ':' <<
         setw(2) << clk.m_seconds;
}

/**
 * @brief Applies extracted stream values to clock, no change on failure
 */
std::istream& operator>>(std::istream& in, Clock& clk) {
  Clock newClock;
  in >> newClock.m_hours >> newClock.m_minutes >> newClock.m_seconds;
  if (in.fail())
    return in;
  clk = newClock;
  clk.fixup();
  return in;
}

}

#if 0 /* Disabled for Lab3 */
/**
 * Driver for running clock with user input
 */
int main(int argc, char** argv) {
  cout << "Enter 3 values \"h m s\" per line to set clock," << endl <<
  "'a' to spring ahead, 'b' to fall back, empty line to exit" << endl;

  Lab2::Clock clk;
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
#endif
