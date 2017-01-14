Alarm Clock
===========

Arduino Pro Mini based alarm clock.

Author: Jyotiprakash Sahoo

Email: jp.sahoo1991@gmail.com

Github: www.github.com/weerdmonk

I started this project with the intension of making an alarm clock which would
also show the humidity and temperature. The outcome was not exactly what I had
planned for but I am quite satisfied with the way things have turned out to be
within a short time and limited resources.

All components are housed in pringles can (American potato chips) with the LCD
mounted on the lid. A servo motor with wights attached acts as a pendulum to
sway the can sideways when the alarm triggers. Fun isn't it!


The bitmaps were converted to character maps for displaying custom images
using LCD Assistant.

http://en.radzio.dxp.pl/bitmap_converter

Thanks and cheers!! If you like his website please donate!


Features

- NOKIA 3310 monochrome LCD
- Programmable alarm with melody
- Humidity and Temperature readouts
- Requires 6v - 12v dc supply

Hardware:

- Arduino Pro Mini board
- PCD8544 LCD (NOKIA 3310)
- DS1307 RTC module
- DHT11 Temp-Humidity sensor
- SG90 Servo for pendulum
- Generic buzzer

NOTES:

- Using classes added huge overhead. Code size increases significantly.
- Display library uses a buffer which takes up most of RAM on the ATMega328.
- The code wss for rapid application. The entire project took 26 days to finish
  from scratch to the end result.
- There is scope for huge amount of optimization.

TODO:

- Add battery module
- Interface for setting time
- Better alarm melody
- Calendar fucntion
- Real Rime scheduling
- Optimize code
