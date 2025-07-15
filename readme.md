# Advanced Scientific Calculator

**Introduction**

This is a scientific calculator developed using Arduino, which features an LCD display and two keypads, allowing users to perform various mathematical operations from basic arithmetic to complex scientific calculations.

**What's in this project**
- Arduino file of implemented code
- Proteus file for simulation

**Used devices (Proteus)**
- Arduino Uno
- 4*6 matrix keypad for basic mode
- 4*4 small Matrix Keypad for scientific mode
- LM016L LCD Screen
- LED

**Features**
- Basic arithmetic operations: addition, subtraction, multiplication, division, percentage, and exponentiation.
- Special functions: square root, reciprocal, square, negation.
- Scientific functions: trigonometric functions (sin, cos, tan), inverse trigonometric functions (asin, acos, atan), logarithmic functions (log base 10, natural log), exponential, factorial, power of 10, binary and hexadecimal conversions.
- Mode toggle: switch between basic calculator mode and scientific calculator mode.
- Memory functionality to store and recall values.
- Error handling for invalid operations such as division by zero or invalid inputs.

**Tips**
- Use the keypad to input numbers and select operations.
- Press the mode toggle keys ('Base' on scientific keypad and 'Sci' on basic keypad) to switch between basic and scientific modes.
- In basic mode, use keys for digits, decimal point, basic operators, and special functions.
- In scientific mode, use keys for advanced mathematical functions. First, enter the number using basic mode, then change to scientific mode and press any function key for calculation
- To use Binary & Hexadecimal function keys, first enter the number you'd like to be converted using basic mode, then change to scientific mode and press 'Bin' or 'Hex' function keys for conversion to the chosen base. The answer will be will be displayed on the LCD screen for 5 seconds and screen will be cleared afterwards. 
- Results and error messages will be displayed on the LCD screen.

**Libraries**
- Keypad.h
- LiquidCrystal.h
- math.h
- <a href="https://www.theengineeringprojects.com/2015/12/arduino-uno-library-proteus.html">ArduinoUNOLibraryforProteus</a> 
