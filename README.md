# Arduino Feedback Library 

A non-blocking user feedback library for Arduino based system.

## Installation

Clone this library into Arduino/libraries.

## Usage

Add the following line to your project.
   
   #include "Feedback.h"

then, in the loop add

   fb.Handle();

You can then call Feedback methods like fb.beep(3).

See Feedback.h for further documentation.
