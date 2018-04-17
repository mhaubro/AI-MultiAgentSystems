# AIMAS
Added from last time: C++ conversion

Run by e.g. doing ./runSearchClient level (Without extension), so fx ./runSearchClient SAsoko2_48

Design choices changed: Everything is now x and y, there is no col and row left except where things are initialized.

Build by using "make", "make all" will also build auxiliary programs, which is also okay. Clean by using "make clean".

More info to be added.


## Algorithms
Work in progress algorithms

### Pre-analysis
```
PreAnalysis:
  Perceive the world
  Calculate (Manhattan) distances from initial state
  Define goals
  Define subgoals
  Divide subgoals to agents (?)
```

### MultiAgent
```
MultiAgent:
  while goals exist:
    Perceive current world
    Select goal with highest priority (?)
    Calculate plan to goal
    if no solution to plan:
      broadcast for help
      execute(noop)
      continue
    Take head of plan
    if head of plan is valid:
      execute head of plan
    else:
      negotiate or re-plan (?)
```

## TODO List
1. Everything
2. Win competition
