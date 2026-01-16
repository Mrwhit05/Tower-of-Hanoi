#pragma once
#include <vector>
using namespace std;
class GameLogic
{
public:
	GameLogic(int diskCount = 5);
	void reset();

	bool selectDisk(int pillar);
	bool placeDisk(int pillar);

	const vector<vector<int>>& getPillars() const;
	bool hasSelectedDisk() const;
	int getSelectedDisk() const;
	int getSelectedFromPillar() const;

	bool isSolved() const;
	int getMoveCount() const;

private:
	vector<vector<int>> pillars;

	bool holding;
	int selectedDisk;
	int fromPillar;

	int moveCount;
	int diskCount;

	bool canMoveInternal(int from, int to) const;
};

