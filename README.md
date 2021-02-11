# ledDriver
The ledDriver is a very simple PCB containing an Arduino Nano, a few resistors, a rectifier and a bunch of connectors. It has 12 connectors to connect six leds to without the need of a extra 
resistor. The ledDriver has fixed onboard resistors and all leds are individual dimmable using the Arduino's pwm pins. 

On the other side of the board there are six more connectors ment for turning off and on the leds individually. All leds are enabled by default.

As a suplement, the ledDriver is able to run certain hard coded programs. So police cars, firetrucks, welders and fire-place lighting will all be possibilities. Some of the programs
may use 2 leds which can work togather.

## configuring the ledDriver
Configuring the led's brightnesses and programs can be done using a standard rotary encoder.

By default all six leds light up or run their programs (provided they are enabled by their switches). When an encoder is attached, a push on the switch will trigger configuration mode.
In configuration mode all six leds will go dark. By turning the encoder,  one can select one of the leds. A selected LED will light up. By holding down the encoder's switch for 1 second 
will allow one to configure it's brightness. By holding down the switch for two seconds will allow one to configur a pre-defined program.

When configuring the brightness, one can simply turn the encoder left and right until the desired intensity is met. By pressing the switch, the configuration mode is quit.

When configuring the programs, one can simply turn the encoder left and right untill the desired program is found. There are 20 different programs to choose from. Take note that
the program number flows over from 20 to 0 and from 0 to 20. By pressing the switch, the configuration mode is quit.

