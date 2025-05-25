#pragma once
#include <string>
#include <vector>

#define MAP_SIZE 21

using namespace std;

class MapManager {
private:
  int stage;
  vector<vector<int>> mapData;

public:
  MapManager();
  void setStage(int s);
  void load(const string &filename);
  void print() const;
  void printStageIntro();
  void printNextStage();
  void printIntro();
  bool printGameOver();
  int getStage() const;
  bool isEmpty(int y, int x) const;
  int getCell(int y, int x) const;
  void setCell(int y, int x, int value);
};
