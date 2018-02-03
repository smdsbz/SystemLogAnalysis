# SystemLogAnalysis
Data-Structure Curriculum Design, Grade 2 First-Half, Topic 1

## LightBulbs

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
|   +-- StorageGraph/
|       +-- StorageGraph.h
+-- test_env/                   // all tests should be done in here
+-- makefile
```


## Fun-Facts

 1. (20180102) Finally! It can read from the dumped text file!
    I spent a whole afternoon adjusting the parameters in `HashFunc`, but I
    found that it couldn't be improved no more.
    Guess what? It was because that the log itself have repetitive parts, and
    there *were a lot of them!*


