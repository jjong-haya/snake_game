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

bool Snake::handleInput() {
  int ch = getch();
  if (ch == KEY_UP)
    nextDir = 0;
  else if (ch == KEY_DOWN)
    nextDir = 1;
  else if (ch == KEY_LEFT)
    nextDir = 2;
  else if (ch == KEY_RIGHT)
    nextDir = 3;
  return true;
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
