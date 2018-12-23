def kernighan_piece_masks (A):
    """
    Generator for retrieving single piece
    Bit masks
    """
    A = A.copy()
    while A != 0:
        B = np.bitwise_and(A, A-1)
        yield A - B
        A = B

def piece_indexes(A):
    #is the use of numpy.log2 ridiculously expensive?
    for word in kernighan_piece_masks(A):
        yield np.log2(word)
