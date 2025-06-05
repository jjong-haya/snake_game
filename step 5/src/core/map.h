#pragma once
#include <string>
#include <vector>

#define MAP_SIZE 21

class GateManager;

class MapManager {
private:
  int stage;
  std::vector<std::vector<int>> mapData;
  int wallUp, wallDown, wallLeft, wallRight;

public:
  MapManager();
  void setStage(int s);
  void load(const std::string &filename);
  void print() const;
  void printStageIntro();
  void printNextStage();
  void printIntro();
  bool printGameOver();
  int getStage() const;
  bool isEmpty(int y, int x) const;
  int getCell(int y, int x) const;
  void setCell(int y, int x, int value);
  char getSymbol(int cellType) const;
  void showMessage(const std::string &msg) const;
  void printCenteredMessage(const std::string &title, const std::string &prompt,
                            bool waitKey) const;
  void WallRandom(GateManager *gateManager, int dir);
};
