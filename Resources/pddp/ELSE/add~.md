---
title: add~

description: Signal accumulator

categories:
 - object

pdcategory: General

arguments:
- type: float
  description: starting sum
  default: 0

inlets:
  1st:
  - type: float/signal
    description: value to accumulate
  - type: bang
    description: resets sum to starting point
  - type: set <float>
    description: sets starting sum
  2nd:
  - type: signal
    description: impulse resets to starting point

outlets:
  1st:
  - type: signal
    description: the accumulated value

draft: false
---

[add~] starts accumulating input values to a starting sum value (0 by default). The starting sum can be set as an argument or via the 'set' message. You can reset the sum to the starting sum value with a bang or with an impulse signal to the right inlet.
