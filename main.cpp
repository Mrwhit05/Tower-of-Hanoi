#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool canMove(const vector<vector<int>>& pillars, int from, int to) {
	int startVal;
	if (from == to) {
		return 0;
	}
	else if ((from > 2 or from < 0) or (to > 2 or to < 0)) {
		return 0;
	}
	else if (pillars[from].empty()) {
		return 0;
	}
	else {
		startVal = pillars[from].back();
	}

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
}

void printPillars(const vector<int>& pillarA, const vector<int>& pillarB, const vector<int>& pillarC) {
	cout << "Pillar A: " << endl;
	for (int i : pillarA) {
		cout << i << " ";
	}
	cout << endl;
	cout << "Pillar B: " << endl;
	for (int i : pillarB) {
		cout << i << " ";
	}
	cout << endl;
	cout << "Pillar C: " << endl;
	for (int i : pillarC) {
		cout << i << " ";
	}
	cout << endl;
}

bool checkSolved(const vector<int>& pillarB, const vector<int>& pillarC) {
	vector<int> solution = { 5, 4, 3, 2, 1 };
	if (pillarB == solution) {
		return 1;
	}
	else if (pillarC == solution) {
		return 1;
	}
	else {
		return 0;
	}
}

int main() {
	int count = 0;
	vector<vector<int>> pillars(3);
	pillars[0] = {5, 4, 3, 2, 1};
	pillars[1] = {};
	pillars[2] = {};

	cout << "Tower of Hanoi Simulator" << endl;

	while (!checkSolved(pillars[1], pillars[2])) {
		printPillars(pillars[0], pillars[1], pillars[2]);

		cout << "Choose a pillar(0, 1, or 2): ";
		int choiceStart;
		std::cin >> choiceStart;
		
		if (choiceStart < 0 or choiceStart >= pillars.size()) {
			cout << "Not an option" << endl;
			continue;
		}

		cout << "Choose a pillar(0, 1, or 2): ";
		int choiceEnd;
		std::cin >> choiceEnd;
		
		if (choiceEnd < 0 or choiceEnd >= pillars.size()) {
			cout << "Not an option" << endl;
			continue;
		}
		
		if (canMove(pillars, choiceStart, choiceEnd)) {
			vector<int>& start = pillars[choiceStart];
			vector<int>& end = pillars[choiceEnd];

			int top = start[start.size() - 1];
			end.push_back(top);
			start.pop_back();
			count++;
		}
		else {
			cout << "Invalid move" << endl;
			continue;
		}
	}
	cout << "Congratulations! You finished in " << count << " moves" << endl;
}
