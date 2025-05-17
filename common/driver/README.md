# Drivers

This directory contains abstract device driver interfaces for on-chip peripherals. These are intended to act as a common interface that each hardware target (i.e. microcontrollers, microprocessors) can encapsulate their own specific driver within, but plug into any higher level piece of software that accepts and uses the defined interface.

**[bus](./bus/)**: Contains interfaces for serial communication protocols, such as SPI and UART.

**[io](./io/)**: Contains interfaces for I/O style hardware, such as GPIO, PWM, and Encoders.

**[memory](./memory/)**: Contains interfaces for embedded memory access, like flash.


**[platform](./platform/)**: Contains sets of factory functions and drivers for each hardware target, abiding by the above interfaces. For each unique hardware target selected during the build process (other than native as of now), there should be a corresponding directory in this folder that implements all of the available hardware on the chip.

