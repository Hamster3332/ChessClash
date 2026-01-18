# ChessClash

A chess GUI with multiple fun engines to see which tactic is the best!

### Requirements

- C++ Compiler (Clang or gcc)
- [**SFML** library](https://www.sfml-dev.org/)
- CMake


### Current Features

- All Chess rules implemented
- A board gets rendered
- drag and drop pieces
- different bots / engines you can play against or play against each other [^1]
- online playing (WIP) [^1]

#### List of engines
- Bandit *pieces move towards the opponents king*
- UnBandit *pieces move away from the opponents king*
- Cuddle *pieces move towards your own king*
- Kamikaze *blunders all pieces*
- UnKamikaze *doesnt blunder all pieces*
- Random *plays random moves*
- Capture *if it can capture, it captures*
- Bad School Kid *a very simple try to create a decent bot with simple logic*


#### List of evaluators
    push,
    material,
    check


[^1] currently you would need to change the code before compiling to choose

