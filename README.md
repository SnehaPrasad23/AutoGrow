# AutoGrow
An automatic plant monitoring system using ESP32 in Wokwi online simulator and TensorFlow Lite.

Simulation link: https://wokwi.com/projects/390857771838240769

The automatic plant monitoring system is designed to optimize plant health by continuously monitoring key environmental parameters and taking action based on the data collected from sensors. 
In this project we have implemented sensors which detect temperature and humidity and the soil moisture, pH level, and nitrogen, phosphorus and potassium levels in the soil (agricultural field). We have implemented the esp32 circuit with attached sensors (pH sensor, temperature sensor, moisture sensor) on Wokwi online simulator that collects the data from sensors and sends it to Blynk interface for display and to Sensor data google sheet for real time data collection. We have used neural network tflite model to predict the ideal amount of soil moisture, ph level and nutritients level needed for healthy growth of the plants with temperature and humidity as input. If the current levels of the soil near the plant  are less than the predicted value, then the DC motor/buzzer/led is switched ON till the required levels are reached.
