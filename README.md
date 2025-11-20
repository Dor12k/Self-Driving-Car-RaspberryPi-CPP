


## Self-Driving Car | Computer Vision & Deep Learning on Embedded Systems 🚗

## 🎥 Demo Video
[![Watch the video](https://github.com/user-attachments/assets/7f4690b1-4221-4d37-945c-dd8631545ef5)](https://youtu.be/1vHhDjWunTw)

<br>

## 🧭 Architecture Diagram
![Project Graph](https://github.com/user-attachments/assets/864ad59d-2d36-4685-a912-daec6416ef65)

<br>

# Overview

This repository contains a complete autonomous driving system built using Raspberry Pi, Arduino UNO, C++/Python, OpenCV, and deep learning models.  
The car can autonomously navigate a road, detect traffic signs and lights, avoid obstacles, and even recognize the end of the road to perform a U-turn.

To maximize learning and maintainability, the project was developed in **two versions**:

---

# 🔀 Code Structure Overview

## **1. Full Windows Simulation (Main Project Code)**
This is the version presented throughout the repository, showing the full system architecture:

- Fully modular structure (multiple classes, clean architecture)
- TensorFlow Lite model for traffic sign detection  
- High-resolution video processing
- GPS simulation with multithreading  
- Designed for clarity, readability, and software engineering best practices  
- Runs on Windows + Linux using C++/Python (Visual Studio, Code::Blocks, PyCharm)

➡ **This is the primary code for demonstrating system design, CV implementation, and overall engineering quality.**

---

## **2. Raspberry Pi Runtime Version**
Located in:  
```
/RaspberryPI/main.cpp
```

The Raspberry Pi + Arduino UNO hardware required a much lighter, real-time optimized version:  

- Single-file implementation for faster runtime
- Uses OpenCV Haar Cascade Classifier instead of TensorFlow Lite  
- Optimized for limited CPU/GPU resources of Raspberry Pi  
- Provides smooth, real-time object detection with minimal frame drops  
- This is the version shown in the demo video running on the actual autonomous car

➡ **This code powers the real robot.**

---

# 🛠️ Technologies Used

**Languages:**  
- C++, Python  

**Libraries & Frameworks:**  
- OpenCV  
- TensorFlow, TensorFlow Lite  
- NumPy  

**Platforms & Environments:**  
- Raspberry Pi OS (Linux)  
- Windows (Visual Studio, Code::Blocks)  
- Python (PyCharm)  

---

# 🚦 Computer Vision & AI Implementation

### **Obstacle Avoidance**
- Real-time object detection and tracking  
- Dynamic steering adjustments to avoid collisions  

### **Traffic Light Detection**
- Color-based classification (red / yellow / green)  
- Real-time decisions: stop, prepare, go  

### **Lane Detection**
- Extracts road boundaries using OpenCV  
- Computes the lane curve and steering correction  
- Aligns the car by comparing camera center vs. lane center  

### **Traffic Sign Detection**
Two separate implementations:
1. **Deep Learning (TensorFlow Lite)** — trained on Kaggle dataset  
2. **Haar Cascade Classifier** — trained using a custom dataset of traffic signs, lights, and obstacles  

---

# 🤖 Autonomous Driving Logic

The movement logic compares:

```
road center  <->  camera center
```

Based on this, the vehicle decides whether to:

- Steer left  
- Steer right  
- Continue forward  
- Stop  
- Perform a U-turn (when road ends)  

Smooth driving required precise timing between:

- Camera capture  
- CV processing  
- Motor control (Arduino UNO + motor driver)  

---

# 🖥️ User Interface & Remote Control

The car also supports **manual driving** using a remote control interface.  
A **GPS simulation**, implemented using multithreading, displays:

- Real-time movement  
- Path history  
- Distance calculations  
- Car orientation (angle)  

This allowed testing the control system on Windows even before deploying to hardware.

---

# 💡 Key Challenges & Learnings

- Synchronizing CV processing speed with motor control for real-time decision making  
- Building and cleaning datasets for ML training  
- Handling variations in lighting and outdoor conditions  
- Migrating code between platforms (Windows ↔ Linux ↔ Raspberry Pi)  
- Re-implementing core logic in a lightweight manner for embedded hardware  

---

# 🎯 Final Outcome

The autonomous car successfully:

- ✅ Follows road lanes  
- ✅ Detects and avoids obstacles  
- ✅ Understands and reacts to traffic lights  
- ✅ Recognizes stop signs and performs correct stopping behavior  
- ✅ Detects the end of the road and performs a U-turn  
- ✅ Runs smoothly on Raspberry Pi in real-time  

This project demonstrates deep understanding of computer vision, embedded optimization, and AI-driven robotics integration.

<br>

---

If you want to explore the Raspberry Pi runtime code used in the real hardware demo, check:

```
/RaspberryPI/main.cpp
```

