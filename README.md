More of an exercise in masochism/learning C then an actual chess engine that somebody would want to use.


### Done

     1. Board representation (uses classical bitboards) including pesky enpassants. 

     2. Parser - gets through all the games with no bugs, but not actually verified that it makes the right moves.

     3. Basic alpha beta search.

     4. Basic transpose table (hashes every move from scratch, no multi threading strategy)

### In process

     1. Opening book (should be easy).

     2. Search - Move ordering/iterative deepening.

     3. Incremental move hashing (think this should be easy)

     4. Command line interface.

### To do

    1. More search. Maybe some multi threading?
    
    2. Interface - Hopefully wrap everything in a Pyobject and make it playable in a browser? Or just provide a pipe that a pyapp can use. Not too sure if there is much point creating a winboard/xboard interface, just so it can be crushed into oblivion by stockfish.

    3. Evaluation - Just counting pieces at the moment. Kind of hope to dive into tensor flow and train on a bunch of internet game data. In reality the engine will probably be too slow already for an ML model to be feasible. 