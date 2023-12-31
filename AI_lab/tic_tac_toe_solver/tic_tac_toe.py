class xo:
    def __init__(self):
        self.grid=((' ',' ',' '),(' ',' ',' '),(' ',' ',' '))
        self.player_turn='X'
        self.utility=0
        self.num_leaves_explored=0
        self.num_turns_played=0
        self.max_depth=0

    def moves(self,nod):
        return [i for i in range(1,10) if nod[(i-1)//3][(i-1)%3]==' ']

    def result(self,nod,action):
        if nod.player_turn=='X':
            temp=[]
            for row in nod.state:
                temp.append(list(row))
            temp[(action-1)//3][(action-1)%3]='X'
            temp1=[]
            for row in temp:
                temp1.append(tuple(row))
            return tuple(temp1)
        
        temp=[]
        for row in nod.state:
            temp.append(list(row))
        temp[(action-1)//3][(action-1)%3]='O'
        temp1=[]
        for row in temp:
            temp1.append(tuple(row))
        return tuple(temp1)
                
    def x_play(self,index): 
        temp=[]
        for row in self.grid:
            temp.append(list(row))
        if(temp[(index-1)//3][(index-1)%3]==' '):
            temp[(index-1)//3][(index-1)%3]='X'
        else:
            self.x_play(int(input("wrong position! please enter another position: ")))
            return
        temp1=[]
        for row in temp:
            temp1.append(tuple(row))
        self.grid=tuple(temp1)
        self.player_turn='O'
        self.num_turns_played+=1

    def terminal_state(self,nod):
        if nod.player_turn=='X':
            for i in range(3):
                for j in range(3):
                    if not nod.state[i][j]=='O':
                        break
                else:
                    nod.utility=1
                    return 1
            
            for i in range(3):
                for j in range(3):
                    if not nod.state[j][i]=='O':
                        break
                else:
                    nod.utility=1
                    return 1
                
            for i in range(3):
                if not nod.state[i][i]=='O':
                    break
            else:
                nod.utility=1
                return 1
                                                                     
            for i in range(3):
                if not nod.state[i][2-i]=='O':
                    break
            else:
                nod.utility=1
                return 1
            
            return 0
                                                                     
        for i in range(3):
            for j in range(3):
                if not nod.state[i][j]=='X':
                    break
            else:
                nod.utility=-1
                return 2
        
        for i in range(3):
            for j in range(3):
                if not nod.state[j][i]=='X':
                    break
            else:
                nod.utility=-1
                return 2
            
        for i in range(3):
            if not nod.state[i][i]=='X':
                break
        else:
            nod.utility=-1
            return 2
                                                                    
        for i in range(3):
            if not nod.state[i][2-i]=='X':
                break
        else:
            nod.utility=-1
            return 2
        
        if nod.num_turns_played==9:
            return 3
        return 0
        
    def display(self):
        for i in range(2):
            for j in range(2):
                print(f"{self.grid[i][j]}|",end='')
            print(self.grid[i][2])
            print("-+-+-")
        print(f'{self.grid[2][0]}|{self.grid[2][1]}|{self.grid[2][2]}\n')
                                                                     
    def o_play(self):
        index=alpha_beta_minimax(self,node(self.grid,self.num_turns_played,self.player_turn))
        temp=[]
        for row in self.grid:
            temp.append(list(row))
        if(temp[(index-1)//3][(index-1)%3]==' '):
            temp[(index-1)//3][(index-1)%3]='O'
        else:
            self.x_play(int(input("wrong position! please enter another position: ")))
        temp1=[]
        for row in temp:
            temp1.append(tuple(row))
        self.grid=tuple(temp1)
        self.player_turn='X'
        self.num_turns_played+=1

    def play_game(self):
        terminal_state=self.terminal_state(node(self.grid,self.num_turns_played,self.player_turn))
        while not terminal_state:
            self.display()
            if self.player_turn=='X':
                self.x_play(int(input("enter a position for X player: ")))
            else:
                self.o_play()
            terminal_state=self.terminal_state(node(self.grid,self.num_turns_played,self.player_turn))
        self.display()
        if terminal_state==1:
            print("O player wins")
            
        elif terminal_state==2:
            print("X player wins")
            
        else:
            print("Game ended in a Draw")
        
class node:
    def __init__(self,state,num_turns,to_play,action=None,utility=0):
        self.state=state
        self.utility=utility
        self.num_turns_played=num_turns    
        self.player_turn=to_play
        self.action=action

    def child_gen(self,game:xo):
        ret=[]
        for move in game.moves(self.state):
            if self.player_turn=='O':
                z='X'
            else:
                z='O'
            nod=node(game.result(self,move),self.num_turns_played+1,z,move)
            ret.append(nod)
        return ret
    
def minutil(game:xo,nod:node,depth,alpha,beta):
    if nod.num_turns_played>depth[0]:
        depth[0]=nod.num_turns_played
    if game.terminal_state(nod):
        game.num_leaves_explored+=1
        return
    min_utility=2
    children=nod.child_gen(game)
    for child in children:
        maxutil(game,child,depth,alpha,beta)
        if child.utility<min_utility:
            min_utility=child.utility
            if min_utility<beta:
                 beta=min_utility
        if child.utility<alpha:
            break 
    nod.utility=min_utility
    
def maxutil(game:xo,nod:node,depth,alpha,beta):
    if nod.num_turns_played>depth[0]:
        depth[0]=nod.num_turns_played
    if game.terminal_state(nod):
        game.num_leaves_explored+=1
        return
    max_utility=-2
    children=nod.child_gen(game)
    for child in children:
        minutil(game,child,depth,alpha,beta)
        if child.utility>max_utility:
            max_utility=child.utility
            if max_utility>alpha:
                alpha=max_utility
        if child.utility>beta:
            break
    nod.utility=max_utility

def alpha_beta_minimax(game:xo,nod:node):
    alpha=-1
    beta=1
    depth=[0]
    max_utility=-2
    max_node=None
    children=nod.child_gen(game)
    for child in children:
        minutil(game,child,depth,alpha,beta)
        if child.utility>max_utility:
            max_node=child
            max_utility=child.utility
            if max_utility>alpha:
                alpha=max_utility
    if depth[0]-nod.num_turns_played>game.max_depth:
        game.max_depth=depth[0]-nod.num_turns_played
    return max_node.action
    
print('''Board locations are as follows
1|2|3
-+-+-
4|5|6
-+-+-
7|8|9
''')
play_xo=xo()
play_xo.play_game()
print(f"no. of leaves explored: {play_xo.num_leaves_explored} and max depth: {play_xo.max_depth}")
