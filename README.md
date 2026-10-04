# Sharp UP-700 Recovered Display Clock - Project overview

This project explores the reuse of a recovered **Sharp UP-700 POP UP DISPLAY PWB** as the basis for a standalone digital clock. The original display assembly was designed as part of a commercial point-of-sale system and contains a four-digit seven-segment display driven through a multiplexed interface. The objective of the project was to understand the original hardware, determine how the display could be controlled independently, and integrate it with a modern microcontroller to create a completely new application.

The project started with a simple piece of recovered hardware: the POP UP DISPLAY PWB from a Sharp UP-700 cash register.The board contains four 7-segment displays originally controlled by the cash register's main electronics. Instead of treating it as an obsolete component, the goal was to understand its electrical architecture and reuse it as the display of a completely new device.

The final system is:

                    🔋 Battery
                       │
                       │ VIN
                       ▼
              ┌──────────────────┐
              │ Arduino UNO R4   │
              │     Minima       │
              │                  │
              │      RTC         │
              └────────┬─────────┘
                       │
                 GPIO control
                       │
                       ▼
              ┌──────────────────┐
              │ Sharp UP-700     │
              │ POP UP DISPLAY   │
              │                  │
              │      12:34       │
              └──────────────────┘

The project combines hardware reverse engineering, electrical characterization, embedded programming and reuse of electronic hardware. Rather than replacing the original display with a modern module, the original PCB and its four seven-segment displays are retained and controlled by an **Arduino UNO R4 Minima**.


## Reverse Engineering

The first objective was to determine the function of the connector pins and establish how the four displays were controlled. The Sharp service manual showed that the POP UP DISPLAY PWB uses a 15-pin connector and that the display is controlled through separate segment and digit-selection lines.

The display contains the seven conventional segments, A through G, together with the decimal point. These eight lines are shared between the four digits. Additional lines are used to select which of the four physical digits is currently active. This means that the board does not require a separate set of eight segment signals for every digit. Instead, the same segment lines are reused while the active digit is changed rapidly.

The connector and PCB traces were inspected to establish the relationship between the connector pins, the display modules and the current-limiting resistors. Continuity measurements with a multimeter were used to confirm electrical connections and to distinguish the segment lines from the digit-selection lines.

Voltage measurements were then used to characterize the electrical levels involved in the original circuit. The service documentation indicates that the display circuitry operates around the 5 V logic domain, while the original system also contained a separate display supply. The recovered board includes 27 Ω resistors associated with the display segments, which are important because they limit LED current and must be taken into account when connecting the display to a different controller.

The reverse-engineering process therefore consisted of comparing three sources of information: the original Sharp schematics, the physical PCB layout and direct electrical measurements. This was important because the connector pin numbering alone does not provide enough information to safely assume how a line should be driven. 

## Arduino UNO R4 Minima
The recovered display is controlled using an **Arduino UNO R4 Minima**. This board was selected because it provides sufficient digital I/O for the display interface and includes an integrated Real-Time Clock (RTC). 

The software was developed incrementally, starting with basic GPIO tests and individual display segments before progressing to complete digit control and multiplexing. 

The program defines patterns for numerical digits and controls the four display positions independently. The decimal point is also controlled separately, allowing the display to represent a conventional clock format such as `14.37`.

The RTC is accessed through the Arduino RTC library. The program reads the current hour and minute and converts them into four individual numerical digits before sending the corresponding segment patterns to the display.

## Software Development
The software was developed experimentally while the hardware was being characterized. Rather than treating the display as a black-box component, each stage of the program was used to test an increasingly complete understanding of the hardware.

Initial tests controlled individual digits and segments. Numerical patterns were then introduced, followed by alphabetic patterns to verify that arbitrary segment combinations could be generated. A message-display function was subsequently implemented to demonstrate that the four digits could be controlled independently.

The RTC was then integrated into the same system. The current time is divided into four values:

```text
HH:MM
││ ││
││ └┴── minutes
└┴───── hours
```

Each value is converted into a seven-segment pattern and displayed using the multiplexing system.

## Autonomous Power
The final version of the project is intended to operate without a permanent USB connection to a computer. The Arduino can instead be powered through its **VIN input** from an external battery supply.

The UNO R4 Minima accepts an input voltage of 6–24 V through VIN according to Arduino's official documentation. The board regulates this input internally to its 5 V operating voltage. The barrel jack is also connected to VIN, providing another way of supplying the board independently of USB.

The planned configuration is therefore:
```text
Battery
   │
   ▼
VIN
   │
   ▼
Arduino UNO R4 Minima
   │
   ▼
Sharp UP-700 Display
```

## Hardware and Components
The main components used in the project are:

* Sharp UP-700 POP UP DISPLAY PWB
* Arduino UNO R4 Minima
* External battery supply
* Connection wiring

The Sharp display is the main recovered component of the project. The Arduino is used as the modern control system, replacing the original Sharp electronics responsible for generating the display signals.

## Repair and Reuse
An important principle behind this project is that electronic hardware should not automatically be treated as disposable simply because the original product is obsolete.

The project follows the same general philosophy promoted by the **Right to Repair** movement: access to repair information, tools, spare parts and technical documentation makes it possible to extend the useful life of existing hardware rather than replacing it unnecessarily. iFixit has been particularly influential in promoting this approach and in making repair documentation and practical repair knowledge accessible.

The project therefore follows a simple principle:

> **Repair or reuse existing hardware before buying new hardware when practical.**

In this project, an obsolete point-of-sale display becomes a programmable clock. The original hardware is not discarded simply because its original system is no longer being used. Its function is reinterpreted and extended through reverse engineering and modern control electronics.

## References

* Sharp UP-700 Service Manual — technical documentation used for reverse engineering the display hardware.
* Arduino UNO R4 Minima documentation — microcontroller, GPIO, RTC and power specifications.
* [iFixit — Right to Repair](https://www.ifixit.com/Right-to-Repair?utm_source=chatgpt.com) — resources and information concerning repairability and the Right to Repair movement.
