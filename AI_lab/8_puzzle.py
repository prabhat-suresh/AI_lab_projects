from queue import PriorityQueue

class puzzle_8:
    def __init__(self,initial,goal=(1,2,3,4,5,6,7,8,0)):
        self.initial=initial
        self.goal=goal
        
    def actions(self,state):
        possible_actions=[]
        blank=state.index(0)
        if blank>2:
            possible_actions.append("up")
        if blank<6:
            possible_actions.append("down")
        if (blank+1)%3:
            possible_actions.append("right")
        if blank%3:
            possible_actions.append("left")
        return possible_actions
        
    def result(self,state,action):
        l=list(state)
        results={'up':-3,'down':3,'right':1,'left':-1}
        blank=state.index(0)
        l[blank],l[blank+results[action]]=l[blank+results[action]],l[blank]
        return tuple(l)
    
    def goal_test(self,state):
        return state==self.goal
    
    def check_solvability(self):
        """ Checks if the given state is solvable """
        state=self.initial
        inversion = 0
        for i in range(len(state)):
            for j in range(i + 1, len(state)):
                if (state[i] > state[j]) and state[i] != 0 and state[j] != 0:
                    inversion += 1

        return inversion % 2 == 0
    
    def num_misplaced_tiles(self,state):
        num_misplaced=0
        for i in range(len(state)):
            if not state[i]==i+1:
                num_misplaced+=1
        return num_misplaced
    
    def manhattan(self,state):
        manhattan_dist=0
        for i in range(1,len(state)):
            index=state.index(i)
            manhattan_dist+=abs((i-1)%3-index%3)+abs((i-1)//3 - index//3)
        return manhattan_dist

class Node:
    def __init__(self,state,parent=None,action=None):
        self.state=state
        self.parent=parent
        self.action=action
        if parent:
            self.depth=parent.depth+1
            self.path_cost=parent.path_cost+1
        else:
            self.depth=0
            self.path_cost=0

    def __eq__(self, other):
        return isinstance(other, Node) and self.state == other.state       
 
    def expand(self,problem):
        return [self.child_node(problem,action) for action in problem.actions(self.state)]
       
    def __lt__(self, node):
        return self.state < node.state
 
    def child_node(self,problem,action):
        next_state=problem.result(self.state,action)
        child=Node(next_state,self,action)
        return child
    
    def path(self):
        l=[]
        node=self
        while node:
            l.append(node)
            node=node.parent
        l.reverse()
        return list(l)
        # return list(l.reverse())
    
    def solution(self):
        return self.path()[1:]
    
def A_star(puzzle):
    explored_list=[]
    num_steps=0
    frontier=PriorityQueue()
    frontier.put((puzzle.manhattan(puzzle.initial),Node(puzzle.initial)))
    while frontier:
        node=frontier.get()[1]
        num_steps+=1
        if puzzle.goal_test(node.state):
            return (node,num_steps)
        for child in node.expand(puzzle):
            if not child in explored_list and not child in frontier.queue:
                frontier.put((puzzle.manhattan(child.state)+child.path_cost,child))
        explored_list.append(node)
    return None

start=tuple(map(int,input("enter initial configuration").split()))
puzzle8=puzzle_8(start) 
if(puzzle8.check_solvability()):
    sol=A_star(puzzle8)
    route=sol[0].solution()
    for node in route:
        print(node.action)
        print(node.state[:3])
        print(node.state[3:6])
        print(node.state[6:9])

    print("number of steps taken:",len(route))
    print("explored: ",sol[1])
else:
    print("not solvable")