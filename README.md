Simple chess engine with bitboards done as a learning exercise in C.

Compile the file 'compilethis.c' in the root directory. Type in commands like *exg8=Q_*. Remember to add '_' after each command. It might work and **it might not**.

### Done

     1. Board representation (uses classical bitboards, no rotation or magic numbers) including pesky enpassants. 

     2. Parser - gets through all the games with no bugs, but not actually verified that it makes the right moves.

     3. Basic alpha beta search.

     4. Basic transpose table and hashing.

     5. Opening book. Well kinda. Just picks the most common move from the parsed internet games.

     6. Search - Move ordering/iterative deepening. Works. Can get to depth 6 pretty quick but will really struggle to go much further before the end of the universe.

     7. Command line interface.

### In Process

     1. Simple flask app with chess.js and some pipes so it can be played in a browser

     2. An evaluation function that does more than just count material.

### To do

    1. More search. Maybe some multi threading?

    2. Still can't seem to legally castle or enpassant off its own accord.

    3. Still has no concept of checkmate, though it avoids check like the plague.
    
    4. Interface - How difficult would it be to plug it into winboard?

    5. Evaluation - Purely counting pieces at the moment. Kind of hoped to dive into tensor flow and train on a bunch of internet game data. However, I sincerely doubt the basic engine is fast enough for this additional overhead.