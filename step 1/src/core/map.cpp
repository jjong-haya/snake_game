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
    mvprintw(0, 0, "파일 열기 실패: %s", filename.c_str());
    refresh();
    getch();
    return;
  }

  int num = 0;
  for (int i = 0; i < MAP_SIZE; ++i) {
    for (int j = 0; j < MAP_SIZE; ++j) {
      if (!(file >> num)) {
        mvprintw(0, 0, "읽기 실패: (%d,%d)", i, j);
        refresh();
        getch();
        return;
      }
      mapData[i][j] = num;
    }
  }
}

void MapManager::print() const {
  for (int i = 0; i < MAP_SIZE; ++i) {
    for (int j = 0; j < MAP_SIZE; ++j) {
      switch (mapData[i][j]) {
      case 0:
        mvaddch(i, j, ' ');
        break;
      case 1:
        mvaddch(i, j, '#');
        break;
      case 2:
        mvaddch(i, j, 'X');
        break;
      case 5:
        mvprintw(i, j, "+");
        break;
      case 6:
        mvprintw(i, j, "-");
        break;
      case 7:
        attron(COLOR_PAIR(2));
        mvprintw(i, j, "@");
        attroff(COLOR_PAIR(2));
        break;
      }
    }
  }
}

void MapManager::printStageIntro() {
  clear();
  string title = "Stage " + to_string(stage);
  string prompt = "Good Luck!";

  mvprintw(LINES / 2 - 1, (COLS - title.size()) / 2, "%s", title.c_str());
  mvprintw(LINES / 2 + 1, (COLS - prompt.size()) / 2, "%s", prompt.c_str());

  refresh();
  sleep(2);
}

void MapManager::printNextStage() {
  clear();
  string title = "Stage " + to_string(stage) + " Clear!";
  string prompt = "Press any key to Next Stage";

  mvprintw(LINES / 2 - 1, (COLS - title.size()) / 2, "%s", title.c_str());
  mvprintw(LINES / 2 + 1, (COLS - prompt.size()) / 2, "%s", prompt.c_str());

  refresh();
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

void MapManager::printIntro() {
  clear();
  string title = "SNAKE";
  string prompt = "Press any key to start";

  mvprintw(LINES / 2 - 1, (COLS - title.size()) / 2, "%s", title.c_str());
  mvprintw(LINES / 2 + 1, (COLS - prompt.size()) / 2, "%s", prompt.c_str());

  refresh();
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

bool MapManager::isEmpty(int y, int x) const { return mapData[y][x] == 0; }

int MapManager::getCell(int y, int x) const { return mapData[y][x]; }

void MapManager::setCell(int y, int x, int value) { mapData[y][x] = value; }
