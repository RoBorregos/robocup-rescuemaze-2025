# SoftwareSerial library for Arduino

This directory contains the SoftwareSerial library split out from https://github.com/Arduino-org/Arduino. All relevant history is retained.

We split it out because it needs some TLC to be compatible with our Modbus library (in particular, the APIs didn't match the modern serial port APIs, and it will no longer compile with modern compilers -- see [issue 234](https://github.com/arduino/ArduinoCore-samd/issues/234)).

[![GitHub release](https://img.shields.io/github/release/mcci-catena/SoftwareSerial/all.svg)](https://github.com/mcci-catena/SoftwareSerial/releases/latest) ![GitHub commits](https://img.shields.io/github/commits-since/mcci-catena/SoftwareSerial/latest.svg)

## Change Summary

### v1.0.0

This is the version as imported from Arduino.

### v2.0.0

This library contains several "unbreaking changes" -- it breaks old code, but makes the library compatible with new code.

The changes:

- `SoftwareSerial::flush()` no longer empties the RX queue. Since there is no TX queue with this library, this API does nothing.

- `SoftwareSerial::begin(unsigned long speed)` matches modern APIs for `UART` -- the old `SoftwareSerial::begin(long speed)` maps to the new API.

- Added `SoftwareSerial::begin(unsigned long speed, uint16_t config)`. This is currently only for compatibility with `UART`; the config is not yet used.

- Added `SoftwareSerial::drainRead()` to do what flush() used to do, in case you are not happy with the recommended `while (mySerial.read() >= 0) /* spin */;` work-around.

- Added fix for [issue 234](https://github.com/arduino/ArduinoCore-samd/issues/234).

### v3.0.0

The changes are:

- Fixed issue - ```call of overloaded 'begin(int)' is ambiguous```.

- Modify file names to get library added in Library Manager.