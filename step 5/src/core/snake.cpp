#include "snake.h"
#include "gate.h"
#include "map.h"
#include <ncurses.h>

Snake::Snake() { reset(); }

void Snake::reset() {
  body = {{10, 10}, {10, 9}, {10, 8}};
  dir = 3;
  nextDir = 3;
  speedDelay = 0.2;
  reverseKey = false;
}

void Snake::render() const {
  for (size_t i = 0; i < body.size(); ++i) {
    mvaddch(body[i].first, body[i].second, i == 0 ? 'O' : 'o');
  }
}

void Snake::handleInput() {
  int ch = getch();
  bool rev = reverseKey;
  if (ch == KEY_UP)
    nextDir = rev ? 1 : 0;
  else if (ch == KEY_DOWN)
    nextDir = rev ? 0 : 1;
  else if (ch == KEY_LEFT)
    nextDir = rev ? 3 : 2;
  else if (ch == KEY_RIGHT)
    nextDir = rev ? 2 : 3;
}

bool Snake::move() {
  if ((dir == 0 && nextDir == 1) || (dir == 1 && nextDir == 0) ||
      (dir == 2 && nextDir == 3) || (dir == 3 && nextDir == 2)) {
    return false;
  }

  dir = nextDir;
  int ny = body[0].first + dy[dir];
  int nx = body[0].second + dx[dir];

  if (gateManager && gateManager->isGate(ny, nx)) {
    auto [gy, gx] = gateManager->getOtherGate(ny, nx);
    int outDir = gateManager->getExitDirection(gy, gx, dir);
    if (outDir == -1)
      return false;

    ny = gy + dy[outDir];
    nx = gx + dx[outDir];
    dir = outDir;
    nextDir = outDir;
    gateManager->setActive(true);
    gateManager->setUsed(true);
  } else {
    gateManager->setActive(false);
  }

  int cell = map->getCell(ny, nx);
  if (!map || (cell == 1 && !gateManager->isGate(ny, nx))) {
    return false;
  }

  for (auto &cur : body) {
    if (cur.first == ny && cur.second == nx)
      return false;
  }

  body.insert(body.begin(), {ny, nx});
  body.pop_back();
  return true;
}

int Snake::getLength() const { return body.size(); }
void Snake::setMap(MapManager *m) { map = m; }
std::pair<int, int> Snake::getHead() const { return body[0]; }
void Snake::grow() { body.push_back(body.back()); }

bool Snake::shrink() {
  if (body.size() <= 3)
    return false;
  body.pop_back();
  return true;
}

void Snake::applySpeedUp() {
  speedDelay -= 0.025;
  if (speedDelay < 0.05)
    speedDelay = 0.05;
}

void Snake::applySpeedDown() {
  speedDelay += 0.025;
  if (speedDelay > 0.5)
    speedDelay = 0.5;
}

bool Snake::isOnBody(int y, int x) const {
  for (const auto &part : body) {
    if (part.first == y && part.second == x)
      return true;
  }
  return false;
}

void Snake::toggleReverse() {
  reverseKey = !reverseKey;
  if (reverseKey)
    keyReverseStart = std::chrono::steady_clock::now();
}
double Snake::getSpeedDelay() const { return speedDelay; }
bool Snake::isKeyReversed() const { return reverseKey; }
void Snake::setGateManager(GateManager *g) { gateManager = g; }

void Snake::teleportGate() {
  if (!gateManager)
    return;
  auto gateA = gateManager->getGateA();
  int gy = gateA.first;
  int gx = gateA.second;
  int outDir = gateManager->getExitDirection(gy, gx, dir);
  if (outDir < 0)
    return;
  int ny = gy + dy[outDir];
  int nx = gx + dx[outDir];
  body.insert(body.begin(), {ny, nx});
  body.pop_back();
  dir = outDir;
  nextDir = outDir;
  gateManager->setActive(true);
  gateManager->setUsed(true);
}
