#pragma once
#include <memory>
#include <string>

namespace AdventureGame
{

/**
 * Bitmask enumeration for representing movement directions
 */
enum MoveDir
{
  None = 0,
  N = 1,
  E = 2,
  S = 4,
  W = 8
};

/**
 * Types of map cells
 */
enum class CellType
{
  Road,
  Start,
  Shop1,
  Shop2,
  Jungle,
  Gorge,
  Trail,
  Zip
};

class Game;

/**
 * Superclass interface for all map cells
 */
class Cell
{
public:
  virtual ~Cell() = default;

  /**
   * @brief Get map cell type.
   */
  virtual CellType type() const = 0;

  /**
   * @brief Get bitmask of valid move directions.
   * @param myX x coordinate of this cell.
   * @param myY y coordinate of this cell.
   */
  virtual MoveDir dirFlags(const Game& game, int myX, int myY) const = 0;

  /**
   * @brief Print description string for the user.
   */
  virtual void look(const Game& game) const = 0;

  /**
   * @brief Perform special cell-events in response to player arrival. Calls look() by default.
   * @param dir movement direction leading to this arrival
   */
  virtual void arrive(Game& game, MoveDir dir) const;

  /**
   * @brief Accept special cell-commands from the user.
   * @return True if command was valid and processed
   */
  virtual bool command(Game& game, const std::string& line) const { return false; }
};

class Road : public Cell
{
public:
  CellType type() const { return CellType::Road; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
};

class Start : public Road
{
public:
  CellType type() const { return CellType::Start; }
  void look(const Game& game) const;
  bool command(Game& game, const std::string& line) const;
};

class Shop1 : public Cell
{
public:
  CellType type() const { return CellType::Shop1; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
  bool command(Game& game, const std::string& line) const;
};

class Shop2 : public Cell
{
public:
  CellType type() const { return CellType::Shop2; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
  bool command(Game& game, const std::string& line) const;
};

class Jungle : public Cell
{
public:
  CellType type() const { return CellType::Jungle; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
};

class Gorge : public Cell
{
public:
  CellType type() const { return CellType::Gorge; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
  void arrive(Game& game, MoveDir dir) const;
};

class Trail : public Cell
{
public:
  CellType type() const { return CellType::Trail; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
  bool command(Game& game, const std::string& line) const;
};

class Zip : public Cell
{
public:
  CellType type() const { return CellType::Zip; }
  MoveDir dirFlags(const Game& game, int myX, int myY) const;
  void look(const Game& game) const;
  void arrive(Game& game, MoveDir dir) const;
};

/**
 * Player state class. Contains attributes representing player
 * inventory and progress, as well as functions to mutate overall
 * game progression.
 */
class Player
{
  friend class Game;
  int m_x = 0, m_y = 4;
  int m_points = 0;
  int m_tilesWithHarness = 0;
  bool m_hasHarness = false;
  bool m_equippedHarness = false;
public:
  /** Accessors */
  int x() const { return m_x; }
  int y() const { return m_y; }
  int points() const { return m_points; }
  int tilesWithHarness() const { return m_tilesWithHarness; }
  bool hasHarness() const { return m_hasHarness; }
  bool equippedHarness() const { return m_equippedHarness; }

  /** Mutators */
  void addPoints(int points) { m_points += points; }
  void setHasHarness(bool has) { m_hasHarness = has; }
  bool setEquippedHarness(bool equipped);

  /**
   * @brief determine if movement direction is valid, and move player if so
   * @param dir direction to move player
   */
  void move(Game& game, MoveDir dir);

  /**
   * Reset game state, only preserving points
   */
  void reset();

  /**
   * Special reset function to subtract 25 points and reprint the game start message
   */
  void die(Game& game);
};

/**
 * Game architecture class maintaining ownership of map cells, player, and processing game loop iterations
 */
class Game
{
  std::unique_ptr<Cell> m_cells[5][5];
  Player m_player;
  bool m_gameEnd = false;
public:
  /**
   * Default constructor creates all 25 game cells and initializes player
   */
  Game();

  /** Player Accessors */
  Player& player() { return m_player; }
  const Player& player() const { return m_player; }

  /**
   * Prints point string for user
   */
  void points();

  /**
   * Prints map for user
   */
  void map();

  /**
   * Signals game end
   */
  void end() { m_gameEnd = true; }

  /**
   * Obtain map cell pointer
   * @param x x coordinate of cell
   * @param y y coordinate of cell
   * @return Cell pointer or nullptr if out of bounds
   */
  const Cell* cellAt(int x, int y) const;

  /**
   * Obtain map cell of player's present position
   * @return Cell pointer or nullptr if out of bounds
   */
  const Cell* cellAtPlayer() const { return cellAt(m_player.x(), m_player.y()); }

  /**
   * Prints start string for user
   */
  void start();

  /**
   * Performs one loop iteration of user-input, processing, and user-output
   * @return True if game continues, False if game over
   */
  bool proc();
};

}
