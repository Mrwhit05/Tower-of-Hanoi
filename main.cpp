#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool canMove(vector<int> start, vector<int> end) {
	if (start[-1] < end[-1]) {
		return 0;
	}
	else {
		return 1;
	}
}

vector<int> moveBlock(vector<int> start, vector<int> end) {

	return start, end;
}

void printPillars(vector<int> pillarA, vector<int> pillarB, vector<int> pillarC) {
	cout << "Pillar A: ";
	for (int i : pillarA) {
		cout << i << " ";
	}
	cout << "Pillar B: ";
	for (int i : pillarB) {
		cout << i << " ";
	}
	cout << "Pillar C: ";
	for (int i : pillarC) {
		cout << i << " ";
	}
	return 0;
}

int main() {
	vector<int> pillarA(5) = { 5, 4, 3, 2, 1 };
	vector<int> pillarB(5) = {};
	vector<int> pillarC(5) = {};

	cout << "Tower of Hanoi Simulator" << endl;

	printPillars(pillarA, pillarB, pillarC);
	
	cout << "Choose a pillar(A, B, or C): ";
	string choiceStart;
	cin >> choiceStart;
	
	cout << "Choose a pillar(A, B, or C): ";
	string choiceEnd;
	cin >> choiceEnd;

	if (canMove(choiceStart, choiceEnd)
}