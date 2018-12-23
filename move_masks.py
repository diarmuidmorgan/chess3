from start_state import blank
import numpy as np
def knight_moves(x,y,board=blank()): 
    moves = [[1,-2],[1,2],[-1,2],[-1,-2],[2,1],[2,-1],[-2,1],[-2,-1]]
    for m in moves:
        i = (x+m[0] * 8) + m[1] + y
        if i >= 0 and i < 64:
            board[i] = True
    return board
def rank_x(x,board=blank()):
    board[8*x:8*x + 8] = True
    return board
def rank_y(y,board=blank()):
    for i in range(0, 8):
        board[i*8 + y] = True
    return board
def rook_moves(x,y):
    return rank_x(x) + rank_y(y)
def diagonal(x,y,nx,ny,board=blank()):
    i=x;j=y
    while 0<= i <= 7  and 0<= j <= 7:
        board[8*i + j] = True
        i+= nx
        y+= ny
    i=x; j= y
    while 0 <=i <= 7 and 0 <=j <= 7:
        board[8*i + j] = True
        i-= nx
        y -= ny
    return board
def bishop_moves(x,y):
    return diagonal(x,y,1,1) + diagonal(x,y,-1,-1)
def queen_moves(x,y):
    return bishop_moves(x,y) + rook_moves(x,y)
def king_moves(x,y,board=blank()):
    moves = [[1,0],[-1,0],[0,1],[0,-1],[1,1],[1,-1],[-1,1],[-1,-1]]
    for move in moves:
        i = x + move[0]
        j=y+move[1]
        q = 8 * i + j
        if 0<=i<=7 and 0<=j<=7:
            board[q] = True
    return board
def king_side_castles(rank,board):
    board[rank*8 + 5] = True
    board[rank * 8 * 6] = True
    return board
def queen_side_castles(rank,board):
    board[rank * 8 + 2] = True
    board[rank * 8 + 3] = True
    return board
def check_king_side_castles(rank,board):
    board[rank*8+5:rank*8+7] = True
    return board
def check_queen_side_castles(rank,board):
    board[rank*8+1:rank*8+4] = True
    return board
def pawn_attacks(x,y,direction,board=blank()):
    ys = [1,-1]
    for m in ys:
        i = x+direction
        j = y + m
        if 0<=i<=7 and 0<=j<=7:
            board[i*8 + j] = True
    return board
def pawn_forward(x,y,direction,board=blank()):
    if 0<= x + direction <= 7:
        board[(x+direction)*8 + y] = True
    if x == 1 and direction == 1:
        board[(x+2) * 8 + y] = True
    elif x == 6 and direction == -1:
        board[(x-2) * 8 + y] = True
    return board

def enpassants():
    pass
    
def mask_dictionary():
    d = []
    for i in range(0,8):

        for j in range(0,8):
          a = {}
          a['knight'] = np.packbits(knight_moves(i,j))
          a['rook'] = np.packbits(rook_moves(i,j))
          a['bishop'] = np.packbits(bishop_moves(i,j))
          a['queen'] = np.packbits(queen_moves(i,j))
          a['king'] = np.packbits(king_moves(i,j))
          p = {}
          p['white_forward'] = np.packbits(pawn_forward(i,j,1))
          p['black_forward'] = np.packbits(pawn_forward(i,j,-1))
          p['white_attack'] = np.packbits(pawn_attacks(i,j,1))
          p['black_attack'] = np.packbits(pawn_attacks(i,j,-1))
          a['pawn'] = p
            


          d.append(a)
    return d

d = mask_dictionary()




            

        

