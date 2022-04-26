# ha ("Haskell Archiver")

## Task

Лабораторная работа № 2
Используя Haskell (`ghc` с ключем `-threaded`), создать многопоточное приложение:
    1. Реализовать многопоточную версию сжатия множества файлов в один архив, используя алгоритм Хаффмана. Показать эффективность вашего алгоритма (по времени) на многопроцессорной системе.

## Implementation

### Compile
```sh
ghc --make -threaded -dynamic lab2.hs
```

### Create archive (parallel)
```sh
./lab2 +RTS -N -RTS create-par archive.ha file1.txt file2.txt
```

### Create archive (singlethreaded)
```sh
./lab2 +RTS -N -RTS create archive.ha file1.txt file2.txt
```

or just:
```sh
./lab2 create archive.ha file1.txt file2.txt
```

### Extract
```sh
./lab2 extract archive.hs
```
