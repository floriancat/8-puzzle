#include <iostream>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <queue>
using namespace std;

void prompt();
void search();
void printPuzzle(vector<vector<int>> puzzle); // Function to print a puzzle
vector<vector<vector<int>>> expansion(vector<vector<int>> current); // Function to create puzzle children
vector<vector<vector<int>>> visited; // Vector of puzzles to keep track of puzzle states that were already visited
vector<vector<vector<int>>> frontier; // Vector of puzzles to hold expansion return
int userChoice;
vector<vector<int>> puzzle(3, vector<int>(3)); // Initializing vector of  3 vectors each containing 3 ints to represent the 8 puzzle: https://stackoverflow.com/questions/13121469/initializing-a-vector-of-vectors-having-a-fixed-size-with-boost-assign
vector<vector<int>> solution(3, vector<int>(3)); // Puzzle state that is the solution, used to compare current puzzle state to solution state
int depth = 0;
bool checkDuplicate(vector<vector<int>> state); // Function to check if a state is already one that we have visited

int main() {
  // Requesting user input
  prompt();
  // Filling solution matrix with values in correct places
  int count = 1;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      solution[i][j] = count;
      count++;
    }
  }
  solution[2][2] = 0;
  //Printing solution to reach
  //printPuzzle(solution);

  //search();
}

// Prompts user for either default or custom puzzle and algorithm choice
void prompt() {
  //int userChoice = 0;

  cout << "Welcome to Florian Catalan's 8-puzzle solver.\n";
  cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle: ";

  cin >> userChoice;

  if(userChoice == 2){
    cout << "\nEnter your puzzle, use a zero to represent the blank\n";
    cout << "Enter the first row, use space or tabs between numbers: ";
    for(int i = 0; i < 3; i++) {
        cin >> puzzle[0][i];
    }
    cout << "Enter the second row, use space or tabs between numbers: ";
    for(int i = 0; i < 3; i++) {
        cin >> puzzle[1][i];
    }
    cout << "Enter the third row, use space or tabs between numbers: ";
    for(int i = 0; i < 3; i++) {
        cin >> puzzle[2][i];
    }
  } else if(userChoice == 1) {
    int count = 1;
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
        puzzle[i][j] = count;
        count++;
      }
    }
    puzzle[2][2] = 0;
  }

  cout << "\n Enter your choice of algorithm:\n1. Uniform Cost Search\n2. A* with the Misplaced Tile Heuristic\n3. A* with the Manhattan Distance Heuristic\n";
  cin >> userChoice;
  if(userChoice == 1)
    cout << "\nuniformCost()" << endl;
  else if(userChoice == 2)
    cout << "\nmisplacedTile()" << endl;
  else if(userChoice == 3)
    cout << "\nmanhattanDist()" << endl;

  printPuzzle(puzzle);
  
  //Print vector of puzzles
  vector<vector<vector<int>>> children = expansion(puzzle);
  for(int i = 0; i < children.size(); i++) {
    for(int j = 0; j < 3; j++) {
      for(int l = 0; l < 3; l++) {
        cout << children.at(i)[j][l] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
}

vector<vector<vector<int>>> expansion(vector<vector<int>> current) {
  // Locating the zero tile
  int i0, j0;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(current[i][j] == 0) {
        i0 = i;
        j0 = j;
        break;
      }
    }
  }
  //Expanding the current puzzle state w/ possible moves
  vector<vector<vector<int>>> children;
  vector<vector<int>> temp = current;
  int upi = i0 - 1;
  int downi = i0 + 1;
  int leftj = j0 - 1;
  int rightj = j0 + 1;
  if(upi >= 0) {
    swap(temp[i0][j0], temp[upi][j0]);
    children.push_back(temp);
    temp = current;
  }
  if(downi <= 2) {
    swap(temp[i0][j0], temp[downi][j0]);
    children.push_back(temp);
    temp = current;
  }
  if(leftj >= 0) {
    swap(temp[i0][j0], temp[i0][leftj]);
    children.push_back(temp);
    temp = current;
  }
  if(rightj  <= 2) {
    swap(temp[i0][j0], temp[i0][rightj]);
    children.push_back(temp);
    temp = current;
  }
  return children;
}

void printPuzzle(vector<vector<int>> puzzle){
  cout << endl; //cout << "Your puzzle: " << endl;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      cout << puzzle[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

void search(){
  //int priority = 1;
  queue<pair<int, vector<vector<int>>>> q;
  q.push(make_pair(depth, puzzle));
  //int qCount = 1;
  while(1) {
    //priority++;
    if(q.empty()) {
      cout << endl << "Failure." << endl;
      return;
    }
    vector<vector<int>> node = q.front().second;
    q.pop();
    // qCount--;
    // if(qCount == 0) {
    //   depth++;
    // }
    if(node == solution) {
      cout << endl << "Solution found at depth: " << depth << '!' << endl;
      return;
    }
    //cout << endl << "calling expansion to fill frontier" << endl;
    // vector<vector<vector<int>>> expanse = expansion(node);
    // for(int i = 0; i < expanse.size(); i++) {
    //   frontier.push_back(expanse.at(i));
    // }
    frontier = expansion(node);
    //cout << endl << "filled frontier" << endl;
    for(int i = 0; i < frontier.size(); i++) {
      if(!checkDuplicate(frontier.at(i))) {
        //cout << endl << "pushing state from frontier to pq" << endl;
        q.push(make_pair(depth, frontier.at(i)));
        //priority++;
        //cout << endl << "pushed state from frontier to pq" << endl;
      }
    }
    //qCount = q.size();
    //cout << endl << "pushing current node to visited" << endl;
    visited.push_back(node);
  }
}

bool checkDuplicate(vector<vector<int>> state){
  for(int i = 0; i < visited.size(); i++) {
    if(state == visited.at(i)) {
      return true;
    }
  }
  return false;
}