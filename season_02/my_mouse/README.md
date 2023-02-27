# Welcome to My Mouse
This is the solution to the `my_mouse` problem in C.

## Task
Task is to find the shortest possible path between two characters in a maze whilst avoiding obstacles.  
Sadly, no diagonal moves, so the "normal" A* algorithm wouldn't work.

## Description
Using an [iterative deepening A*](https://en.wikipedia.org/wiki/Iterative_deepening_A*) (IDA*) path search algorithm.  

The time complexity of the algorithm is O(3<sup>m</sup>), where `m` is the number of steps needed to go from the entrance to the exit.  

Its space complexity is O(m).  

## Installation
Just run the command:
```
make && ./my_mouse <MAP_PATH>
```

Where `MAP_PATH` can be any valid path to a map file. For example, the complete command could be something like:
```
make && ./my_mouse ./maps/01.map
```

## Usage
See the above points.  
You can also run the binary directly after the first build if you don't plan on iteratively changing anything. 

### The Core Team

#### Developers
- [Petar (Shannarra) Angelov](https://github.com/Shannarra/), text editor: [Emacs](https://www.gnu.org/software/emacs/)
- [Chris Clayton](https://github.com/Chrs-Clytn/)

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
