#include "item.h"
#include "map.h"
#include "snake.h"
#include <cstdlib>
#include <ncurses.h>

Item::Item(int y, int x, ItemType type)
    : y(y), x(x), type(type), spawnTime(time(nullptr)) {}

int Item::getY() const { return y; }
int Item::getX() const { return x; }
ItemType Item::getType() const { return type; }
time_t Item::getTime() const { return spawnTime; }

ItemManager::ItemManager() : map(nullptr), snake(nullptr) {}

void ItemManager::setSnake(Snake *s) { snake = s; }
void ItemManager::setMap(MapManager *m) { map = m; }

void ItemManager::spawn(int stage) {
  if (items.size() >= 3 || rand() % 10 >= 3)
    return;

  static const ItemType pool[7] = {ItemType::Growth,    ItemType::Poison,
                                   ItemType::Random,    ItemType::SpeedUp,
                                   ItemType::SpeedDown, ItemType::RandomGate,
                                   ItemType::KeyReverse};

  int maxType = (stage >= 3) ? 7 : 6;
  ItemType type = pool[rand() % maxType];

  int y = rand() % MAP_SIZE;
  int x = rand() % MAP_SIZE;

  if (map->isEmpty(y, x) && snake && !snake->isOnBody(y, x)) {
    items.emplace_back(y, x, type);
    map->setCell(y, x, static_cast<int>(type));
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
      ItemType type = it->getType();
      map->setCell(y, x, 0);
      items.erase(it);

      switch (type) {
      case ItemType::Growth:
        snake.grow();
        break;
      case ItemType::Poison:
        if (!snake.shrink())
          return false;
        break;
      case ItemType::Random:
        if (rand() % 2 == 0)
          snake.grow();
        else if (!snake.shrink())
          return false;
        break;
      case ItemType::SpeedUp:
        snake.applySpeedUp();
        break;
      case ItemType::SpeedDown:
        snake.applySpeedDown();
        break;
      case ItemType::KeyReverse:
        snake.toggleReverse();
        break;
      case ItemType::RandomGate:
        snake.teleportGate();
        break;
      }
      break;
    }
  }
  return true;
}

void ItemManager::reset() {
  for (const auto &item : items) {
    map->setCell(item.getY(), item.getX(), 0);
  }
  items.clear();
}
