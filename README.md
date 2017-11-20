# WiFi-Modbus-TCPtoRTU

NOTE: ACTUALLY, THIS IS A RAW "GOOGLE" TRANSLATION FROM CYRILIC TO ENGLISH!!
WiFi ESP8266 gateway Modbus TCP to Modbus RTU 

The task:

- The device is connected via a wireless connection (WiFi) via the uart or rs-485 interface to various devices for transferring the Modbus protocol from the TCP / UDP environment to the RTU.
- The device should be small in size and be powered from the device to which it is connected.
- The device must provide simultaneous connection of up to 4 TCP / UDP devices simultaneously.
- The device must support the matching of requests from the master when more than one Modbus request is present in one TCP packet.
- The device must support one telnet connection port 23 for setting up and diagnosing the WiFi device.
- The device should be finished both in terms of use (universal) and have a body and body version.

Implementation:

- As a basis, the Chinese-made ESP-01 card with WiFi module ESP8266 1 MB. At the output, we have 1 uart and 1 - GPIO.
- The program is written on espressos SDK under arduino.
- The IDE Platform io is used.

Implemented by:

- The ability to connect 4 TCP masters.
- UART with a speed of 115200 bit / c.
- A port is opened for connecting Telnet port 23. This connection displays debug information trace.print and so on.
- Testing was carried out using Modbus pull requests over TCP and Modbus slave responses over RTU.
- Save the uart port settings and Telnet access point settings to the nonvolatile memory.

It remains to implement:

- UDP support. (At the same time on 502 the port will not get on, or switch or put on another port)
- Make a model of 3d case for printing on a 3D printer.
- Develop a scheme for powering and pairing this device.

If there are interested persons, I invite to the company.

