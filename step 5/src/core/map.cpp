#include "map.h"
#include "gate.h"
#include <cstdlib>
#include <fstream>
#include <ncurses.h>
#include <string>
#include <unistd.h>

MapManager::MapManager()
    : stage(0), mapData(MAP_SIZE, std::vector<int>(MAP_SIZE, 0)), wallUp(0),
      wallDown(0), wallLeft(0), wallRight(0) {}

int MapManager::getStage() const { return stage; }
void MapManager::setStage(int s) { stage = s; }

void MapManager::load(const std::string &filename) {

  std::ifstream file(filename);
  if (!file.is_open()) {
    showMessage("파일 열기 실패: " + filename);
    return;
  }

  for (int i = 0; i < MAP_SIZE; ++i) {
    for (int j = 0; j < MAP_SIZE; ++j) {
      int num;
      if (!(file >> num)) {
        showMessage("읽기 실패: (" + std::to_string(i) + "," +
                    std::to_string(j) + ")");
        return;
      }
      mapData[i][j] = num;
    }
  }
  wallUp = wallDown = wallLeft = wallRight = 0;
}

void MapManager::print() const {
  for (int i = 0; i < MAP_SIZE; ++i) {
    for (int j = 0; j < MAP_SIZE; ++j) {
      int cell = mapData[i][j];
      char symbol = getSymbol(cell);

      switch (cell) {
      case 5:
        attron(COLOR_PAIR(4));
        break;
      case 6:
        attron(COLOR_PAIR(5));
        break;
      case 7:
        attron(COLOR_PAIR(2));
        break;
      case 8:
        attron(COLOR_PAIR(6));
        break;
      case 9:
        attron(COLOR_PAIR(7));
        break;
      case 10:
        attron(COLOR_PAIR(8));
        break;
      case 11:
        attron(COLOR_PAIR(9));
        break;
      case 12:
        attron(COLOR_PAIR(10));
        break;
      default:
        attron(COLOR_PAIR(1));
        break;
      }

      mvaddch(i, j, symbol);

      switch (cell) {
      case 5:
        attroff(COLOR_PAIR(4));
        break;
      case 6:
        attroff(COLOR_PAIR(5));
        break;
      case 7:
        attroff(COLOR_PAIR(2));
        break;
      case 8:
        attroff(COLOR_PAIR(6));
        break;
      case 9:
        attroff(COLOR_PAIR(7));
        break;
      case 10:
        attroff(COLOR_PAIR(8));
        break;
      case 11:
        attroff(COLOR_PAIR(9));
        break;
      case 12:
        attroff(COLOR_PAIR(10));
        break;
      default:
        attroff(COLOR_PAIR(1));
        break;
      }
    }
  }
}

char MapManager::getSymbol(int cellType) const {
  switch (cellType) {
  case 0:
    return ' ';
  case 1:
    return '#';
  case 2:
    return 'X';
  case 5:
    return '+';
  case 6:
    return '-';
  case 7:
    return '@';
  case 8:
    return '?';
  case 9:
    return '*';
  case 10:
    return '=';
  case 11:
    return '&';
  case 12:
    return '~';
  default:
    return ' ';
  }
}

void MapManager::printIntro() {
  printCenteredMessage("SNAKE", "Press any key to start", true);
}

void MapManager::printStageIntro() {
  printCenteredMessage("Stage " + std::to_string(stage), "Good Luck!", false);
}

void MapManager::printNextStage() {
  printCenteredMessage("Stage " + std::to_string(stage) + " Clear!",
                       "Press any key to Next Stage", true);
}

bool MapManager::printGameOver() {
  clear();
  std::string title = "Game Over";
  std::string prompt = "Press R to restart or Q to quit";

  mvprintw(LINES / 2 - 1, (COLS - title.size()) / 2, "%s", title.c_str());
  mvprintw(LINES / 2 + 1, (COLS - prompt.size()) / 2, "%s", prompt.c_str());
  refresh();
  while (true) {
    int ch = getch();
    if (ch == 'r' || ch == 'R')
      return true;
    if (ch == 'q' || ch == 'Q')
      return false;
  }
}

bool MapManager::isEmpty(int y, int x) const { return mapData[y][x] == 0; }
int MapManager::getCell(int y, int x) const { return mapData[y][x]; }
void MapManager::setCell(int y, int x, int value) { mapData[y][x] = value; }

void MapManager::showMessage(const std::string &msg) const {
  clear();
  mvprintw(0, 0, "%s", msg.c_str());
  refresh();
  getch();
}

void MapManager::printCenteredMessage(const std::string &title,
                                      const std::string &prompt,
                                      bool waitKey) const {
  clear();
  mvprintw(LINES / 2 - 1, (COLS - title.size()) / 2, "%s", title.c_str());
  mvprintw(LINES / 2 + 1, (COLS - prompt.size()) / 2, "%s", prompt.c_str());
  refresh();
  if (waitKey) {
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
  } else {
    sleep(2);
  }
}

#include "gate.h"
#include "map.h"

