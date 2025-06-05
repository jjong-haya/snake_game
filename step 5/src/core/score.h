#pragma once
#include <chrono>
#include <string>

class GateManager;
class MapManager;
class Snake;

class ScoreManager {
private:
  int stage;

  int growth, poison, gateUsed, randomItem, randomGate;
  double keyReverseTime; // 누적된 유지시간
  double currentSpeed;

  int growthGoal, poisonGoal, gateGoal, randomItemGoal, randomGateGoal;
  double keyReverseGoal, maxSpeed;

  std::chrono::steady_clock::time_point gateSpawnTime;
  std::chrono::steady_clock::time_point lastShrinkTime;
  std::string nextWallDir;
  double gameTime;

public:
  ScoreManager();
  void reset(int stage);

  void updateGrowth();
  void updatePoison();
  void updateGate();
  void updateRandomItem();
  void updateRandomGate();
  void updateKeyReverse(double delta);
  void updateSpeed(double current, double max);

  void setGateSpawnTime(std::chrono::steady_clock::time_point);
  void setShrinkTime(std::chrono::steady_clock::time_point,
                     const std::string &dir);

  void render(const GateManager *gate, const Snake *snake) const;
  bool isMissionComplete() const;
  void setGameTime(double t);
};
