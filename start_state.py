"""
Start states as bit arrays
"""
import numpy as np
def blank():
    return np.zeros(64) > 0
def squares(c):
    board = blank()
    
    for i in range(0,8):
        c = (c+1)%2
        for j in range(c,8,2):
            board[i*8 + j] = True
    return board
def white_squares():
    return squares(0)
def black_squares():
    return squares(1)
#STARTING POSITIONS
def white_pawns():
    board = blank()
    board[8:16] = True
    return board
def black_pawns():
    board = blank()
    board[8*6:8*6+8] = True
    return board
def bishops(i):
    board = blank()
    board[i*8+2]=True
    board[i*8+5]=True
    return board
def white_bishops():
    return bishops(0)
def black_bishops():
    return bishops(7)
def horses(i):
    board = blank()
    board[i*8+1]=True
    board[i*8+6]=True
    return board
def white_horses():
    return horses(0)
def black_horses():
    return horses(7)
def castles(i): 
    board = blank()
    board[i*8]=True
    board[i*8+7]=True
    return board
def white_castles():
    return castles(0)
def black_castles():
    return castles(7)
def white_king():
    board = blank()
    board[5] = True
    return board
def white_queen():
    board = blank()
    board[4] = True
    return board
def black_king():
    board = blank()
    board[8*7 + 5] = True
    return board
def white_queen():
    board = blank()
    board[8*7 + 4] = True
    return board

def original_game_state_arr(func=lambda x : x,word_length=0):
    data = np.zeros([16,word_length]) > 0
    data[0] = func(white_pawns())
    data[1] = func(black_pawns())
    data[2] = func(white_horses())
    data[3] = func(black_horses())
    data[4] = func(white_bishops())
    data[5] = func(black_bishops())
    data[6] = func(white_castles())
    data[7] = func(black_castles())
    data[8] = func(white_queen())
    data[9] = func(black_queen())
    data[10] = func(white_king())
    data[11] = func(black_king())
    data[12] = func(data[0] + data[2] + data[4] + data[6] + data[8] + data[10])
    data[13] = func(data[1] + data[3] + data[5] + data[7] + data[9] + data[11])
    #pinned
    data[14] = []
    data[15] = []
    return data

def pieces(data,c):
    d = np.zeros(64) > 0
    for i in range(c, 12, 2):
        d += data[i]
    return d
def white_pieces(data):
    return pieces(data,0)
def black_pieces(data):
    return pieces(data,1)



