Some benchmark results for double queues:
```
LinkedDeque test, random operations:                 21,176,884 op/s (space constraint: 50331648 entries)
LinkedDeque test, one way fill & clear:              19,849,831 op/s
LinkedDeque test, two way fill & clear:              23,777,946 op/s
Pre-allocated ArrayDeque test, random operations:    26,506,885 op/s (space constraint: 67108864 entries)
Pre-allocated ArrayDeque test, one way fill & clear: 52,506,160 op/s
Pre-allocated ArrayDeque test, two way fill & clear: 54,221,098 op/s
VectorDeque test, random operations:                 7,846,371 op/s  (space constraint: 16777216 entries)
VectorDeque test, one way fill & clear:              18,012,351 op/s
VectorDeque test, two way fill & clear: (around 30,000 op/s, freezes during test)
```

(two-way means we push elements to the left, then to the right, then left, then right, ..., same with popping).
Inserting at the start of a VectorDeque is painfully slow because the entire internal array needs to be re-allocated every time. In fact, the program tends to freeze during the two-way test, unable to allocate enough memory for yet another move.

See `test-results.txt` for more info.
