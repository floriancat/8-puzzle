#include <iostream>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <queue>
#include <cmath>
using namespace std;

int userChoice; // Know what algorithm to use
vector<vector<int>> puzzle(3, vector<int>(3)); // The user's 8 puzzle
vector<vector<int>> solution(3, vector<int>(3)); // The solved puzzle
vector<vector<vector<int>>> visitedPuzzles; // Vector of puzzles that have been seen before 
void prompt(); // Initial prompt
void printPuzzle(vector<vector<int>> puzzle); // Print a puzzle
bool checkDuplicate(vector<vector<int>> puzzle); // Checks if puzzle has been seen before
vector<vector<vector<int>>> createChildren(vector<vector<int>> puzzle); // Create the puzzle's children
void searchUFC(); // Perform uniform cost search (breadth first search) on puzzle
vector<vector<int>> nullPuzzle(3, vector<int>(3, 0));
void searchHeuristic(); // Perform search with a heuristic on puzzle
int misplacedTile(vector<vector<int>> puzzle); // Heuristic to compute number of misplaced tiles
int manhattanD(vector<vector<int>> puzzle); // Heuristic to compute sum of distances tiles are from their solution space
typedef pair<int, vector<vector<int>>> pi; // Min heap

int main() {
  prompt();
  int count = 1;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      solution[i][j] = count;
      count++;
    }
  }
  solution[2][2] = 0;
  //printPuzzle(nullPuzzle);
  if(userChoice == 1){
    searchUFC();
  } else if(userChoice == 2 || userChoice == 3){
    searchHeuristic();
  }
}

void searchHeuristic() {
  int depth = 0;
  int maxQueueSize = 0;
  int nodesExpanded = 0;
  priority_queue<pi, vector<pi>, greater<pi>> pq;
  //priority_queue<pair<int, vector<vector<int>>>> pq;
  pq.push(make_pair(1, puzzle));
  while(!pq.empty()) {
    // priority_queue<pi, vector<pi>, greater<pi>> print = pq;
    // //priority_queue<pair<int, vector<vector<int>>>> print = pq;
    // while(!print.empty()) {
    //   cout << endl << print.top().first << endl;
    //   print.pop();
    // }
    cout << endl << pq.top().first << endl;

    if(pq.size() > maxQueueSize) {
      maxQueueSize = pq.size();
    }
    vector<vector<int>> curr = pq.top().second;
    pq.pop();
    if(curr == solution) {
      cout << endl << "Solution found at depth: " << depth << endl;
      cout << "Max queue size: " << maxQueueSize << endl;
      cout << "Number of nodes expanded: " << nodesExpanded << endl;
      return;
    }
    vector<vector<vector<int>>> toEnqueue = createChildren(curr);
    nodesExpanded++;
    for(int i = 0; i < toEnqueue.size(); i++) {
      if(!checkDuplicate(toEnqueue.at(i))) {
        if(userChoice == 2) {
          pq.push(make_pair(misplacedTile(toEnqueue.at(i)), toEnqueue.at(i)));
        } else if(userChoice == 3) {
          pq.push(make_pair(manhattanD(toEnqueue.at(i)), toEnqueue.at(i)));
        }
      }
    }
    depth++;
    visitedPuzzles.push_back(curr);
  }
  cout << endl << "Failed" << endl;
  return;
}

int misplacedTile(vector<vector<int>> puzzle){
  int numMisplaced = 0;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(puzzle[i][j] == 0) {

      } else if(puzzle[i][j] != solution[i][j]) {
        numMisplaced++;
      }
    }
  }
  return numMisplaced;
}

int manhattanD(vector<vector<int>> puzzle){
  int distance = 0;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(puzzle[i][j] != 0) {
        for(int n = 0; n < 3; n++) {
          for(int m = 0; m < 3; m++) {
            if(solution[n][m] == puzzle[i][j]){
              distance += abs(n - i) + abs(m - j);
            }
          }
        }
      }
    }
  }
  return distance;
}

