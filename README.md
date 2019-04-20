# Rotary

A Particle Photon is used to control several input devices to interface a MQTT network including:

* two rotary encoders
* four push button switches
* four LEDs associated with the push button switches
* a slide switch

### Rotary Encoder

The rotary encoder sends a one character payload for:

* r -- turn shaft clockwise
* l -- turn shaft counter-clockwise
* R -- turn shaft clockwise while pressing in, like a shift key
* L -- turn shaft counter-clockwise while pressing in, like a shift key
* 1 -- shaft pressed in
* 0 -- shaft released

### Push Button Switches

Each switch sends a one character payload for:

* 1 -- button pressed
* 0 -- button released

Each button has an associated LED which is turned on while the button is pressed.

### LED

There is an LED associated with each switch. The state of the LED currently is only controlled by the associated switch.

### Slide Switch

The slide switch is used to control the Particle MANUAL mode to allow the Photon to be used without having to connect to the Particle.io server. This is useful when the Photon is used on a WiFi network that does not or does not allow connections to particle.io, such as demos in schools, STEM fairs, etc.

#### Files

Code has been compile using the Particle command line interface.

- **src/rotary.ino** This is the main source code file.
- **src/address.h** This file holds the addressing information for the MQTT and the node name.
- **src/button.h** This file holds the class definiton for the button.
- **src/encoder.h** This file holds the class definition for the rotary encoder.
-**project.properties** This file specifies the name and version number of the libraries that your project depends on. Dependencies are added automatically to your `project.properties` file when you add a library to a project using the `particle library add` command in the CLI or add a library in the Desktop IDE.
-**Makefile** This file specifies the dependencies and the recipes for making
the various files for the system.
