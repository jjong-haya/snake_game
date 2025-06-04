#include <chrono>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "gate.h"
#include "item.h"
#include "map.h"
#include "snake.h"

int main() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  start_color();
  use_default_colors();
  init_color(100, 500, 250, 0);
  init_color(101, 1000, 950, 800);
  init_color(102, 0, 0, 900);
  init_color(110, 200, 1000, 200);
  init_color(111, 1000, 200, 200);

  init_pair(1, 100, 101);
  init_pair(2, COLOR_WHITE, 102);
  init_pair(3, 101, 101);
  init_pair(4, 110, 101);
  init_pair(5, 111, 101);
  init_pair(6, COLOR_YELLOW, 101);
  init_pair(7, COLOR_CYAN, 101);
  init_pair(8, COLOR_MAGENTA, 101);
  init_pair(9, COLOR_WHITE, 101);
  init_pair(10, COLOR_GREEN, 101);
  bkgd(COLOR_PAIR(1));

  std::vector<std::string> Maps = {"src/map/map1.txt", "src/map/map2.txt",
                                   "src/map/map3.txt", "src/map/map4.txt"};

  Snake snake;
  MapManager map;
  ItemManager item;
  GateManager gate;

  map.printIntro();

  for (size_t stage = 0; stage < Maps.size(); stage++) {
    clear();
    map.load(Maps[stage]);
    map.setStage(stage + 1);
    gate.setMap(&map);
    gate.setMaxGates(1);
    snake.reset();
    snake.setMap(&map);
    snake.setGateManager(&gate);
    item.setMap(&map);
    item.setSnake(&snake);
    map.printStageIntro();

    while (true) {
      snake.handleInput();

      static auto lastMove = std::chrono::steady_clock::now();
      auto now = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed = now - lastMove;

      if (elapsed.count() >= snake.getSpeedDelay() + 1) {
        lastMove = now;

        clear();
        item.spawn(map.getStage());
        item.removeItem();
        if (!gate.isActive())
          gate.spawn();
        if ((gate.isUsed() || gate.isExpired()) && gate.isActive())
          gate.clear();
        if (!snake.move() || !item.getItem(snake)) {
          if (!map.printGameOver()) {
            endwin();
            return 0;
          }
          stage--;
          break;
        }
        map.print();
        snake.render();
        refresh();
      }
      usleep(3000);
    }
  }
  endwin();
  return 0;
}
