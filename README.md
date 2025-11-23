#  SkyLink: Custom Wireless UAV Control System (Phase 1)

**Project Status:**  Active / Phase 1 Complete  
**Platform:** Arduino (Prototyping) -> STM32 (Target)  
**Communication:** 2.4GHz RF (NRF24L01)

##  Project Overview
This project aims to design and develop a custom, long-range wireless remote control system for Unmanned Aerial Vehicles (UAVs). Unlike off-the-shelf RC transmitters, "SkyLink" is built from the ground up to provide a modular, open-source, and programmable avionic control architecture.

**Phase 1** focuses on establishing a reliable "Proof of Concept" (PoC) using Arduino microcontrollers to control a high-performance BLDC motor via a custom RF protocol.

##  Key Features (Phase 1)
* **Custom RF Protocol:** Packet-based communication using NRF24L01 PA+LNA modules.
* **Fail-Safe Architecture:** Automatic motor cut-off in case of signal loss or joystick disconnection.
* **Safety Arming Sequence:** Modified PWM signals (700us floor) to ensure safe ESC initialization.
* **Deadzone Logic:** Software filtering to prevent accidental motor spin at neutral joystick positions.
* **Real-time Debugging:** Serial monitoring of Throttle and Aux channels.

##  Hardware Specifications (BOM)

| Component | Model / Specification | Purpose |
| :--- | :--- | :--- |
| **Microcontroller** | 2x Arduino Uno (ATmega328P) | TX (Transmitter) & RX (Receiver) Processing |
| **RF Module** | NRF24L01+ PA+LNA (SMA Antenna) | Long-range 2.4GHz Communication |
| **Motor** | **FPVCYCLE 30mm (1150kV/1420kV)** | High-efficiency propulsion for 7-8" props |
| **ESC** | Skywalker 50A (with UBEC) | Electronic Speed Control & Power Regulation |
| **Battery** | **4S LiPo - 7000mAh (14.8V)** | High-capacity Power Source |
| **Input** | Analog Joystick (2-Axis) | Pilot Control Interface |
| **Capacitors** | 10uF - 100uF Electrolytic | Voltage stabilization for RF modules |

##  Technical Implementation

### 1. RF Communication Strategy
* **Library:** `RF24` optimized for stability.
* **Data Rate:** 1 MBPS (Selected for better sensitivity over 2MBPS).
* **Power Level:** Dynamic adjustment (Low for bench test, Max for field test).
* **Structure:** Custom `struct` data packets containing `throttle_value` (byte) and `aux_value` (byte).

### 2. Motor Control Logic & Safety
The standard `Servo.h` library range (1000us-2000us) was insufficient for the Skywalker ESC to recognize the "Zero Throttle" signal, causing continuous beeping (Safety Mode).
* **Solution:** The PWM output range was re-mapped to **700us - 2000us**.
* **Algorithm:**
    ```cpp
    // Joystick Deadzone Logic
    if (raw_input < 520) {
        throttle_output = 0; // Motor STOP (Safety)
    } else {
        throttle_output = map(raw_input, 520, 1023, 0, 255); // Linear Acceleration
    }
    ```

##  Wiring Diagram (Prototype)

### Receiver (RX) - Vehicle Side
* **ESC Signal (White):** Pin D3 (PWM)
* **ESC GND (Black):** Arduino GND
* **NRF24 CE/CSN:** Pin D9 / D10
* **NRF24 SPI:** D11, D12, D13

### Transmitter (TX) - Remote Side
* **Joystick VRy:** Pin A1
* **Joystick VRx:** Pin A0
* **NRF24 CE/CSN:** Pin D9 / D10

### Bağlantı Şemaları (Wiring Diagrams)

**Receiver:**
![Receiver Schematic](Receiver_Schematic.jpeg)

**Transmitter:**
![Transmitter Schematic](Transmitter_Schematic.jpeg)

##  Roadmap & Future Plans

- [x] **Phase 1:** Basic RF Communication & Motor Drive (Completed) 
- [ ] **Phase 2:** Independent Power Systems (UBEC integration) & Portable TX Design
- [ ] **Phase 3:** Telemetry System (Battery Voltage feedback to Remote)
- [ ] **Phase 4:** Servo Control Integration (For Aileron/Elevator)
- [ ] **Phase 5:** Migration to **STM32 & Altium Designer** Custom PCB

##  Author
**[Faruk Gökoğlu, Selim Süleyman Hüthüt, Engin Çelik]** Electrical & Electronics Engineering Students  
*Focus: Embedded Systems & UAV Avionics*

---