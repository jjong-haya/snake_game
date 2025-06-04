#include "map.h"
#include <fstream>
#include <ncurses.h>
#include <string>
#include <unistd.h>

MapManager::MapManager()
    : stage(0), mapData(MAP_SIZE, std::vector<int>(MAP_SIZE, 0)) {}

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
