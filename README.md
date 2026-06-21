Filament Winding Machine — 3-Axis Stepper Control

Open-source firmware and documentation for a 3-motor filament winding machine built around an Arduino Mega 2560 and three closed-loop / open-loop stepper drivers (PUL / DIR / ENA interface). The machine coordinates three independent axes to wind continuous filament (or fiber tow) onto a rotating mandrel with a controlled, reciprocating delivery eye.

This repository holds the controller code used in the VinUniversity Center for Materials Innovation and Technology fiber-fabrication work, where coordinated multi-motor motion is needed to lay fiber/filament at a controlled angle.


What the machine does

A filament winding machine needs at least three coordinated motions:

AxisRole on the machineMotion profileMotor 1Carriage / delivery-eye traverseReciprocating: runs N revolutions, reverses, repeats. Can be homed.Motor 2Mandrel / spindle rotationContinuous rotation (sets winding speed).Motor 3Filament feed / tensionerSlow continuous feed, speed-trimmed.

Three potentiometers give live, hands-on speed control of each axis, and a serial command line lets the operator STOP-and-HOME or RUN the whole machine.


Repository layout

filament-winding-machine/
├── README.md                         ← this file
├── LICENSE
├── firmware/
│   ├── winding_main/                 ← MAIN program (3 motors + pots + home)
│   │   └── winding_main.ino
│   ├── motor1_traverse_2rev/         ← bring-up: 2-motor reverse-every-2-rev test
│   │   └── motor1_traverse_2rev.ino
│   └── motor1_serial_jog/            ← bring-up: single-motor F/B/S/R/M jog
│       └── motor1_serial_jog.ino
├── hardware/
│   └── wiring.md                     ← pin map and driver wiring
└── docs/
    ├── getting_started.md
    ├── serial_commands.md
    ├── tuning.md
    └── safety.md


Quick start


Wire the three drivers to the Arduino Mega per hardware/wiring.md.
Open firmware/winding_main/winding_main.ino in the Arduino IDE.
Select Board → Arduino Mega 2560, pick the serial port, and upload.
Open Serial Monitor at 9600 baud, line ending Newline.
Use the potentiometers to trim each axis speed; send S to stop+home Motor 1, R to resume.


See docs/getting_started.md for the full procedure.


Firmware versions


winding_main — production sketch. Drives all three axes, reads three pots with smoothing + dead-zone + speed limits, and supports STOP/HOME (S) and RUN (R).
motor1_traverse_2rev — early two-motor bring-up: Motor 1 reverses every 2 revolutions, Motor 2 runs continuously. Useful for verifying traverse + spindle wiring.
motor1_serial_jog — single-axis test with full serial jog control (F/B/S/R/Mxxx). Useful for checking one driver and tuning RPM.
