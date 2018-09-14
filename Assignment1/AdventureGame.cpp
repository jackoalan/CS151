#include "AdventureGame.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace AdventureGame
{

/**
 * @brief Trim s whitespace, make lowercase, compare against test string
 * @param s User-input string to compare
 * @param test Test string (the code-defined literal to compare with)
 * @return True if trimmed and lower'd string matches test
 */
static bool CompareEquals(const string& s, const char* test)
{
  string copy = s;
  auto begin = copy.begin();
  auto end = copy.end();
  while (begin != end && ::isspace(*begin)) ++begin;
  while (begin != end && ::isspace(*(end - 1))) --end;
  copy = string(begin, end);
  transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
  return copy == test;
}

void Cell::arrive(Game& game, MoveDir dir) const
{
  look(game);
}

MoveDir Road::dirFlags(const Game& game, int myX, int myY) const
{
  return MoveDir(N | S | E | W);
}

void Road::look(const Game& game) const
{
  cout << "This is a road. It runs north-to-south." << endl;
}

void Start::look(const Game& game) const
{
  cout << "This is a road. It runs north-to-south. Your car is parked nearby." << endl;
}

bool Start::command(Game& game, const string& line) const
{
  if (CompareEquals(line, "get in car")) {
    cout << "You came here for a good time. Don't leave yet!" << endl;
    return true;
  }
  return false;
}

MoveDir Shop1::dirFlags(const Game& game, int myX, int myY) const
{
  return MoveDir(N | S);
}

void Shop1::look(const Game& game) const
{
  cout << "Hey! A zip-line rental shop. Sounds like a good way to spend your afternoon.\n"
          "A sign instructs you to 'rent'." << endl;
}

bool Shop1::command(Game& game, const string& line) const
{
  if (CompareEquals(line, "rent")) {
    if (game.player().hasHarness()) {
      cout << "You already have a harness rented" << endl;
      return true;
    }
    cout << "You sign a 56-page rental agreement in exchange for the zip-line equipment.\n"
            "There's a strict 10-tile rental term according to the fine print." << endl;
    game.player().setHasHarness(true);
    game.player().addPoints(25);
    return true;
  }
  return false;
}

MoveDir Shop2::dirFlags(const Game& game, int myX, int myY) const
{
  return MoveDir(N | S);
}

void Shop2::look(const Game& game) const
{
  cout << "It's another shop that accepts equipment returns on behalf of the first shop.\n"
          "A sign instructs you to 'return'." << endl;
}

bool Shop2::command(Game& game, const string& line) const
{
  if (CompareEquals(line, "return")) {
    cout << "You return the zip-line equipment and leave the shop. What a fun afternoon!" << endl;
    game.player().addPoints(25);
    game.end();
    return true;
  }
  return false;
}

MoveDir Jungle::dirFlags(const Game& game, int myX, int myY) const
{
  return MoveDir(E | W);
}

void Jungle::look(const Game& game) const
{
  cout << "Very thick jungle. You can only really maneuver east or west." << endl;
}

MoveDir Gorge::dirFlags(const Game& game, int myX, int myY) const
{
  return None;
}

void Gorge::look(const Game& game) const
{
  cout << "A deadly drop." << endl;
}

void Gorge::arrive(Game& game, MoveDir dir) const
{
  cout << "AHHHH! You've fallen to your death down a deep gorge. [sad violin plays]." << endl;
  game.player().die(game);
}

MoveDir Trail::dirFlags(const Game& game, int myX, int myY) const
{
  auto ret = MoveDir(E | W);
  if (game.player().equippedHarness()) {
    if (const Cell* c = game.cellAt(myX - 1, myY)) {
      if (c->type() == CellType::Road)
        ret = MoveDir(ret & ~W);
    }
    if (const Cell* c = game.cellAt(myX + 1, myY)) {
      if (c->type() == CellType::Road)
        ret = MoveDir(ret & ~E);
    }
  }
  return ret;
}

void Trail::look(const Game& game) const
{
  cout << "A nice, scenic trail. It runs east-to-west.\n"
          "There is a zip-line you can 'clip' or 'attach' to." << endl;
}

bool Trail::command(Game& game, const string& line) const
{
  if (CompareEquals(line, "attach") ||
      CompareEquals(line, "clip") ||
      CompareEquals(line, "attach to zip line")) {
    if (game.player().setEquippedHarness(true))
      cout << "You clip your harness onto the zip line." << endl;
    else
      cout << "You don't have a harness to clip onto the zip line." << endl;
    return true;
  }
  return false;
}

MoveDir Zip::dirFlags(const Game& game, int myX, int myY) const
{
  return MoveDir(E | W);
}

void Zip::look(const Game& game) const
{
  cout << "A zip line." << endl;
}

void Zip::arrive(Game& game, MoveDir dir) const
{
  if (!game.player().equippedHarness()) {
    if (game.player().hasHarness())
      cout << "AHHHH! It seems you've forgotten to attach your zip harness to the line.\n"
              "[sad violin plays]." << endl;
    else
      cout << "AHHHH! You've fallen to your death down a deep gorge. [sad violin plays]." << endl;
    game.player().die(game);
    return;
  }
  cout << "WOOO! You zip across the gorge and safely arrive at the trail on the other side." << endl;
  game.player().setEquippedHarness(false);
  game.player().addPoints(50);
  game.player().move(game, dir);
}

bool Player::setEquippedHarness(bool equipped)
{
  if (equipped && !m_hasHarness)
    return false;
  m_equippedHarness = equipped;
  return true;
}

void Player::move(Game& game, MoveDir dir)
{
  const Cell* oldCell = game.cellAt(m_x, m_y);
  if ((dir & oldCell->dirFlags(game, m_x, m_y)) != dir) {
    cout << "You can't move in that direction" << endl;
    return;
  }

  int dx = ((dir & E) ? 1 : 0) + ((dir & W) ? -1 : 0);
  int dy = ((dir & N) ? -1 : 0) + ((dir & S) ? 1 : 0);
  int newX = m_x + dx;
  int newY = m_y + dy;
  if (newX < 0 || newX > 4 || newY < 0 || newY > 4) {
    cout << "You run into an arbitrarily-placed forcefield. How annoying!" << endl;
    return;
  }
  m_x = newX;
  m_y = newY;

  if (m_hasHarness && (dx || dy)) {
    ++m_tilesWithHarness;
    if ((m_tilesWithHarness % 10) == 0) {
      cout << "RIIING! You get an angry phone call from the manager of the zip-line rental.\n"
              "He says a collection agency has garnished 25 of your points." << endl;
      m_points -= 25;
    }
  }

  const Cell* newCell = game.cellAt(newX, newY);
  newCell->arrive(game, dir);
}

void Player::reset()
{
  m_x = 0;
  m_y = 4;
  m_tilesWithHarness = 0;
  m_hasHarness = false;
  m_equippedHarness = false;
}

void Player::die(Game& game)
{
  m_points -= 25;
  reset();
  game.start();
}

Game::Game()
{
  m_cells[0][0] = make_unique<Road>();
  m_cells[0][1] = make_unique<Jungle>();
  m_cells[0][2] = make_unique<Gorge>();
  m_cells[0][3] = make_unique<Jungle>();
  m_cells[0][4] = make_unique<Road>();

  m_cells[1][0] = make_unique<Shop1>();
  m_cells[1][1] = make_unique<Jungle>();
  m_cells[1][2] = make_unique<Gorge>();
  m_cells[1][3] = make_unique<Jungle>();
  m_cells[1][4] = make_unique<Road>();

  m_cells[2][0] = make_unique<Road>();
  m_cells[2][1] = make_unique<Jungle>();
  m_cells[2][2] = make_unique<Gorge>();
  m_cells[2][3] = make_unique<Jungle>();
  m_cells[2][4] = make_unique<Road>();

  m_cells[3][0] = make_unique<Road>();
  m_cells[3][1] = make_unique<Trail>();
  m_cells[3][2] = make_unique<Zip>();
  m_cells[3][3] = make_unique<Trail>();
  m_cells[3][4] = make_unique<Road>();

  m_cells[4][0] = make_unique<Start>();
  m_cells[4][1] = make_unique<Jungle>();
  m_cells[4][2] = make_unique<Gorge>();
  m_cells[4][3] = make_unique<Jungle>();
  m_cells[4][4] = make_unique<Shop2>();
}

void Game::points()
{
  cout << "You have " << m_player.points() << " points." << endl;
}

void Game::map()
{
  for (int i = 0; i < 5; ++i) {
    if (i != player().y()) {
      cout << "[ ][ ][ ][ ][ ]" << endl;
      continue;
    }
    for (int j = 0; j < 5; ++j) {
      if (j == player().x())
        cout << "[X]";
      else
        cout << "[ ]";
    }
    cout << endl;
  }
}

const Cell* Game::cellAt(int x, int y) const
{
  if (x >= 0 && x <= 4 && y >= 0 && y <= 4)
    return m_cells[y][x].get();
  return nullptr;
}

void Game::start()
{
  points();
  const Cell* cell = cellAtPlayer();
  cell->arrive(*this, None);
}

bool Game::proc()
{
  string line;
  getline(cin, line);
  const Cell* currentCell = cellAtPlayer();

  if (currentCell->command(*this, line)) {
  } else if (CompareEquals(line, "n")) {
    m_player.move(*this, N);
  } else if (CompareEquals(line, "e")) {
    m_player.move(*this, E);
  } else if (CompareEquals(line, "s")) {
    m_player.move(*this, S);
  } else if (CompareEquals(line, "w")) {
    m_player.move(*this, W);
  } else if (CompareEquals(line, "look")) {
    currentCell->look(*this);
  } else if (CompareEquals(line, "points")) {
    points();
  } else if (CompareEquals(line, "map")) {
    map();
  } else if (CompareEquals(line, "help") || CompareEquals(line, "?")) {
    cout << "'look' for area description\n'map' for map\n'points' for score\n"
            "'n'|'e'|'s'|'w' to move\n'quit' to quit" << endl;
  } else if (CompareEquals(line, "quit")) {
    return false;
  } else {
    cout << "I don't know how to " << line << endl;
  }

  if (m_gameEnd) {
    cout << "You've finished the game! ";
    points();
    return false;
  }

  return true;
}

}

int main(int argc, char** argv)
{
  AdventureGame::Game game;
  game.start();
  while (game.proc()) {}
  cout << "Press any key to exit" << endl;
  cin.get();
  return 0;
}
