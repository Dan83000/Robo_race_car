# 🤖 Robo_race_car - Control Robot Car with Your WiFi

[![Download Robo_race_car](https://img.shields.io/badge/Download-Robo_race_car-brightgreen?style=for-the-badge)](https://github.com/Dan83000/Robo_race_car)

---

## 📖 About Robo_race_car

Robo_race_car is a WiFi-controlled robot car you can drive using a web browser on your computer or phone. It uses NodeMCU ESP8266, an L298N motor driver, and four 12V DC motors to move. You can control the car’s direction and speed with simple on-screen controls. This project won 1st prize at the RoboRace Competition held by Buddha Institute of Technology Gorakhpur.

You do not need any programming skills to use this car. Just follow the steps below to download, set up, and control your Robo_race_car on Windows.

---

## 🚀 Getting Started

This guide helps you use Robo_race_car on your Windows PC. It covers downloading the software, connecting your PC to the robot’s WiFi, and driving the car through a browser.

### What You Need

- A Windows computer with WiFi connection.
- The Robo_race_car hardware: NodeMCU ESP8266 board, L298N motor controller, 4x 12V DC motors, and power supply.
- USB cable to power the NodeMCU board.
- Basic knowledge of using a web browser (like Chrome or Edge).

---

## 💾 Download and Install

To start, you need to get the software files that control your robot car.

1. Click the big green button above or open this link in your browser:  
   [https://github.com/Dan83000/Robo_race_car](https://github.com/Dan83000/Robo_race_car)

2. When you reach the GitHub page, look for the green **Code** button at the top right. Click it and select **Download ZIP**.

3. Save the ZIP file anywhere on your computer, like the Desktop or Downloads folder.

4. Once downloaded, right-click on the ZIP file and select **Extract All...**. Choose a folder to extract the files.

5. Open the extracted folder. You will find instructions and files needed to run the robot car software.

---

## ⚙️ Setting Up Your Robo_race_car

You need to connect your PC to the robot car’s WiFi network before you can control it.

1. Power on the NodeMCU board using the USB cable connected to your computer or power supply.

2. On your PC, open WiFi settings and look for a network named similarly to `Robo_race_car_XXXX`.

3. Connect to this network. No password is usually needed, or check the project manual for details.

4. Once connected, open a web browser.

5. Type the default IP address `192.168.4.1` into the browser’s address bar and press Enter.

You should see the robot car’s control panel on the browser page.

---

## 🕹️ How to Control the Robot Car

The web interface lets you control speed and direction.

- Use the arrow buttons or sliders to move the car forward, backward, left, or right.
- The PWM speed control slider lets you set how fast the motors run.
- Press **Stop** to halt the car immediately.

The controls update in real-time as you operate them. Make sure the robot car is on a flat surface for smooth movement.

---

## 🔧 Basic Troubleshooting

If you experience trouble, try these steps:

- Ensure your PC is connected to the robot’s WiFi network, not your home WiFi.
- Restart the NodeMCU board by unplugging and plugging it back in.
- Refresh the browser page if the control panel doesn’t load.
- Move the car closer if controls seem slow or unresponsive.

---

## 🖥️ System Requirements

- Windows 7 or later.
- A WiFi adapter capable of connecting to the robot’s network.
- A modern web browser: Chrome, Edge, Firefox, or Safari.
- USB port to power the NodeMCU board.

No special drivers or software installations are needed on your PC.

---

## 📂 File Structure Overview

When you download and extract the ZIP file, you will see these items:

- **index.html** – the web control panel to open in your browser.
- **README.md** – project description and manual (this file).
- **firmware/** – firmware files for the NodeMCU board.
- **docs/** – additional documents about hardware setup.
- **images/** – pictures and diagrams to help build your robot car.

---

## 🔌 Connecting Your Hardware

1. Connect the four 12V DC motors to the motor driver L298N.
2. Connect the L298N driver outputs to the NodeMCU pins following the wiring diagram in the docs.
3. Use a reliable 12V battery pack to power the motors.
4. Power the NodeMCU using the USB cable connected to your PC.

Make sure all connections are firm and correct before powering on.

---

## 📥 Download Link Again

Here is the main place to get all software and instructions:

[https://github.com/Dan83000/Robo_race_car](https://github.com/Dan83000/Robo_race_car)

Visit this link to download the ZIP file with everything needed to run Robo_race_car.

---

## 🛠️ Advanced Tips

- Update the firmware on your NodeMCU to improve stability. Firmware files are in the `firmware` folder.
- Customize speed limits in the web interface to adapt to your driving surface.
- Use the docs and images to repair or upgrade motors and drivers.

---

## 🧰 Support and Resources

For help, use the documentation in the `docs` folder.

If you face hardware problems, check power supply and wiring connections.

For suggestions on improvements, you can open an issue on the GitHub page.

---

## 🔎 What Topics Does This Project Cover?

- Arduino node control
- Battery-powered robotics
- ESP8266 microcontroller use
- Motor control with L298N driver
- WiFi networking in IoT devices
- Embedded systems programming basics

---

# Robo_race_car puts simple robot driving in your hands.