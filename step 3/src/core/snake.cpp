#include "snake.h"
#include "map.h"
#include <ncurses.h>

Snake::Snake() { reset(); }

void Snake::reset() {
  body = {{10, 10}, {10, 9}, {10, 8}};
  dir = 3;     // RIGHT
  nextDir = 3; // 초기 입력 방향도 RIGHT
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
  //==========설명 2번 tail dir 체크============
  if ((dir == 0 && nextDir == 1) || (dir == 1 && nextDir == 0) ||
      (dir == 2 && nextDir == 3) || (dir == 3 && nextDir == 2)) {
    return false;
  }
  //============================================

  dir = nextDir;
  int ny = body[0].first + dy[dir];
  int nx = body[0].second + dx[dir];
  //==========설명 4번 wall 통과 체크============
  if (!map || map->getCell(ny, nx) == 1) {
    return false;
  }
  //==========설명 3번 body 통과 체크============
  for (auto &curbody : body) {
    if (curbody.first == ny && curbody.second == nx) {
      return false;
    }
  }
  //============================================
  body.insert(body.begin(), {ny, nx});
  body.pop_back();
  return true;
}

int Snake::getLength() const { return body.size(); }
void Snake::setMap(MapManager *m) { map = m; }

pair<int, int> Snake::getHead() const { return body[0]; }

//==========설명 3-2번 grow get============
void Snake::grow() { body.push_back(body.back()); }
//=========설명 3-3번 최소 길이 체크==========
bool Snake::shrink() {
  if (body.size() <= 3) {
    return false;
  }
  body.pop_back();
  return true;
}

void Snake::applySpeedUp() {
  speedDelay -= 0.025;
  if (speedDelay < 0.05)
    speedDelay = 0.05; // 최소 제한
}

void Snake::applySpeedDown() {
  speedDelay += 0.025;
  if (speedDelay > 0.5)
    speedDelay = 0.5; // 최대 제한
}

bool Snake::isOnBody(int y, int x) const {
  for (const auto &part : body) {
    if (part.first == y && part.second == x)
      return true;
  }
  return false;
}

void Snake::toggleReverse() { reverseKey = !reverseKey; }

double Snake::getSpeedDelay() const { return speedDelay; }

bool Snake::isKeyReversed() const { return reverseKey; }
