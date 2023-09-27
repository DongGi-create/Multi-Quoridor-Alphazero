import numpy as np
import random as rd
import math
import copy
from QuoridorAPI import State
import time


PARENT_NODE_COUNT = 3
SIMULATION_COUNT = 100


def random_action(state):
    legal_actions = state.legal_actions()
    return legal_actions[rd.randint(0, len(legal_actions) - 1)]

def playout(state):

    if state.is_done():

        if state.is_draw():
            return [0.25 for _ in range(4)]
        
        rlst = [0] * 4
        rlst[state.winner()] += 1
        return rlst
    
    return playout(state.next(random_action(state)))

def argmax(collection, key=None):
    return collection.index(max(collection))

def mcts_action(state):
    class Node:
        def __init__(self, state):
            self.state = state
            self.n = 0
            self.scores = [0,0,0,0]
            self.child_nodes = None

        def get_w(self):
            return self.scores[self.state.get_player()]

        def next_child_node(self):
            for child_node in self.child_nodes:
                if child_node.n == 0:
                    return child_node

            t = 0
            for c in self.child_nodes:
                t += c.n
            ucb1_values = []
            for child_node in self.child_nodes:
                ucb1_values.append(child_node.get_w() / child_node.n + (2 * math.log(t) / child_node.n) ** 0.5)

            return self.child_nodes[argmax(ucb1_values)]

        def expand(self):
            self.child_nodes = [ Node(copy.deepcopy(self.state).next(action)) for action in self.state.legal_actions() ]
        
        def eval(self):
            if self.state.is_done():
                if self.state.is_draw():
                    self.scores = [0.25 for _ in range(4)]
                else:
                    winner = self.state.winner()
                    self.scores[winner] += 1
                return self.scores

            if not self.child_nodes:
                s = copy.deepcopy(self.state)
                values = playout(s)
                self.scores = [self.scores[i] + values[i] for i in range(4)]

                self.n += 1

                if self.n == 10:
                    self.expand()
                return self.scores
            else:
                child_scores = self.next_child_node().eval()

                player = self.state.get_player()

                if self.scores[player] < child_scores[player]:
                    self.scores = child_scores
                self.n += 1

                return child_scores

            
    root_node = Node(state)
    root_node.expand()

    for _ in range(SIMULATION_COUNT):
        root_node.eval()

    legal_actions = state.legal_actions()
    n_list = []
    for c in root_node.child_nodes:
        n_list.append(c.n)
    return legal_actions[argmax(n_list)]


EP_GANE_COUNT = 10

def play(next_actions):
    state = State()

    while True:

        if state.is_done():
            break

        # print(state)
        start = time.time()
        next_action = next_actions[state.get_player()]
        action = next_action(state)
        end = time.time()
        print('cost : {}'.format(end-start))


        state = state.next(action)

    if state.is_draw():
        return [0.25 for _ in range(4)]
    
    rlst = [0] * 4
    rlst[state.winner()] += 1
    return rlst

def change_turn(collection):
    return collection[1:] , collection[0]

def evaluate_algorithm_of(label, next_actions):

    total_point = 0

    for i in range(EP_GANE_COUNT):
        total_point += play(next_actions)[i % 4]
        change_turn(next_actions)

        print('\rEvaluate {}/{}'.format(i+1, EP_GANE_COUNT), end='')
    print('')

    average_point = total_point / EP_GANE_COUNT
    print(label.format(average_point))

if __name__ == '__main__':
    next_actions = (mcts_action, random_action, random_action, random_action)
    evaluate_algorithm_of('VS_RANDOM {:3f}', next_actions)