void searchUFC(){
  int depth = 0;
  int maxQueueSize = 0;
  int nodesExpanded = 0;
  queue<vector<vector<int>>> q;
  q.push(puzzle);
  while(!q.empty()) {
    if(q.size() > maxQueueSize) {
      maxQueueSize = q.size();
    }
    vector<vector<int>> curr = q.front();
    q.pop();
    if(curr == nullPuzzle){
      depth++;
      curr = q.front();
      q.pop();
      q.push(nullPuzzle);
    }
    if(curr == solution) {
      cout << endl << "Solution found at depth: " << depth << endl;
      cout << "Max queue size: " << maxQueueSize << endl;
      cout << "Number of nodes expanded: " << nodesExpanded << endl;
      return;
    }
    vector<vector<vector<int>>> toEnqueue = createChildren(curr);
    nodesExpanded++;
    for(int i = 0; i < toEnqueue.size(); i++) {
      if(!checkDuplicate(toEnqueue.at(i))) {
        q.push(toEnqueue.at(i));
      }
    }
    if(depth == 0) {
      depth++;
      q.push(nullPuzzle);
    }
    visitedPuzzles.push_back(curr);
  }
  cout << endl << "Failed" << endl;
  return;
}

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
  // if(userChoice == 1)
  //   cout << "\nuniformCost()" << endl;
  // else if(userChoice == 2)
  //   cout << "\nmisplacedTile()" << endl;
  // else if(userChoice == 3)
  //   cout << "\nmanhattanDist()" << endl;

  //printPuzzle(puzzle);
  
  //Print vector of puzzles
  // vector<vector<vector<int>>> children = expansion(puzzle);
  // for(int i = 0; i < children.size(); i++) {
  //   for(int j = 0; j < 3; j++) {
  //     for(int l = 0; l < 3; l++) {
  //       cout << children.at(i)[j][l] << " ";
  //     }
  //     cout << endl;
  //   }
  //   cout << endl;
  // }
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

bool checkDuplicate(vector<vector<int>> state){
  for(int i = 0; i < visitedPuzzles.size(); i++) {
    if(state == visitedPuzzles.at(i)) {
      return true;
    }
  }
  return false;
}

vector<vector<vector<int>>> createChildren(vector<vector<int>> puzzle) {
  // Locating the zero tile
  int i0, j0;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(puzzle[i][j] == 0) {
        i0 = i;
        j0 = j;
        break;
      }
    }
  }
  //Expanding the current puzzle state w/ possible moves
  vector<vector<vector<int>>> children;
  vector<vector<int>> temp = puzzle;
  int upi = i0 - 1;
  int downi = i0 + 1;
  int leftj = j0 - 1;
  int rightj = j0 + 1;
  if(upi >= 0) {
    swap(temp[i0][j0], temp[upi][j0]);
    children.push_back(temp);
    temp = puzzle;
  }
  if(downi <= 2) {
    swap(temp[i0][j0], temp[downi][j0]);
    children.push_back(temp);
    temp = puzzle;
  }
  if(leftj >= 0) {
    swap(temp[i0][j0], temp[i0][leftj]);
    children.push_back(temp);
    temp = puzzle;
  }
  if(rightj  <= 2) {
    swap(temp[i0][j0], temp[i0][rightj]);
    children.push_back(temp);
    temp = puzzle;
  }
  return children;
}

// Referred pages
//https://stackoverflow.com/questions/13121469/initializing-a-vector-of-vectors-having-a-fixed-size-with-boost-assign;
//https://stackoverflow.com/questions/31247634/how-to-keep-track-of-depth-in-breadth-first-search
//https://stackoverflow.com/questions/29781359/how-to-find-manhattan-distance-in-a-continuous-two-dimensional-matrix#:~:text=The%20Manhattan%20distance%20is%20simply,to%20(5%2C%201)%20.
//https://www.geeksforgeeks.org/priority-queue-in-cpp-stl/
//https://www.geeksforgeeks.org/priority-queue-of-pairs-in-c-ordered-by-first/