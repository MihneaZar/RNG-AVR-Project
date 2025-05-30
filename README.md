# RNG AVR Project
Project for a simple ATmega328P program that prints on an LCD pseudo-randomly-generated numbers.

Libraries: avr/io.h, avr/interrupt.h, stdio.h.  
3rd-party resources: https://github.com/Matiasus/SSD1306/tree/master/lib.

Functions: 
- main.c - init_all() initializes all modules, Set_time() is an interface for setting the time.
- lib:
  - spi, twi - for communication between ATmega and modules;
  - timer, button_interrupt - timer and buttons;
  - ssd1306, sd_reader: lcd and microsd module.
  - miscellaneous: printing functions to lcd. 

</br>
<p align="center">
  <img src="hardware/hardware_pic.jpg" width="500"/>
</p>
</br>

Demo: https://youtube.com
