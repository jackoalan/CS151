/**
 * Clock arithmetic
 * Jack Andersen
 */

#pragma once
#include <iostream>

namespace Lab2
{

class Clock
{
  int m_hours = 0; /**< Hours, may be negative until internal call to fixup() */
  int m_minutes = 0; /**< Minutes, may be negative until internal call to fixup() */
  int m_seconds = 0; /**< Seconds, may be negative until internal call to fixup() */

  /** @brief Applies chain of modulo arithmetic to ensure positive attributes */
  void fixup();

public:
  /** @brief Initializes clock to 00:00:00 */
  Clock() = default;

  /**
   * @brief Initializes clock to hh:mm:00
   * @param hours
   * @param minutes
   */
  Clock(int hours, int minutes)
  : m_hours(hours), m_minutes(minutes) { fixup(); }

  /**
   * @brief Initializes clock to hh:mm:ss
   * @param hours
   * @param minutes
   * @param seconds
   */
  Clock(int hours, int minutes, int seconds)
  : m_hours(hours), m_minutes(minutes), m_seconds(seconds) { fixup(); }

  /** @brief Accesses current hours, always positive */
  int hours() const { return m_hours; }
  /** @brief Accesses current minutes, always positive */
  int minutes() const { return m_minutes; }
  /** @brief Accesses current seconds, always positive */
  int seconds() const { return m_seconds; }

  /** @brief Mutates current hours @param hours may be negative */
  void setHours(int hours) { m_hours = hours; fixup(); }
  /** @brief Mutates current minutes @param minutes may be negative */
  void setMinutes(int minutes) { m_minutes = minutes; fixup(); }
  /** @brief Mutates current seconds @param seconds may be negative */
  void setSeconds(int seconds) { m_seconds = seconds; fixup(); }

  /** @brief Spring ahead for beginning DST */
  void springAhead() { ++m_hours; fixup(); }
  /** @brief Fall back for ending DST */
  void fallBack() { --m_hours; fixup(); }

  friend std::ostream& operator<<(std::ostream& out, const Clock& clk);
  friend std::istream& operator>>(std::istream& in, Clock& clk);
};

}
