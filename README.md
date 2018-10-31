# Amiga DRAM tester for HYB-514256B with Arduino UNO 

The 514256b ram chips are 4bit 256KB with 20pin and in A500+ there are 8 of them.  
So they are 8*4*256 = 8192 bit = 8192 / 8 = 1024 bytes = 1Kb.  
  
![](img/arduino.jpg)  
  
## Please read carefully:
This project is on your own risk, and not intended for professional use.
You can use it on your own risk and I am not responsible for any damage by any reason.
Before you make the Arduino software test, you must check first with a multimemeter if your chip is broken to protect your hardware, see bellow

## What you will need:
A multimeter for checking if chip is broken.
You will also need a breadboard connecting cables and of course the ram chips to check.
Also a ziff socket will be good idea for securing the chip but the breadboard holes can also be used.

## Initial procedure before testing your chips:
 1- Check for shorted Vcc
 2- Inputs Ax, /CAS, /RAS, /WE, DIN must be high-Z. If shorted to 0v/5v,	it's broken.
 3- Output DOUT must be high-Z in the absence of RAS/CAS activity.
 4- Output DOUT is push-pull AFTER the RAS/CAS read sequence (see datasheet). If it's high-Z, it's broken.

##				Wiring diagram:
```

      Chip		  Arduino
Description / AA	  Description / Arduino Number  
 CHIP	 ARDUINO	     ARDUINO	CHIP  
-------+-----------	    ----------+--------  
IO1  1     A1   15       PD2   2    17  CAS  
IO2  2     A2   16       PD3   3    4   RAS  
WE   3     A5   19       PD4   4    --  --  
RAS  4     3    PD3      PD5   5    6   A0  
NC   5     --   --       PD6   6    7   A1  
A0   6     5    PD5      PD7   7    8   A2  
A1   7     6    PD6      PB0   8    9   A3  
A2   8     7    PD7      PB1   9    11  A4  
A3   9     8    PB0      PB2   10   12  A5  
VCC  10    +5V  +5V      PB3   11   13  A6  
A4   11    9    PB1      PB4   12   14  A7  
A5   12    10   PB2      PB5   13   15  A8  
A6   13    11   PB3      --    --   5   NC  
A7   14    12   PB4      +5V   +5V  10  VCC  
A8   15    13   PB5      14    A0   16  OE  
OE   16    A0   14       15    A1   1   IO1  
CAS  17    2    PD2      16    A2   2   IO2  
IO3  18    A3   17       17    A3   18  IO3  
IO4  19    A4   18       18    A4   19  IO4  
VSS  20    GND  GND      19    A5   3   WE  
--   --    4    PD4      GND   GND  20  VSS  
```  
  
## Special Thanks:
I would like to thank first of all the Amiga and Commodore community for keeping alive these very old but so beloved and wonderful machines, and all the people who spend a lot of time to share their work with others:  


Commodore 64 hardware test tools  
jamarju  
https://github.com/jamarju/c64_test_tools  


iss: for DRAMARDUINO - Dram tester with Arduino  
http://forum.defence-force.org/viewtopic.php?p=15035&sid=17bf402b9c2fd97c8779668b8dde2044  


Chris Osborn:  
http://insentricity.com/a.cl/252
