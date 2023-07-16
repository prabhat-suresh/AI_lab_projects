#include <iostream>
#include <queue>
#include <set>
using namespace std;

typedef struct node { // structure to represent the states
  int priority;
  int cost;
  vector<vector<int>> state;
  string action;
} node;

int manhattan(node *nod, int k) { // manhattan heuristic
  int count = 0, x;
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j++) {
      if (x = nod->state[i][j]) {
        count += abs((x - 1) / k - i) + abs((x - 1) % k - j);
      }
    }
  }
  return count;
}

bool solvable(node *nod,int k) { // checking whether the puzzle is solvable or invalid
  int inversions = 0;
  bool blankrow;
  for (int i = 0; i < k * k; i++) {
    if(nod->state[i/k][i%k]==0) blankrow=(i/k)%2==0;
    for (int j = i + 1; j < k * k; j++) {
      if (nod->state[i / k][i % k] > nod->state[j / k][j % k] &&
          nod->state[i / k][i % k] && nod->state[j / k][j % k])
        inversions++;
    }
  }
  if(k%2==0 && blankrow) return inversions%2==1;
  return inversions % 2 == 0;
}

bool goal_test(node *nod, int k) { // test for goal state (final configuration)
  for (int i = 0; i < k * k - 1; i++) {
    if (nod->state[i / k][i % k] != i + 1)
      return false;
  }
  return true;
}

struct comp {
  bool operator()(node *i,node *j) { // a custom comparison fn for the priority queue
    return i->priority >= j->priority;
  }
};

void check_child_state(node *x, char move, int i, int k,priority_queue<node *, vector<node *>, comp> &pq,
set<string> &explored) {

  node *y = new node;
  y->cost = x->cost + 1;
  y->state = x->state;
  y->action=x->action;

  if (move == 'l') {
    y->action.push_back('l');
    int temp = y->state[i/k][i%k];
    y->state[i/k][i%k] = y->state[(i - 1)/k][(i-1)%k];
    y->state[(i - 1)/k][(i-1)%k] = temp;
  } 
  else if (move == 'r') {
    y->action.push_back('r');
    int temp = y->state[i/k][i%k];
    y->state[i/k][i%k] = y->state[(i + 1)/k][(i+1)%k];
    y->state[(i + 1)/k][(i+1)%k] = temp;
  } 
  else if (move == 'd') {
    y->action.push_back('d');
    int temp = y->state[i/k][i%k];
    y->state[i/k][i%k] = y->state[(i + 3)/k][(i+3)%k];
    y->state[(i + 3)/k][(i+3)%k] = temp;
  } 
  else {
    y->action.push_back('u');
    int temp = y->state[i/k][i%k];
    y->state[i/k][i%k] = y->state[(i-3)/k][(i-3)%k];
    y->state[(i-3)/k][(i-3)%k] = temp;
  }

  y->priority = manhattan(y, k) + y->cost;

  string representation;
  for(int i=0;i<k;i++){
    for(int j=0;j<k;j++){
      string temp=to_string(y->state[i][j]);
      for(char c:temp) representation.push_back(c);
    }
  }

  if (explored.count(representation) == 0) {
    if (y == NULL) {
      cout<<"Memory overflow\n";
      return;
    }
    pq.push(y);
    explored.insert(representation);
  }
}

node *a_star(node *nod, int k) { // A* search algorithm
  unsigned int explored_num = 0;
  int i;
  node *x;
  priority_queue<node *, vector<node *>, comp> pq;
  set<string> explored;
  pq.push(nod);

  while (true) {
    x = pq.top();
    pq.pop();
    explored_num++;
    // printf("No. of nodes explored: %u\n", explored_num);
    if (goal_test(x, k)) {
      cout << "No. of nodes explored: " << explored_num << '\n';
      cout << "No. of moves taken: " << x->cost << '\n';
      return x;
    }
    for (i = 0; x->state[i / k][i % k]; i++);

    if (i % k) {
      check_child_state(x, 'l', i, k, pq, explored);
    }
    if ((i + 1) % k) {
      check_child_state(x, 'r', i, k, pq, explored);
    }
    if (i > k-1) {
      check_child_state(x, 'u', i, k, pq, explored);
    }
    if (i < k*k-k) {
      check_child_state(x, 'd', i, k, pq, explored);
    }
    delete x;
  }
}

/* void print(node *solution,int k,int index) { // fn to print the puzzle after each move of th solution
  if (index>=0) {
    int dj=0,di=0;
    if(solution->action[index]=='l') dj=1;
    else if(solution->action[index]=='r') dj=-1;
    else if(solution->action[index]=='u') di=-1;
    else di=1;
    for(int i=0;i<k;i++){
      for(int j=0;j<k;j++){
        if(solution->state[i][j]==0){
          solution->state[i][j]=solution->state[i+di][j+dj];
          solution->state[i+di][j+dj]=0;
          i=j=k;
        }
      }
    }
    print(solution, k, index-1);
    for(int i=0;i<k;i++){
      for(int j=0;j<k;j++){
        if(solution->state[i][j]==0){
          solution->state[i][j]=solution->state[i-di][j-dj];
          solution->state[i-di][j-dj]=0;
          i=j=k;
        }
      }
    }
    cout << solution->action[index] << '\n';
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        cout << solution->state[i][j] << ' ';
      }
      cout << '\n';
    }
    cout<<'\n';
  }
} */

int main() {
  int k;
  cout << "Enter the value of k of the n-puzzle such that n=k^2-1 for some positive integer k: ";
  cin >> k;

  node *start = new node;
  start->action = ""; // Null action
  start->cost = 0;

  cout << "enter starting position in row-major form (row 1 values followed by row 2 values and so on) as an (n+1)-length array of space-separated digits with '0' indicating the blank tile: ";
  for (int i = 0; i < k; i++) {
    start->state.push_back({});
    for (int j = 0; j < k; j++) {
      int inp;
      cin >> inp;
      start->state[i].push_back(inp);
    }
  }

  start->priority = manhattan(start, k);

  if (solvable(start, k)){
    node *solution=a_star(start, k);
    cout<<solution->action<<'\n';
    // print(solution, k, solution->action.size()-1);
  }
  else cout << "Invalid puzzle input. This puzzle is not solvable.";
}
