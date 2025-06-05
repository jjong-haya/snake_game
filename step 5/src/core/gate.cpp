#include "gate.h"
#include "map.h"
#include <cstdlib>
#include <ctime>
#include <vector>

GateManager::GateManager()
    : active(false), used(false), map(nullptr), maxGates(1) {}

void GateManager::setMap(MapManager *m) { map = m; }
void GateManager::setMaxGates(int count) { maxGates = count; }
bool GateManager::isActive() const { return active; }
void GateManager::setActive(bool a) { active = a; }
bool GateManager::isUsed() const { return used; }
void GateManager::setUsed(bool u) { used = u; }
bool GateManager::isExpired() const {
  double elapsed = std::chrono::duration<double>(
                       std::chrono::steady_clock::now() - spawnTime)
                       .count();
  return elapsed >= 20.0;
}

std::chrono::steady_clock::time_point GateManager::getSpawnClock() const {
  return spawnTime;
}
void GateManager::setGateA(std::pair<int, int> pos) { gateA = pos; }
void GateManager::setGateB(std::pair<int, int> pos) { gateB = pos; }

void GateManager::reset() {
  gateA = gateB = {-1, -1};
  active = false;
  used = false;
  spawnTime = std::chrono::steady_clock::time_point();
}

void GateManager::clear() {
  if (map && gateA.first != -1 && gateB.first != -1) {
    map->setCell(gateA.first, gateA.second, 1);
    map->setCell(gateB.first, gateB.second, 1);
  }
  reset();
  maxGates++;
}

bool GateManager::isGate(int y, int x) const {
  return std::make_pair(y, x) == gateA || std::make_pair(y, x) == gateB;
}

std::pair<int, int> GateManager::getOtherGate(int y, int x) const {
  return std::make_pair(y, x) == gateA ? gateB : gateA;
}

void GateManager::spawn() {
  if (!map || active || maxGates <= 0)
    return;

  std::vector<std::pair<int, int>> walls;
  for (int y = 0; y < MAP_SIZE; ++y)
    for (int x = 0; x < MAP_SIZE; ++x)
      if (map->getCell(y, x) == 1)
        walls.emplace_back(y, x);

  if (walls.size() < 2)
    return;

  const int dy[4] = {-1, 1, 0, 0};
  const int dx[4] = {0, 0, -1, 1};

  for (int attempt = 0; attempt < 100; ++attempt) {
    int i = rand() % walls.size();
    int j;
    do {
      j = rand() % walls.size();
    } while (j == i);

    auto a = walls[i], b = walls[j];
    int by = b.first, bx = b.second;
    int outDir = -1;

    if (by == 0)
      outDir = 1;
    else if (by == MAP_SIZE - 1)
      outDir = 0;
    else if (bx == 0)
      outDir = 3;
    else if (bx == MAP_SIZE - 1)
      outDir = 2;
    else
      continue;

    int ny = by + dy[outDir];
    int nx = bx + dx[outDir];

    if (ny >= 0 && ny < MAP_SIZE && nx >= 0 && nx < MAP_SIZE &&
        map->getCell(ny, nx) != 1) {
      gateA = a;
      gateB = b;
      map->setCell(gateA.first, gateA.second, 7);
      map->setCell(gateB.first, gateB.second, 7);
      spawnTime = std::chrono::steady_clock::now();
      used = false;
      active = true;
      maxGates--;
      return;
    }
  }
}

int GateManager::getExitDirection(int y, int x, int) const {
  if (!map)
    return -1;

  const int dy[4] = {-1, 1, 0, 0};
  const int dx[4] = {0, 0, -1, 1};
  int candidates[4] = {0, 1, 2, 3};

  if (y == 0)
    candidates[0] = 1;
  else if (y == MAP_SIZE - 1)
    candidates[0] = 0;
  else if (x == 0)
    candidates[0] = 3;
  else if (x == MAP_SIZE - 1)
    candidates[0] = 2;
  else
    return -1;

  for (int i = 0; i < 4; ++i) {
    int d = candidates[i];
    int ny = y + dy[d], nx = x + dx[d];
    if (ny >= 0 && ny < MAP_SIZE && nx >= 0 && nx < MAP_SIZE &&
        map->getCell(ny, nx) != 1)
      return d;
  }
  return -1;
}
