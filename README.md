# AIMAS Project
Project for AIMAS

## Algorithms
Work in progress algorithms

### Pre-analysis
```
PreAnalysis:
  Perceive the world
  Calculate (Manhattan) distances from initial state
  Define goals
  Divide goals to agents (?)
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
