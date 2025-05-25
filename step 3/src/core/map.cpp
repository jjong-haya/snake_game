#include "map.h"
#include <fstream>
#include <ncurses.h>
#include <string>
#include <unistd.h>

MapManager::MapManager()
    : stage(0), mapData(MAP_SIZE, vector<int>(MAP_SIZE, 0)) {}

int MapManager::getStage() const { return stage; }
void MapManager::setStage(int s) { stage = s; }

void MapManager::load(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    showMessage("파일 열기 실패: " + filename);
    return;
  }

  for (int i = 0; i < MAP_SIZE; ++i) {
    for (int j = 0; j < MAP_SIZE; ++j) {
      int num;
      if (!(file >> num)) {
        showMessage("읽기 실패: (" + to_string(i) + "," + to_string(j) + ")");
        return;
      }
      mapData[i][j] = num;
    }
  }
}

void MapManager::print() const {
  for (int i = 0; i < MAP_SIZE; ++i) {
    for (int j = 0; j < MAP_SIZE; ++j) {
      mvaddch(i, j, getSymbol(mapData[i][j]));
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
  printCenteredMessage("Stage " + to_string(stage), "Good Luck!", false);
}

void MapManager::printNextStage() {
  printCenteredMessage("Stage " + to_string(stage) + " Clear!",
                       "Press any key to Next Stage", true);
}

bool MapManager::printGameOver() {
  clear();
  string title = "Game Over";
  string prompt = "Press R to restart or Q to quit";

  mvprintw(LINES / 2 - 1, (COLS - title.size()) / 2, "%s", title.c_str());
  mvprintw(LINES / 2 + 1, (COLS - prompt.size()) / 2, "%s", prompt.c_str());
  refresh();
  while (true) {
    int ch = getch();
    if (ch == 'r' || ch == 'R')
      return true; // 재시작
    if (ch == 'q' || ch == 'Q')
      return false; // 종료
  }
}
bool MapManager::isEmpty(int y, int x) const { return mapData[y][x] == 0; }

int MapManager::getCell(int y, int x) const { return mapData[y][x]; }

void MapManager::setCell(int y, int x, int value) { mapData[y][x] = value; }

void MapManager::showMessage(const string &msg) const {
  clear();
  mvprintw(0, 0, "%s", msg.c_str());
  refresh();
  getch();
}

void MapManager::printCenteredMessage(const string &title, const string &prompt,
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
