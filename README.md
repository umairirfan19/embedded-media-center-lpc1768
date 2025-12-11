<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue.svg" />
  <img src="https://img.shields.io/badge/MCU-ARM%20Cortex--M3-green.svg" />
  <img src="https://img.shields.io/badge/Board-LPC1768-orange.svg" />
  <img src="https://img.shields.io/badge/RTOS-Keil%20RTX-yellow.svg" />
  <img src="https://img.shields.io/badge/Status-Stable-brightgreen.svg" />
</p>

# Embedded Media Center on LPC1768 (COE718 Project)

A full-featured **Media Center** built on the **NXP LPC1768 (ARM Cortex-M3)** microcontroller.  
The system integrates **photo viewing**, **USB audio playback**, and a set of **interactive games**, all running through a clean menu interface rendered on a **320×240 GLCD**.  
Navigation is handled entirely via a **5-way joystick**, and real-time performance is coordinated with **Keil RTX**.

This project demonstrates:
- Embedded UI development  
- Real-time audio streaming  
- USB device driver integration  
- Game programming on microcontrollers  
- GLCD graphics  
- Structured, modular C design  
- Multi-threaded embedded scheduling (RTX)

---

## 🚀 Features Overview

### **1. Photo Gallery**
- Displays stored images on the GLCD  
- Smooth GUI navigation using joystick inputs  
- Efficient screen drawing routines for clean transitions  
- Fully modular — easy to add more images or categories  

---

### **2. USB Audio Playback**
- Implements USB Audio Class streaming  
- Handles endpoint setup, buffering, descriptors, and playback pipeline  
- Uses modules such as:
  - `usbcore.c`, `usbdesc.c`, `usbdmain.c`
  - `audio.c`, `audio.h`, `usb.h`, `type.h`
- Provides stable, uninterrupted audio output using DAC  
- High-priority RTX threading to prevent audio glitches  

---

## 🎮 Game Center (Arcade Mode)

A dedicated menu section that provides multiple interactive games written for the LPC1768 and optimized for GLCD rendering and joystick control.

### **Snake**
- Fully implemented classic Snake game  
- Smooth 4-direction movement controlled by the joystick  
- Dynamic length increase as food is collected  
- Collision detection with walls and the snake body  
- Real-time game loop powered by RTX for consistent speed  
- Clean, pixel-based rendering on the GLCD grid  

### **Tic-Tac-Toe**
- 2-player mode playable with only the joystick  
- GLCD renders a clean 3×3 board  
- Cursor-based tile selection system  
- Prevents overwriting occupied tiles  
- Win-detection logic for rows, columns, and diagonals  
- Fully modularized so additional board-based games can be added  

### **Reaction Time Tester**
- Randomized trigger signal  
- Measures user response time in milliseconds  
- Uses SysTick + RTX timing functions for accuracy  

### **Additional Mini-Games**
- Extra lightweight games implemented as simple `.c`/`.h` modules  
- Easily extendable architecture  
- Integrated into Game Center menu system  

---

## 🖥 Display & Graphics (GLCD Module)
- SPI-driven 320×240 color display  
- Custom drawing utilities:
  - Images  
  - Text  
  - Borders / shapes  
  - Game elements  
- Optimized to minimize flicker and maximize frame rate  
- Provides an abstraction layer for UI rendering

---

## 🎮 Joystick Input Handling
- Handles:
  - Up / Down / Left / Right  
  - Select  
- Debounced polling via RTX threads  
- Responsive input for real-time games  
- Integrated with UI navigation and gameplay loops  

---

## ⏱ RTX Real-Time Operating System
The project uses RTX to organize tasks into clean, predictable threads:

- **UI/Menu Thread** — rendering and navigation  
- **Audio Thread** — continuous USB audio streaming  
- **Game Threads** — logic and display updates  
- **Input Thread** — joystick polling  
- **Timer Callbacks** — delays, animation pacing, reaction measurement  

This architecture ensures:
- Smooth graphics  
- Reliable USB audio  
- Non-blocking input  
- Stable multi-feature performance  

---

## 🧱 Project Structure

```text
embedded-media-center-lpc1768/
│
├── src/                     # All .c source files (main program, games, gallery, etc.)
│   ├── main.c
│   ├── PHOTO_GALLERY.c
│   ├── snake.c
│   ├── tic_tac_toe.c
│   ├── reaction_time.c
│   ├── LED.c
│   ├── ADC.c
│   └── ... (other modules)
│
├── include/                 # Header files for modules
│   ├── snake.h
│   ├── tic_tac_toe.h
│   ├── KBD.h
│   ├── GLCD.h
│   ├── LED.h
│   └── ... (other headers)
│
├── usb_audio/               # USB Audio driver stack + helper modules
│   ├── audio.c
│   ├── audio.h
│   ├── usb.h
│   ├── usbcore.c
│   ├── usbdesc.c
│   ├── usbdmain.c
│   └── type.h
│
└── README.md

© 2025 Umair Irfan. All rights reserved.

This project is provided publicly for viewing purposes only.
No part of this repository may be copied, reproduced, modified, or used
for any academic, commercial, or personal project without explicit permission.

