# Marble Solitaire Solver

Solves a marble solitaire game and outputs solutions to the console.

## Marble Solitaire explanation

Marble Solitaire uses a wooden board with a grid of marbles and one hole at the center. The goal is to get rid of all but one marble by jumping the marbles over each other. When a marble is jumped, it must go one of the four directions and land in an available empty space on the other side of a marble, removing the marble in between.

## Solver explanation

The solver uses a board state which uses `-1` to indicate an out-of-bounds position, `0` for empty, and `1` for a position with a marble in it.

To solve the board state, it looks for every possible move, and once it finds one, it recurses, logging the solutions to the console and adding them to a vector once done.
