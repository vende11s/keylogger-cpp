## Contents
- [Constructor](#Constructor)
- [setFilename](#setFilename)
- [setConsoleLogging](#setConsoleLogging)
- [start](#start)
- [Running keylogger as a thread](#Running-keylogger-as-a-thread)
  - [Example of threaded code](#Example-of-threaded-code)
  - [startThread](#startThread)
  - [stopThread](#stopThread)
  - [getLogs](#getLogs)
  - [state](#state)

### Constructor
constructor takes 2 arguments:
* string: filename
* bool: console_log 

if console_log is set to true, then all logs are also directed to console output (clog)

### setFilename()
arguments:
* string: filename

when you won't specify filename either in constructor or using this function, keylogger throws exception

### setConsoleLogging()
arguments:
* bool: console_log

enables/disables logging also to console

### start()
starts logging keystrokes to file

## Running keylogger as a thread
You can implement this code to some rat, then it's useful to use keylogger as a thread

**Notice:** when scope ends, keylogger stops working

### Example of threaded code
```cpp
#include <iostream>
#include <Windows.h>
#include "keylogger-cpp/keylogger.hpp"

using namespace std;

int main(){
  keylogger::keylogger log("data.txt");
  log.startThread();
  
  while(true){
      Sleep(10000);
      cout<<log.getLogs()<<endl;
  }

}

```

### startThread()
starts keylogger as a thread

**Notice:** when you run it twice on one object, nothing happens 

### stopThread()
stops keylogger thread

### getLogs()
returns:
* string: logs

### state()
returns:
* bool: state_keylogger
