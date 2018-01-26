# SystemLogAnalysis
Data-Structure Curriculum Design, Grade 2 First-Half, Topic 1

## LightBulbs

### Structure

```
SystemLogAnalysis/
+--- utils/
|   +--- LogClass/        // basic wrapper for log messages
|   |   +--- LogClass.h
|   |   +--- Misc.cpp
|   |   +--- LogClass.cpp
|   +--- HashTable/       // enable fast search
|   +--- Graph/           // psudo "Graph" actually...
|   |                     //   logs are quite *linear* already
|   ...
+--- test_env/            // all tests should be done in here
+--- makefile
```

