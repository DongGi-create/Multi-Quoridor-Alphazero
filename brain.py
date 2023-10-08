import copy
import time
import random
from collections import deque

import sys
sys.path.append("C:/Users/user/Desktop/Project/Quorido/multi-quoridor-alphazero")

from game import State
import game as api


class MyState(State):
    def __init__(self, state=None):
        if state == None:
            super().__init__()
        else:
            super().__init__(state.player, state.turn)

    def is_end(self):
        return self.is_done()
    
    def generate_states(self):
        actions = self.legal_actions()
        return [MyState(copy.deepcopy(self).next(action)) for action in actions]
    
    def get_left_wall(self, turn):
        count = len(self.player[turn][2]) + \
            len(self.player[turn][3])
        return 5 - count
    



def brain1(state: MyState):
    que = deque()
    check = [[0 for _ in range(9)] for _ in range(9)]
    turn = state.get_player()
    print(turn)
    r, c = state.player[turn][0], state.player[turn][1]
    check[r][c] = 1
    for i in range(12):
        nr, nc = r + api.dr[i], c + api.dc[i]
        if state.is_range(nr, nc):
            if check[nr][nc] == 0 and state.can_go(i, r, c, nr, nc):
                que.append((nr, nc, i))
                check[nr][nc] = 1
    direction = -1
    while len(que):
        qr, qc, dir = que.popleft()
        if turn == 0 and qr == 0:
            direction = dir
            break
        elif turn == 1 and qr == 8:
            direction = dir
            break
        elif turn == 2 and qc == 8:
            direction = dir
            break
        elif turn == 3 and qc == 0:
            direction = dir
            break
        for i in range(12):
            nr, nc = qr + api.dr[i], qc + api.dc[i]
            if state.is_range(nr, nc):
                if check[nr][nc] == 0 and state.can_go(i, qr, qc, nr, nc):
                    que.append((nr, nc, dir))
                    check[nr][nc] = 1
    
    if (direction > 7):
        direction = direction -8
    return MyState(copy.deepcopy(state).next(direction))

def brain2(state: MyState):
    if state.left_wall() > 0:
        random_element = random.choice(state.legal_walls())
        return MyState(copy.deepcopy(state).next(random_element))
    else :
        return brain1(state)

def min_distance(state: MyState, pivotr, pivotc, way, destination):
    que = deque()
    check = [[0 for _ in range(9)] for _ in range(9)]
    turn = state.get_player()
    que.append((pivotr, pivotc))
    check[pivotr][pivotc] = 1
    while len(que):
        r, c = que.popleft()
        if way == 1 and r == destination:
            return check[r][c] % 100
        elif way == 2 and c == destination:
            return check[r][c] % 100
        for i in range(12):
            nr, nc = r + api.dr[i], c + api.dc[i]
            if state.is_range(nr, nc):
                if check[nr][nc] == 0 and state.can_go(i, r, c, nr, nc):
                    que.append((nr, nc))
                    check[nr][nc] = check[r][c] + 1

def get_pivot(state: MyState):
    sum = 0
    for turn in range(4):
        if turn == 0:
            way, en = 1, 0
        elif turn == 1:
            way, en = 1, 8
        elif turn == 2:
            way, en = 2, 8
        elif turn == 3:
            way, en = 2, 0
        
        if state.get_player()== turn:
            sum = sum - min_distance(state, state.player[turn][0],state.player[turn][1], way, en)
        else:
            sum = sum + min_distance(state, state.player[turn][0],state.player[turn][1], way, en)
    return sum

def brain3_walling(state: MyState, type, startx, starty, mean):
        ret = (-1, -1, -1, mean)
        if not state.crossWall(type, startx, starty):
            if type == 1:
                state.wallcnt += 1
                state.garowall[startx][starty] = state.wallcnt
                state.garowall[startx][starty + 1] = state.wallcnt

                if not state.closed_bfs():
                    pivot = get_pivot(state)
                    if pivot > mean:
                        mean = pivot
                        ret = (type, startx, starty, mean)
                state.garowall[startx][starty] = 0
                state.garowall[startx][starty + 1] = 0
                state.wallcnt -= 1
                return ret

            elif type == 2:
                state.wallcnt += 1
                state.serowall[startx][starty] = state.wallcnt
                state.serowall[startx + 1][starty] = state.wallcnt

                if not state.closed_bfs():
                    pivot = get_pivot(state)
                    if pivot > mean:
                        mean = pivot
                        ret = (type, startx, starty, mean)
                state.serowall[startx][starty] = 0
                state.serowall[startx + 1][starty] = 0
                state.wallcnt -= 1
                return ret
        else:
            return (-1, -1, -1, mean) #cross wall
        
def brain3walling(state:MyState):
    mean = get_pivot(state)
    temp = mean
    for type in range(1,3):
        for r in range(8):
            for c in range(8):
                a, b, c, mean = brain3_walling(state, type, r, c, mean)
                if a == -1:
                    continue
                istype, isr, isc = a, b, c

    if mean > temp:
        return state.coordinate_to_number(isr, isc, istype)
    else:
        return -1

def brain3(state: MyState):
    if state.left_wall() > 0:
        action = brain3walling(state)
        if action == -1:
            return brain1(state)
        return MyState(copy.deepcopy(state).next(action))
    else :
        return brain1(state)




def bot_play(state: MyState, brainType):
    if brainType == 1:
        return brain1(state)   
    elif brainType == 2:
        return brain2(state)
    elif brainType == 3:
        return brain3(state)

def person_play(state: MyState):
    print(f"legal actions: {state.legal_actions()}")
    action = int(input("action: "))
    return MyState(state.next(action))

def play(state: MyState, is_person, brainType = 1, time_list=[]):
    state = copy.deepcopy(state)

    print(f"turn: {state.turn}")

    if is_person:
        next_state = person_play(state)
    else:
        start = time.time()
        next_state = bot_play(state, brainType)
        end = time.time()

        run_time = end - start
        time_list.append(run_time)

        print(f"{run_time:.5f} sec")

    print(str(next_state), end='')
    print("------------------------------------")
    
    return next_state

if __name__ == "__main__":
    now_state= MyState()
    brainType = 1

    print(str(now_state), end='')
    print("------------------------------------")

    time_list = []
    while(not now_state.is_end()):
        now_state = play(now_state, False, brainType, time_list)
        if now_state.is_end():
            break

        now_state = play(now_state, False, brainType + 1)
        if now_state.is_end():
            break

        now_state = play(now_state, False, brainType + 2)
        if now_state.is_end():
            break

        now_state = play(now_state, True)
        
    print(f"\n{now_state.winner()} win!\n")
    
    sum = 0
    count = 0
    for t in time_list:
        print(f"{t:.5f}", end=' ')
        sum += t
        count += 1
    print(f"\nsum: {sum:.5f}, count: {count}")
    print(f"avg: {sum/count:.5f}")