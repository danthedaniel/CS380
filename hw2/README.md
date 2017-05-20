Block Puzzle Part 2
===

Solve a block puzzle with A* using a couple of heuristics:

* Manhattan distance
* A Custom distance

### Building

The project can be built with `make`.

### Custom Heuristic Information

*The implementation for this heuristic can be found in search.c as the function
named custom_heuristic*

The heuristic that I'm using is a modification of the manhattan distance
heuristic, but for each grid position along the path from the upper-left corner
of the goal block (block #2) to the upper-left corner of the goal that is empty
the heuristic is dropped by 1 (to a minimum of 0).

This heuristic aims to judge to minimum number of moves to move the goal block
into the goal. If the entire board is clear, and the block must reach the goal
2 positions down and 5 over, then the manhattan distance of 7 would correctly
judge the cost of the solution to be 7. This heuristic can only ever be less
than that distance, so it can not over-estimate the cost to the goal and is
therefor admissible.

### Running

The project can be ran by executing the executable built by make:

e.g.:

```bash
./hw2
```
