Some benchmark results for double queues:
```
LinkedDeque test, random operations:                 21,668,508 op/s (space constraint: 50331648 entries)
LinkedDeque test, one way fill & clear:              23,377,645 op/s
LinkedDeque test, two way fill & clear:              19,040,206 op/s
Pre-allocated ArrayDeque test, random operations:    30,817,406 op/s (space constraint: 67108864 entries)
Pre-allocated ArrayDeque test, one way fill & clear: 82,987,963 op/s
Pre-allocated ArrayDeque test, two way fill & clear: 64,828,658 op/s
VectorDeque test, random operations:                 822,765 op/s  (space constraint: 16777216 entries)
VectorDeque test, one way fill & clear:              77,887,620 op/s
VectorDeque test, two way fill & clear: (around 50,000 op/s, freezes before finishing)
```

(two-way means we push elements to the left, then to the right, then left, then right, ..., same with popping).
Inserting at the start of a VectorDeque is painfully slow because the entire internal array needs to be re-allocated every time. In fact, the program tends to freeze during the two-way test, unable to allocate enough memory for yet another move.

See `test-results.txt` for more info.
