

## Self-Driving Car with Computer Vision & Deep Learning 🚗



### **Overview**

This project is a fully functional self-driving car built using Raspberry Pi, Arduino UNO, and C++/Python for computer vision and deep learning. <br>
The car can autonomously navigate a road, detect and respond to traffic signs, traffic lights, and obstacles, and even perform maneuvers like U-turns when reaching the end of the road. <br>

To maximize learning, I developed the project in multiple versions, implementing it in C++ on Linux and Windows (using Visual Studio and Code::Blocks) and in Python on both platforms. <br>
This provided deep experience with computer vision libraries and frameworks across different environments.

🔧 Technologies Used

    Languages: C++, Python 
    Libraries & Frameworks: OpenCV, TensorFlow, TensorFlow Lite, Numpy 
    Development Environments: Linux, Windows (Visual Studio, Code::Blocks, PyCharm) 

<br>

🚦 Computer Vision & AI Implementation

 - **Obstacle Avoidance:** Used object detection and tracking to identify and navigate around obstacles.
 - **Traffic Light Detection:** Used object detection & color recognition to classify red, yellow, and green lights and make real-time driving decisions.
 - **Lane Detection:** Used camera input to detect road boundaries and determine steering direction by comparing the road’s center to the camera’s position. 
 - **Traffic Sign Detection:** Trained a deep learning model with TensorFlow using a Kaggle dataset for recognizing traffic signs. Also implemented an OpenCV Cascade Classifier, where I manually created a dataset of                                  traffic signs, traffic lights, and obstacles for training. 


<br>


🤖 Autonomous Driving Logic

The car determines movement by comparing the camera’s center with the road’s center, deciding whether to steer left, right, or forward based on real-time image processing. <br>
Precise synchronization between the camera, vision processing, and motor control was critical to ensuring smooth driving and avoiding crashes. <br>


<br>


🖥️ User Interface & Remote Control

The car supports manual driving via remote control. <br>
A GPS simulation, implemented using Multithreading library, visualizes the car’s movement, tracks its path, calculates distance traveled, and monitors the car’s angle dynamically. <br>


<br>

💡 Key Challenges & Learnings

 - Synchronizing image processing speed with the car’s electronic response time to ensure accurate real-time decision-making. 
 - Creating a high-quality dataset for training AI models, considering real-world factors like lighting conditions affecting accuracy. 
 - Implementing computer vision algorithms across C++, Python, Linux, and Windows, gaining hands-on experience with cross-platform AI development. 



<br>

🎯 Final Outcome

The car successfully drives autonomously, following the road while detecting and obeying traffic signals: <br> <br>
✅ Avoids obstacles and continues driving.  <br>
✅ Detects and responds to traffic lights (red, yellow, green).  <br>
✅ Recognizes stop signs and stops at the correct distance. <br>
✅ Identifies the end of the road and makes a U-turn. <br>

This project was a fascinating deep dive into computer vision, deep learning, and real-time AI-driven decision-making, pushing the boundaries of both hardware and software integration. <br>

<br>

Preview:

<br>