void MapManager::WallRandom(GateManager *gateManager, int dir) {
  auto gateA = gateManager->getGateA();
  auto gateB = gateManager->getGateB();

  int right = MAP_SIZE - wallRight - 1;
  int down = MAP_SIZE - wallDown - 1;

  //======================== 상 ==========================

  if (dir == 0 && wallUp + wallDown < MAP_SIZE - 8) {

    int y = ++wallUp;

    std::pair<int, int> Lgate(y, wallLeft);
    std::pair<int, int> Rgate(y, right);

    if (Lgate == gateA) {
      mapData[y + 1][wallLeft] = 7;
      gateManager->setGateA({y + 1, wallLeft});
    } else if (Lgate == gateB) {
      mapData[y + 1][wallLeft] = 7;
      gateManager->setGateB({y + 1, wallLeft});
    }

    if (Rgate == gateA) {
      mapData[y + 1][right] = 7;
      gateManager->setGateA({y + 1, right});
    } else if (Rgate == gateB) {
      mapData[y + 1][right] = 7;
      gateManager->setGateB({y + 1, right});
    }

    for (int x = wallLeft + 1; x < right; ++x) {
      if (std::make_pair(y - 1, x) == gateA) {
        mapData[y][x] = 7;
        gateManager->setGateA({y, x});
        mapData[y - 1][x] = 2;
        continue;
      } else if (std::make_pair(y - 1, x) == gateB) {
        mapData[y][x] = 7;
        gateManager->setGateB({y, x});
        mapData[y - 1][x] = 2;
        continue;
      }
      mapData[y][x] = 1;
      mapData[y - 1][x] = 2;
    }
    mapData[y][wallLeft] = 2;
    mapData[y][right] = 2;
  }

  //======================== 하 ==========================

  else if (dir == 1 && wallUp + wallDown < MAP_SIZE - 8) { // 하

    int y = MAP_SIZE - 1 - (++wallDown);

    std::pair<int, int> Lgate(y, wallLeft);
    std::pair<int, int> Rgate(y, right);

    if (Lgate == gateA) {
      mapData[y - 1][wallLeft] = 7;
      gateManager->setGateB({y - 1, wallLeft});
    } else if (Lgate == gateB) {
      mapData[y - 1][wallLeft] = 7;
      gateManager->setGateB({y - 1, wallLeft});
    }

    if (Rgate == gateA) {
      mapData[y - 1][right] = 7;
      gateManager->setGateB({y - 1, right});
    } else if (Rgate == gateB) {
      mapData[y - 1][right] = 7;
      gateManager->setGateB({y - 1, right});
    }

    for (int x = wallLeft + 1; x < right; ++x) {
      if (std::make_pair(y + 1, x) == gateA) {
        mapData[y + 1][x] = 7;
        gateManager->setGateA({y + 1, x});
        mapData[y + 1][x] = 2;
        continue;
      } else if (std::make_pair(y + 1, x) == gateB) {
        mapData[y + 1][x] = 7;
        gateManager->setGateB({y + 1, x});
        mapData[y + 1][x] = 2;
        continue;
      }
      mapData[y + 1][x] = 2;
      mapData[y][x] = 1;
    }
    mapData[y][wallLeft] = 2;
    mapData[y][right] = 2;

  }

  //======================== 좌 ==========================

  else if (dir == 2 && wallLeft + wallRight < MAP_SIZE - 8) {

    int x = ++wallLeft;

    std::pair<int, int> Ugate(wallUp, x);
    std::pair<int, int> Dgate(down, x);

    if (Ugate == gateA) {
      mapData[wallUp][x + 1] = 7;
      gateManager->setGateA({wallUp, x + 1});
    } else if (Ugate == gateB) {
      mapData[wallUp][x + 1] = 7;
      gateManager->setGateB({wallUp, x + 1});
    }

    if (Dgate == gateA) {
      mapData[wallDown][x + 1] = 7;
      gateManager->setGateA({wallDown, x + 1});
    } else if (Dgate == gateB) {
      mapData[wallDown][x + 1] = 7;
      gateManager->setGateB({wallDown, x + 1});
    }

    for (int y = wallUp; y < MAP_SIZE - wallDown; ++y) {
      if (std::make_pair(y, x - 1) == gateA) {
        mapData[y][x] = 7;
        gateManager->setGateA({y, x});
        mapData[y][x - 1] = 2;
        continue;
      } else if (std::make_pair(y, x - 1) == gateB) {
        mapData[y][x] = 7;
        gateManager->setGateB({y, x});
        mapData[y][x - 1] = 2;
        continue;
      }
      mapData[y][x - 1] = 2;
      mapData[y][x] = 1;
    }
    mapData[wallUp][x] = 2;
    mapData[MAP_SIZE - wallDown - 1][x] = 2;

  }

  //======================== 우 ==========================

  else if (dir == 3 && wallLeft + wallRight < MAP_SIZE - 8) {
    int x = MAP_SIZE - 1 - (++wallRight);

    std::pair<int, int> Ugate(wallUp, x);
    std::pair<int, int> Dgate(down, x);

    if (Ugate == gateA) {
      mapData[wallUp][x - 1] = 7;
      gateManager->setGateA({wallUp, x - 1});
    } else if (Ugate == gateB) {
      mapData[wallUp][x - 1] = 7;
      gateManager->setGateB({wallUp, x - 1});
    }

    if (Dgate == gateA) {
      mapData[wallDown][x - 1] = 7;
      gateManager->setGateA({wallDown, x - 1});
    } else if (Dgate == gateB) {
      mapData[wallDown][x - 1] = 7;
      gateManager->setGateB({wallDown, x - 1});
    }

    for (int y = wallUp; y < MAP_SIZE - wallDown; ++y) {

      if (std::make_pair(y, x + 1) == gateA) {
        mapData[y][x] = 7;
        gateManager->setGateA({y, x});
        mapData[y][x + 1] = 2;
        continue;
      } else if (std::make_pair(y, x + 1) == gateB) {
        mapData[y][x] = 7;
        gateManager->setGateB({y, x});
        mapData[y][x + 1] = 2;
        continue;
      }
      mapData[y][x] = 1;
      mapData[y][x + 1] = 2;
    }
    mapData[wallUp][x] = 2;
    mapData[MAP_SIZE - wallDown - 1][x] = 2;
  }
}
