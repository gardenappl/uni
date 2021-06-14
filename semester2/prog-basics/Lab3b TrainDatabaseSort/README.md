I used the ["comparator chaining" pattern](https://stackoverflow.com/questions/1421322/how-do-i-sort-a-list-by-different-parameters-at-different-timed) to sort by multiple parameters. See `include/Sort.h` for additional comments.

Benchmark results:

```Sort by train type (counting sort)... 0.263077 s
Sort by train ID (radix sort)... 0.262024 s
Sort by popularity (optimized merge sort)... 0.486897 s
Sort by popularity and train type (optimized merge sort)... 1.35427 s
Sort by train type and popularity (optimized merge sort)... 2.35967 s
Sort by train type, popularity and ticket ID (optimized merge sort)... 2.37497 s
Sort by train type, popularity, ticket ID and train ID (optimized merge sort)... 2.31064 s 
```

Also includes an interactive mode.
