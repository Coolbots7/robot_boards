#ifndef DEBUG_H
#define DEBUG_H

/*
   Abstract class for debuggable objects
*/

class Debuggable {
  public:
    virtual String debug();
};

/*
   The purpose of this class is to debug objects
   and easily control what, if anything, is debugged.
*/

//TODO debug singleton

class Debug {
  private:
    bool enabled;

    uint8_t object_count;
    Debuggable *objects[5];


  public:
    Debug(bool enabled = false) {
      this->enabled = enabled;

      this->object_count = 0;

      this->info("============\n");
      this->info("Wireless EStop\n'");
      this->info("Hardware Ver: \n");
      this->info("Firmware Ver: \n");
      this->info("============\n\n");
    }

    /*
       Enable or disable all debugging.
       @param enable  Enable by default.
    */
    void enable(bool enabled = true) {
      this->enabled = enabled;
    }

    /*
       Disable all debugging.
    */
    void disable() {
      this->enable(false);
    }

    bool getEnabled() {
      return this->enabled;
    }

    /*
      Print messages to serial while enforcing all debug logic.
      @param message  String to print to serial.
    */
    void print(String message) {
      // Skip if debugging is disabled
      if (!this->enabled) {
        return;
      }

      Serial.print(message.c_str());
    }

    void info(String message) {
      this->print("INFO: " + message);
    }

    void debug(String message) {      
      this->print("DEBUG: " + message);
    }

    void warning(String message) {
      this->print("WARN: " + message);
      
    }

    void error(String message) {
      this->print("ERROR: " + message);
      
    }

    void addObject(Debuggable *object) {
      if (this->object_count > 5 - 1) {
        return;
        this->warning("Max of 5 objects have been added for debugging.\n");
      }
      this->objects[this->object_count] = object;
      this->object_count = this->object_count + 1;
    }

    void update() {
      Serial.print(String(millis()) + "ms:\t");

      for (uint8_t i = 0; i < this->object_count; i++) {
        Serial.print(this->objects[i]->debug());
        if (i < this->object_count - 1) {
          Serial.print("\t");
        }
      }

      Serial.print("\n");
    }
};

#endif
