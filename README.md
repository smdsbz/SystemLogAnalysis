# SystemLogAnalysis
Data-Structure Curriculum Design, Grade 2 First-Half, Topic 1

## LightBulbs

### How to use?

```
$ make build
```

- Run without arguments
```
$ ./a.out
```

- Run with argument
```
$ ./a.out path/to/dumped-syslog
```

### Structure

```
SystemLogAnalysis/
+-- utils/
|   +-- LogClass/               // basic wrapper for log messages
|   |   +-- LogClass.h
|   |   +-- LogDate.cpp
|   |   +-- LogMessage.cpp
|   |   +-- LogRecord.cpp
|   |   +-- LogRecord_iterator.cpp
|   +-- HashTable/              // enable fast search
|   |   +-- HashTable.h
|   |   +-- HashCell.cpp
|   |   +-- HashTable_test.cpp
|   +-- StorageGraph/           // could query along multiple axes intuitively
|   |   +-- StorageGraph.h
|   +-- FPTree/                 // pattern digging, ack. Jiawei Han et. al.
|   |   +-- FPTree.h
|   +-- UIGadgets/
|       +-- UIGadgets.h
|
+-- test_env/                   // all tests should be done in here
|
+-- Misc.h                      // some UI logic helper funcs
+-- main.cpp
+-- makefile
```

### Starring

- FP-Tree (Jiawei Han et. al.)
- Adaptive HashTable, via tuning the *sigmoid* function

## Fun-Facts

 1. (20180102) Finally! It can read from the dumped text file!
    I spent a whole afternoon adjusting the parameters in `HashFunc`, but I
    found that it couldn't be improved no more.
    Guess what? It was because that the log itself have repetitive parts, and
    there *were a lot of them!*


