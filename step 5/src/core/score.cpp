#include "score.h"
#include "gate.h"
#include "map.h"
#include "snake.h"
#include <chrono>
#include <ncurses.h>

ScoreManager::ScoreManager() {
  reset(1); // 기본값으로 초기화
}

void ScoreManager::reset(int stage) {
  this->stage = stage;
  growth = poison = gateUsed = randomItem = randomGate = gameTime = 0;
  switch (stage) {
  case 1:
    growthGoal = 2;
    poisonGoal = 1;
    gateGoal = 0;
    randomItemGoal = 0;
    randomGateGoal = 0;
    keyReverseGoal = 0.0;
    break;

  case 2:
    growthGoal = 3;
    poisonGoal = 2;
    gateGoal = 1;
    randomItemGoal = 1;
    randomGateGoal = 1;
    keyReverseGoal = 0.0;
    break;

  case 3:
    growthGoal = 1;
    poisonGoal = 1;
    gateGoal = 1;
    randomItemGoal = 10;
    randomGateGoal = 10;
    keyReverseGoal = 5.0;
    break;

  case 4:
  default:
    growthGoal = 15;
    poisonGoal = 7;
    gateGoal = 4;
    randomItemGoal = 4;
    randomGateGoal = 3;
    keyReverseGoal = 10.0;
    break;
  }

  keyReverseTime = 0.0;
  currentSpeed = 0.2;
  maxSpeed = 0.5;
  gateSpawnTime = std::chrono::steady_clock::now();
  lastShrinkTime = std::chrono::steady_clock::now();
  nextWallDir = "";
}

void ScoreManager::updateGrowth() { growth++; }
void ScoreManager::updatePoison() { poison++; }
void ScoreManager::updateGate() { gateUsed++; }
void ScoreManager::updateRandomItem() { randomItem++; }
void ScoreManager::updateRandomGate() { randomGate++; }

void ScoreManager::updateKeyReverse(double delta) { keyReverseTime += delta; }

void ScoreManager::updateSpeed(double current, double max) {
  currentSpeed = current;
  maxSpeed = max;
}

void ScoreManager::setGateSpawnTime(std::chrono::steady_clock::time_point t) {
  gateSpawnTime = t;
}
void ScoreManager::setGameTime(double t) { gameTime = t; }

void ScoreManager::setShrinkTime(std::chrono::steady_clock::time_point t,
                                 const std::string &dir) {
  lastShrinkTime = t;
  nextWallDir = dir;
}

void ScoreManager::render(const GateManager *gate, const Snake *snake) const {
  int x = MAP_SIZE + 3;
  int y = 1;
  int width = 30;
  int height = (stage >= 3) ? 17 : 16;

  auto now = std::chrono::steady_clock::now();
  double gateLeft =
      20.0 - std::chrono::duration<double>(now - gate->getSpawnClock()).count();
  if (gateLeft < 0)
    gateLeft = 0;
  double shrinkLeft =
      30.0 - std::chrono::duration<double>(now - lastShrinkTime).count();

  // ┌─────── 박스 테두리 ───────┐
  mvaddch(y, x, ACS_ULCORNER);
  mvhline(y, x + 1, ACS_HLINE, width - 2);
  mvaddch(y, x + width - 1, ACS_URCORNER);

  for (int i = 1; i < height - 1; ++i) {
    mvaddch(y + i, x, ACS_VLINE);
    mvaddch(y + i, x + width - 1, ACS_VLINE);
  }

  mvaddch(y + height - 1, x, ACS_LLCORNER);
  mvhline(y + height - 1, x + 1, ACS_HLINE, width - 2);
  mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);

  // ───────── Scoreboard 내용 ─────────
  int yy = y + 1;
  mvprintw(yy++, x + 2, "     [ SCORE BOARD ]");
  mvprintw(yy++, x + 2, "Play Time     : %2.0f", gameTime);
  mvprintw(yy++, x + 2, "Stage         : %2d", stage);
  mvprintw(yy++, x + 2, "Growth        : %2d / %2d", growth, growthGoal);
  mvprintw(yy++, x + 2, "Poison        : %2d / %2d", poison, poisonGoal);
  mvprintw(yy++, x + 2, "Gate Used     : %2d / %2d", gateUsed, gateGoal);
  mvprintw(yy++, x + 2, "Random Item   : %2d / %2d", randomItem,
           randomItemGoal);
  mvprintw(yy++, x + 2, "Random Gate   : %2d / %2d", randomGate,
           randomGateGoal);

  if (stage >= 3)
    mvprintw(yy++, x + 2, "Key Reverse   : %2.0f / %2.0f s", keyReverseTime,
             keyReverseGoal);

  yy++; // 빈 줄
  mvprintw(yy++, x + 2, "Speed         : %.2f / %.2f", maxSpeed - currentSpeed,
           maxSpeed);
  mvprintw(yy++, x + 2, "Gate Left     : %2.0f sec",
           gateLeft > 0 ? gateLeft : 0);
  mvprintw(yy++, x + 2, "Shrink In     : %2.0f sec",
           shrinkLeft > 0 ? shrinkLeft : 0);
  mvprintw(yy++, x + 2, "Next Wall Dir : %s", nextWallDir.c_str());
}

bool ScoreManager::isMissionComplete() const {
  if (growth < growthGoal)
    return false;
  if (poison < poisonGoal)
    return false;
  if (gateUsed < gateGoal)
    return false;
  if (randomItem < randomItemGoal)
    return false;
  if (randomGate < randomGateGoal)
    return false;
  if (stage >= 3 && keyReverseTime < keyReverseGoal)
    return false;
  return true;
}
