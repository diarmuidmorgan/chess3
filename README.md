More of an exercise in masochism/learning C then an actual chess engine that somebody would want to use.


### In process

     Board representation, basic brute force search, memory management.

### To do

    1. Alpha beta, and maybe move ordering?  Possibly a transposition table. Maybe even some multi threading?
    
    2. Interface - Hopefully wrap everything in a Pyobject? Not too sure if there is much point creating a standard chess engine interface, just so it can be crushed into oblivion by stockfish.

    3. Evaluation - kind of hope to dive into tensor flow and train on a bunch of internet game data. In reality the engine will probably be too slow already for an ML model to be feasible. Maybe just add the pieces up and occasionally check for checkmate? 