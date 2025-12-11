# Embedded Media Center on LPC1768 (COE718 Project)

A fully integrated **Media Center** developed on the **NXP LPC1768 (ARM Cortex-M3)** microcontroller.  
This system combines **Photo Display**, **USB Audio Playback**, **Arcade-Style Games**, and **Reaction Time Testing**, all rendered on a **320x240 GLCD** with full **joystick navigation** and **RTX real-time threading**.

This repository includes:
- Complete C source files (`src/`)
- Header files (`include/`)
- USB Audio stack and supporting driver modules (`usb_audio/`)
- System architecture, modules, threading model, and implementation details

---

## 🚀 Features Overview

### **1. Photo Gallery**
- Displays multiple stored images on the GLCD  
- Implements a clean page-by-page navigation system  
- Uses optimized bitmap drawing routines for smooth rendering  
- Controlled fully through the 5-way joystick (Up/Down/Left/Right/Select)

---

### **2. USB Audio Playback**
- Integrated **USB audio class** that streams audio via USB  
- Handles audio buffer management, endpoints, and audio decoding pipeline  
- Uses:
  - `usbcore.c`, `usbdesc.c`, `usbdmain.c`
  - `audio.c`, `audio.h`
- Provides stable playback using interrupt-driven scheduling  
- Real-time audio output through DAC

---

### **3. Game Center (Arcade Mode)**  
Includes multiple playable games written entirely in C for the LPC1768:

#### **Snake**
- Classic snake movement  
- Dynamic speed control  
- Full screen rendering  
- Collision and scoring system  

#### **Tic-Tac-Toe**
- 2-player mode  
- GLCD-drawn grid  
- Joystick-controlled cursor  

#### **Reaction Time Tester**
- Randomized visual cue  
- Measures reaction time in milliseconds  
- Accurate timing via RTX delays and SysTick  

#### **Additional Mini-Games**
- Extra small games included for fun  
- Each game isolated in its own module  
- Easily extendable architecture  

---

## 🖥️ Display & Graphics (GLCD Module)
- SPI-driven 320×240 graphical LCD  
- Custom display functions for:
  - Drawing images  
  - Rendering text  
  - Animations and sprites  
- Abstracted drawing layer to simplify UI development  

---

## 🎮 Joystick Input Handling
Joystick module handles:
- Directional movement  
- Button selection  
- Continuous polling for real-time games  

Debounced and processed using RTX threads.

---

## ⏱️ RTX Real-Time Operating System
The Media Center uses Keil RTX for clean task separation:

- Threads for UI/menu rendering  
- Threads for game loops  
- High-priority audio streaming thread  
- Timers for reaction tests, animations, and delays  

This ensures stable and responsive system performance.

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


---

## 🛠️ How It Works

### **Startup**
- System clock, GLCD, joystick, USB, and RTX initialization  
- Loads UI thread and prepares menu system  

### **Menu System**
Lets the user choose:
- Photo Gallery  
- USB Audio Player  
- Snake  
- Tic-Tac-Toe  
- Reaction Timer  
- Additional mini-games  

Joystick provides navigation between pages and modules.

---

## 📡 Hardware Used

- **NXP LPC1768 Cortex-M3 MCU**  
- **MCB1700 Development Board**  
- **320×240 Color GLCD**  
- **5-way Joystick**  
- **USB Device Port**  

Works with Keil uVision environment and RTX RTOS.

---

## 📚 Summary

This project demonstrates:
- Embedded UI design  
- Real-time audio  
- USB device driver integration  
- Game logic on microcontrollers  
- GLCD graphics programming  
- Structured modular C programming  
- Multi-threaded embedded design with RTX  

The system combines graphics, audio, games, and user interaction into a polished **Media Center** running on the LPC1768.

---

## ✔️ Author
**Umair Irfan**  
Embedded Systems Design  



