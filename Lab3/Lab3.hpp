/**
 * World Clock arithmetic
 * Jack Andersen
 */

#pragma once
#include "Lab2.hpp"
#include <string>

namespace Lab3
{

class WorldClock : public Lab2::Clock
{
  std::string m_timezone = "GMT"; /**< Timezone string printed alongside clock output */
  double m_offset = 0; /**< Timezone offset indicated alongside clock output */
public:
  /** @brief Initializes clock to 00:00:00 - GMT0 */
  WorldClock() = default;

  /**
   * @brief Initializes clock to hh:mm:ss + TZ offset
   * @param hours
   * @param minutes
   * @param seconds
   * @param timezone String of timezone
   * @param offset Timezone offset
   */
  WorldClock(int hours, int minutes, int seconds, const std::string& timezone, double offset)
  : Lab2::Clock(hours, minutes, seconds), m_timezone(timezone), m_offset(offset) {}

  /** @brief Accesses current timezone string */
  const std::string& timezone() const { return m_timezone; }
  /** @brief Accesses current TZ offset */
  double offset() const { return m_offset; }

  /** @brief Mutates current timezone string */
  void setTimezone(const std::string& timezone) { m_timezone = timezone; }
  /** @brief Mutates current TZ offset */
  void setOffset(double offset) { m_offset = offset; }

  friend std::ostream& operator<<(std::ostream& out, const WorldClock& clk);
  friend std::istream& operator>>(std::istream& in, WorldClock& clk);
};

}
