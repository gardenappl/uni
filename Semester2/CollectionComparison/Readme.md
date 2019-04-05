Some interesting benchmark results for double queues:
```
LinkedDeque test, one way fill & clear:              13,381,802 op/s
LinkedDeque test, two way fill & clear:              11,663,490 op/s
Pre-allocated ArrayDeque test, one way fill & clear: 13,417,070 op/s
Pre-allocated ArrayDeque test, two way fill & clear: 25,962,462 op/s
VectorDeque test, one way fill & clear:              8,981,873 op/s
VectorDeque test, two way fill & clear: (around 10,000 op/s, freezes during test)
```

Linked list and array implementations are very close in performance in one-way tests, however ArrayDeque seems to be twice as fast in two-way tests for some reason.
(two-way means we push elements to the left, then to the right, then left, then right, ..., same with popping).
Some kind of compiler optimization?

Inserting at the start of a VectorDeque is painfully slow because the entire internal array needs to be re-allocated every time.


Full execution log:

```
LinkedDeque test, one way fill & clear: 
0.000255126 s, entries tested: 1024
0.000342331 s, entries tested: 2048
0.000705133 s, entries tested: 4096
0.00139998 s, entries tested: 8192
0.00269883 s, entries tested: 16384
0.008146 s, entries tested: 32768
0.00787105 s, entries tested: 65536
0.0127747 s, entries tested: 131072
0.0277768 s, entries tested: 262144
0.0517984 s, entries tested: 524288
0.103292 s, entries tested: 1048576
0.187529 s, entries tested: 2097152
0.365284 s, entries tested: 4194304
0.74928 s, entries tested: 8388608
1.44808 s, entries tested: 16777216
2.93822 s, entries tested: 33554432
3.59316 s, entries tested: 41943040
4.11604 s, entries tested: 50331648
4.88324 s, entries tested: 58720256
5.6418 s, entries tested: 67108864
Average per 5 seconds: 66909011

LinkedDeque test, two way fill & clear: 
0.000119084 s, entries tested: 1024
0.000167696 s, entries tested: 2048
0.00033113 s, entries tested: 4096
0.000659001 s, entries tested: 8192
0.00134545 s, entries tested: 16384
0.00294456 s, entries tested: 32768
0.00570969 s, entries tested: 65536
0.0118287 s, entries tested: 131072
0.0242635 s, entries tested: 262144
0.0525947 s, entries tested: 524288
0.0920367 s, entries tested: 1048576
0.203085 s, entries tested: 2097152
0.366224 s, entries tested: 4194304
0.801894 s, entries tested: 8388608
1.45036 s, entries tested: 16777216
3.24937 s, entries tested: 33554432
3.65424 s, entries tested: 41943040
4.6637 s, entries tested: 50331648
4.95799 s, entries tested: 58720256
6.47297 s, entries tested: 67108864
Average per 5 seconds: 58317453

Pre-allocated ArrayDeque test, one way fill & clear: 
8.8215e-05 s, entries tested: 1024
0.00019238 s, entries tested: 2048
0.000385247 s, entries tested: 4096
0.000616102 s, entries tested: 8192
0.0012196 s, entries tested: 16384
0.00260245 s, entries tested: 32768
0.00571969 s, entries tested: 65536
0.0102894 s, entries tested: 131072
0.0256558 s, entries tested: 262144
0.0501987 s, entries tested: 524288
0.0958936 s, entries tested: 1048576
0.18366 s, entries tested: 2097152
0.363665 s, entries tested: 4194304
0.747485 s, entries tested: 8388608
1.54704 s, entries tested: 16777216
3.01556 s, entries tested: 33554432
3.61822 s, entries tested: 41943040
4.38963 s, entries tested: 50331648
5.00175 s, entries tested: 58720256
Average per 5 seconds: 67085351

Pre-allocated ArrayDeque test, two way fill & clear: 
1.56825 s, entries tested: 1024
0.000132443 s, entries tested: 2048
0.000173608 s, entries tested: 4096
0.000383715 s, entries tested: 8192
0.000724991 s, entries tested: 16384
0.0015533 s, entries tested: 32768
0.00269899 s, entries tested: 65536
0.00582608 s, entries tested: 131072
0.0111574 s, entries tested: 262144
0.0239711 s, entries tested: 524288
0.0466241 s, entries tested: 1048576
0.0867093 s, entries tested: 2097152
0.17395 s, entries tested: 4194304
0.363027 s, entries tested: 8388608
0.730686 s, entries tested: 16777216
1.42962 s, entries tested: 33554432
2.94184 s, entries tested: 67108864
3.1639 s, entries tested: 75497472
3.45092 s, entries tested: 83886080
3.82041 s, entries tested: 92274688
4.13925 s, entries tested: 100663296
4.49565 s, entries tested: 109051904
4.86736 s, entries tested: 117440512
5.16968 s, entries tested: 125829120
Average per 5 seconds: 129812310

VectorDeque test, one way fill & clear: 
0.000185977 s, entries tested: 1024
0.000288229 s, entries tested: 2048
0.000533039 s, entries tested: 4096
0.00101734 s, entries tested: 8192
0.00201111 s, entries tested: 16384
0.00410224 s, entries tested: 32768
0.00827394 s, entries tested: 65536
0.015661 s, entries tested: 131072
0.0345126 s, entries tested: 262144
0.0671453 s, entries tested: 524288
0.127734 s, entries tested: 1048576
0.253655 s, entries tested: 2097152
0.504212 s, entries tested: 4194304
1.00036 s, entries tested: 8388608
2.0193 s, entries tested: 16777216
3.24093 s, entries tested: 25165824
4.02885 s, entries tested: 33554432
5.60369 s, entries tested: 41943040
Average per 5 seconds: 44909367

VectorDeque test, two way fill & clear: 
0.00201898 s, entries tested: 1024
0.00795977 s, entries tested: 2048
0.0316055 s, entries tested: 4096
0.17081 s, entries tested: 8192
0.582907 s, entries tested: 16384
3.25616 s, entries tested: 32768
(program freezes)
```
