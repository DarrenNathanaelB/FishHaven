<br />
<div align="center">
  <h1 align="center">FishHaven: Smart IoT Monitoring System for Fish Farming</h1>
</div>

# I. Introduction to the problem and the solution
<div align="justify">
FishHaven is a Smart IoT Monitoring System for Fish Farming designed to optimize aquaculture management through real-time monitoring and automation. It utilizes a Water Level Sensor Universal to track water height, a Dallas Temperature Sensor (DS18B20) for temperature measurement, and a Turbidity Sensor Module (AB147) to detect water clarity. These sensors transmit data to the Blynk application via Wi-Fi, enabling fish farmers to monitor critical water parameters remotely. Instant notifications are sent when parameters deviate from safe levels, such as high turbidity or unstable temperatures, allowing for prompt corrective actions. FishHaven ensures optimal water quality, reduces risks, and enhances productivity, making fish farming more efficient and sustainable.
<div>

## Contributors

This is a final Internet of Things project made by Group 23:

- [Louis Benedict Archie](https://www.github.com/benedictlouis) - 2206025224
- [Muhammad Daffa Rizkyandri](https://www.github.com/akrabDafriz) - 2206829194
- [Muhammad Abrisam Cahyo Juhartono](https://www.github.com/AbrisamYuhartono) - 2206026050
- [Darren Nathanael Boentara](https://github.com/DarrenNathanaelB) - 2206059490

## Features
- **Real-time Sensor Monitoring:** Continuously track water temperature, depth, and clarity using integrated sensors.

- **Smart Notifications:** Receive instant alerts via the Blynk app when sensor values exceed safe thresholds, ensuring quick responses to critical changes.

- **Remote Control:** Control the system remotely, including toggling the indicator LED, through the user-friendly Blynk app.

- **Wi-Fi Connectivity:** Seamlessly connect the ESP32 microcontroller to the internet for efficient data transmission and remote access.

- **Multitasking with FreeRTOS:** Efficiently manage multiple sensor readings using task scheduling and queues for reliable performance.

- **Data Synchronization:** Synchronize sensor data using mutexes and semaphores to ensure accurate and consistent readings.

- **Scalable Architecture:** Designed with modularity to add more sensors or features as needed for advanced aquaculture applications.

---

# II. Hardware design and implementation details

## Schematic

<img width="7634" alt="Diagram & Goals " src="https://github.com/DarrenNathanaelB/FishHaven/blob/main/Images/Diagram-Goals.png?raw=true">

This Project is built using the following technologies:

- **ESP32 Microcontroller:** The core of the system, handling data from sensors and enabling connectivity with the Blynk application.

- **Blynk App:** A user-friendly platform for monitoring sensor data, receiving notifications, and managing the system remotely.

- **Water Level Sensor Universal:** Measures water levels with high accuracy, helping maintain optimal conditions for fish farming.

- **Dallas Temperature Sensor (DS18B20):** Precisely monitors water temperature to ensure a stable and suitable environment for fish growth.

- **Turbidity Sensor Module (AB147):** Detects water clarity to prevent issues caused by poor water quality, safeguarding the health of aquatic life.

The ESP32 Microcontroller acts as the core of the FishHaven system, seamlessly integrating multiple sensors to provide real-time monitoring of aquaculture conditions. This central unit processes data efficiently and communicates it to the Blynk app, allowing fish farmers to oversee and manage their operations remotely. The Water Level Sensor Universal ensures stability by accurately measuring water levels, helping to maintain a consistent environment. The Dallas Temperature Sensor (DS18B20) monitors water temperature with precision, ensuring optimal conditions for fish health and growth.

Additionally, the Turbidity Sensor Module (AB147) evaluates water clarity, enabling timely interventions to maintain a clean and safe habitat. Together, these technologies form a cohesive system that simplifies fish farming through automation, real-time insights, and smart notifications, fostering sustainable and efficient aquaculture practices.

---

# III. Software Implementation Details

## Diagram-Goals

<img width="7634" alt="Diagram & Goals " src="https://github.com/DarrenNathanaelB/FishHaven/blob/main/Images/Diagram-Goals.png?raw=true">

The FishHaven project integrates cutting-edge hardware and software to create a comprehensive smart aquaculture management system. At its core, the ESP32 microcontroller processes data from various sensors and facilitates seamless communication with the Blynk app, which enables users to monitor and control the system remotely. Upon powering up, the system establishes a Wi-Fi connection and connects to the Blynk server using the necessary authentication token. Once connected, it allows users to view real-time data and manage aquaculture operations through an intuitive mobile interface.

The system uses a range of sensors, each tasked with monitoring critical aspects of the aquaculture environment. The Dallas Temperature Sensor (DS18B20) plays a crucial role by ensuring precise monitoring of water temperature. Maintaining an optimal temperature is vital for the health and growth of the fish, and this sensor continuously measures water temperature, providing real-time updates. The Turbidity Sensor Module (AB147) is another key component, monitoring the clarity of the water. It detects changes in water quality, which can indicate issues such as contamination, algae blooms, or waste buildup, helping prevent harm to the aquatic life. Additionally, the Water Level Sensor Universal ensures stable water levels, allowing the system to track and maintain the ideal volume for fish farming.

Through the integration with the Blynk app, the FishHaven system offers enhanced interactivity. Users can set thresholds for each sensor, receiving notifications whenever conditions fall outside the desired range. This real-time monitoring provides immediate alerts, enabling swift corrective actions to maintain a stable and healthy environment for the fish. Virtual Pins in the Blynk app provide remote control, allowing users to activate specific features or adjust settings based on real-time conditions. Overall, the FishHaven project is designed to simplify aquaculture management, offering farmers a reliable and automated solution to optimize fish farming operations while promoting sustainability and efficiency.

---

# IV. Test Results and Performance Evaluation

During the testing phase of FishHaven, the primary focus was on establishing reliable connections with four essential sensors: Water Level, Temperature, and Turbidity. These sensors were integrated with the ESP32 microcontroller, and their data outputs were closely monitored through the Serial Monitor. This process verified the accuracy of the sensor readings, ensuring they corresponded to real environmental conditions.

The next phase of testing involved assessing Wi-Fi connectivity and Blynk integration. This included a thorough evaluation of the Wi-Fi connection, with particular attention to stability and signal strength indicators. Simultaneously, the Blynk platform's integration was tested to ensure smooth interaction for remote monitoring and control. Success was measured through the functionality of the Blynk app interface and verification of log data, captured through screenshots and visuals. These results confirmed that the system could reliably transmit sensor data and deliver a user-friendly experience, establishing a solid foundation for further development.

However, during testing, an issue was identified with the temperature sensor (DS18B20). The sensor failed to detect water temperature correctly, consistently returning a reading of -127°C. This discrepancy suggests a possible defect in the sensor hardware. However, simulations conducted using the Wokwi platform indicated that the sensor was functioning as expected, pointing to a potential hardware malfunction in the physical setup. This finding highlights the importance of rigorous testing in identifying and resolving issues to ensure the system’s reliability and performance. This issue has been resolved, though sometimes it will return to only displaying -127°C.

## Serial Monitor

<img height="300" alt="Serial Monitor" src="https://github.com/DarrenNathanaelB/FishHaven/blob/main/Images/Serial%20Monitor.jpg?raw=true">

## Blynk

<img height="500" alt="Display Blynk" src="https://github.com/DarrenNathanaelB/FishHaven/blob/main/Images/Blynk%20Interface.jpg?raw=true">
<img height="500" alt="Display Blynk" src="https://github.com/DarrenNathanaelB/FishHaven/blob/main/Images/Blynk%20Notification.png?raw=true">

## Circuit

<img height="500" alt="Diagram Circuit" src="https://github.com/DarrenNathanaelB/FishHaven/blob/main/Images/Circuit.jpg?raw=true">

---

# V. Conclusion and Future Work

 The FishHaven project has successfully laid the groundwork for a smart, IoT-driven aquaculture management system. By integrating the ESP32 microcontroller with essential sensors for water level, temperature, and turbidity, the system effectively delivers real-time monitoring of critical parameters. This data is transmitted to users through the Blynk platform, enabling remote management and immediate insights into the aquatic environment. The testing phase demonstrated the system’s ability to establish stable Wi-Fi connectivity, maintain seamless communication with the Blynk application, and provide user-friendly access to sensor data. These achievements highlight the system’s practicality and reliability for modern fish farming needs.

There were failures, particularly with the DS18B20 temperature sensor, which consistently failed to provide accurate readings. Simulated tests using Wokwi revealed that the issue was hardware-specific, as the sensor performed correctly in virtual environments. This learning will guide future iterations of the project, ensuring higher reliability and robustness in hardware integration.

For future work the next phase of FishHaven’s development will focus on enhancing its functionality through the introduction of an automated feeding system. This feature will utilize a servo-driven mechanism or similar technology to dispense precise amounts of feed at scheduled intervals. By automating this critical aspect of fish farming, the system aims to reduce manual labor, optimize feeding efficiency, and promote healthier aquatic life. Additional improvements, such as refining sensor calibration, expanding remote control capabilities, and integrating more advanced analytics, will further elevate the system’s utility.
