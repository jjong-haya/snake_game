#include "item.h"
#include "map.h"
#include "snake.h"
#include <cstdlib>
#include <ncurses.h>

#define ITEM_GROWTH 5
#define ITEM_POISON 6
#define ITEM_RANDOM 8
#define ITEM_SPEEDUP 9
#define ITEM_SPEEDDOWN 10
#define ITEM_RANDOMGATE 11
#define ITEM_KEYREVERSE 12

Item::Item(int y, int x, int type)
    : y(y), x(x), type(type), spawnTime(time(nullptr)) {}

int Item::getY() const { return y; }
int Item::getX() const { return x; }
int Item::getType() const { return type; }
time_t Item::getTime() const { return spawnTime; }

ItemManager::ItemManager() : map(nullptr) {}

void ItemManager::setSnake(Snake *s) { snake = s; }

void ItemManager::setMap(MapManager *m) { map = m; }

void ItemManager::spawn(int stage) {
  if (items.size() >= 3 || rand() % 10 >= 3)
    return;

  static const int pool[7] = {ITEM_GROWTH,    ITEM_POISON,    ITEM_RANDOM,
                              ITEM_SPEEDUP,   ITEM_SPEEDDOWN, ITEM_RANDOMGATE,
                              ITEM_KEYREVERSE};

  int maxType = (stage >= 3) ? 7 : 6;
  int type = pool[rand() % maxType];

  int y = rand() % MAP_SIZE;
  int x = rand() % MAP_SIZE;

  if (map->isEmpty(y, x) && snake && !snake->isOnBody(y, x)) {
    items.emplace_back(y, x, type);
    map->setCell(y, x, type);
    return;
  }
}

void ItemManager::removeItem() {
  time_t now = time(nullptr);
  for (auto it = items.begin(); it != items.end();) {
    if (now - it->getTime() >= 5) {
      map->setCell(it->getY(), it->getX(), 0);
      it = items.erase(it);
    } else {
      ++it;
    }
  }
}

bool ItemManager::getItem(Snake &snake) {
  auto [y, x] = snake.getHead();
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (it->getY() == y && it->getX() == x) {
      int type = it->getType();
      map->setCell(y, x, 0);
      items.erase(it);
      switch (type) {
      case ITEM_GROWTH:
        snake.grow();
        break;
      case ITEM_POISON:
        if (!snake.shrink())
          return false;
        break;
      case ITEM_RANDOM:
        if (rand() % 2 == 0)
          snake.grow();
        else if (!snake.shrink())
          return false;
        break;
      case ITEM_SPEEDUP:
        snake.applySpeedUp();
        break;
      case ITEM_SPEEDDOWN:
        snake.applySpeedDown();
        break;
      case ITEM_KEYREVERSE:
        snake.toggleReverse();
        break;
      case ITEM_RANDOMGATE:
        // Gate 구현 후 만들 예정
        break;
      }

      break;
    }
  }
  return true;
}
