#include "GameLogic.h"


GameLogic::GameLogic(int diskCount) : holding(false), selectedDisk(-1), fromPillar(-1), moveCount(0), diskCount(diskCount) {
	reset();
}

void GameLogic::reset() {
	pillars.clear();
	pillars.resize(3);
	for (int i = diskCount; i >= 1; i--) {
		pillars[0].push_back(i);
	}

	holding = false;
	selectedDisk = -1;
	fromPillar = -1;
	moveCount = 0;
}

bool GameLogic::selectDisk(int pillar) {
	if (holding) {
		return 0;
	}
	
	if (pillar < 0 or pillar >= pillars.size()) {
		return 0;
	}

	if (pillars[pillar].empty()) {
		return 0;
	}

	selectedDisk = pillars[pillar].back();
	pillars[pillar].pop_back();

	fromPillar = pillar;
	holding = true;

	return 1;
}

bool GameLogic::placeDisk(int pillar) {
	if (!holding) {
		return 0;
	}

	bool success = false;

	if (canMoveInternal(fromPillar, pillar)) {
		pillars[pillar].push_back(selectedDisk);
		moveCount++;
		success = true;
	}
	else {
		pillars[fromPillar].push_back(selectedDisk);
	}

	holding = false;
	selectedDisk = -1;
	fromPillar = -1;

	return success;
}

bool GameLogic::canMoveInternal(int from, int to) const {
	if (from == to) {
		return 0;
	}

	else if (from < 0 or from >= pillars.size()) {
		return 0;
	}

	else if (to < 0 or to >= pillars.size()) {
		return 0;
	}
	
	int startVal = selectedDisk;
	int endVal;
	if (pillars[to].empty()) {
		endVal = 0;
	}
	else {
		endVal = pillars[to].back();
	}

	if (endVal == 0 or startVal < endVal) {
		return 1;
	}
	else {
		return 0;
	}

	//int topEnd = pillars[from].back();
	//return topEnd > pillars[to].back(); 
}

const vector<vector<int>>& GameLogic::getPillars() const {
	return pillars;
}

bool GameLogic::hasSelectedDisk() const {
	return holding;
}

int GameLogic::getSelectedDisk() const {
	return selectedDisk;
}

int GameLogic::getSelectedFromPillar() const {
	return fromPillar;
}

bool GameLogic::isSolved() const {
	if (pillars[1].size() == diskCount) {
		return 1;
	}

	if (pillars[2].size() == diskCount) {
		return 1;
	}

	return 0;
}

int GameLogic::getMoveCount() const {
	return moveCount;
}

