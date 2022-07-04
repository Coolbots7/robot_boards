#ifndef ROBOT_BOARDS_DEBUG_H
#define ROBOT_BOARDS_DEBUG_H

#include "Arduino.h"

/*
   The purpose of this class is to debug objects
   and easily control what, if anything, is debugged.
*/
class Debug {
  private:
    bool enabled;

  public:
    // TODO pass serial object to use with default
    Debug(String deviceName, bool enabled);

    /*
       Enable or disable all debugging.
       @param enable  Enable by default.
    */
    void enable(bool enabled);

    /*
       Disable all debugging.
    */
    void disable();

    bool getEnabled();

    /*
      Print messages to serial while enforcing all debug logic.
      @param message  String to print to serial.
    */
    void print(String message);

    void info(String message);

    void debug(String message);

    void warning(String message);

    void error(String message);
};

/*
   Abstract class for debuggable objects
*/
class Debuggable {
  protected:
    Debug *debugger;

  public:
    void addDebugger(Debug *d);

    void print(String message);

    void info(String message);

    void debug(String message);

    void warning(String message);

    void error(String message);
};

#endif