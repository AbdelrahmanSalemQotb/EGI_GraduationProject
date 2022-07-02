
# Ventilator-by-sensing-vital-human-functions-using-IOT #

## System Overview ##
The BVM system uses two Arduino which are Arduino UNO in order to provide continuous airflow while providing user input and WiFi module to push the data over mobile application to easily observe the patient's condition.
The system has a master Arduino, which is responsible for managing the screen and user input, and a slave Arduino that actually sends commands to the servo motors. This guarantees the system's real-time safety and enables the Arduino master to function totally independently of the Arduino slave.
We will highlight the operation of each Arduino and the WiFi module separately to provide further clarification about the technical operation of this device.

The Arduino slave operates in three distinct states to enable the device's different functions: load, on, and off. While in the 'load' state, the Arduino slave will read ventilation parameters from the Arduino master through the I2C bus, generate a new pathway based on these parameters, and swap to the 'on' state.
While in the 'on' state, the device will repeat the pathway permanently.
This means that if the Arduino slave receives a command from the master to change its state to'stop', it will never stop moving the motors.
If the master Arduino does this, the device will complete its current cycle and stop moving.

The Arduino master is mainly responsible for controlling the customer LCD and sending commands to the Arduino slave. The Arduino master controls which panel is displayed and controls input from the encoder and button. Each panel displayed by the master is flexible and configured to behave differently. This makes it simple to create panels that behave differently and communicate with the Arduino slave.

Concerning the sensors part, we here have 2 sensors which are the oximeter sensor (MAX30100) and the temperature sensor (MLX90614) that measure the main 3 parameters that COVID-19 or any chest disease affects, which are *Oxygen Ratio*, *Temperature*, and *Heart Rate*. 

When the patient hands are detected in the IR sensor field, it automatically powers on the two previous sensors. And for the results, we have 2 ways to read them; first on the LCD, or over the wifi application assisted by the wifi module that we talked about previously.

If it senses a critical value from the sensors, an alarm sound starts to announce to anyone near the patient , sends an email and notification message to the patient case observer that there's a critical value  that there's a critical value that needs to be take care of. 

## Block Diagram ##

![Block Diagram](https://github.com/AbdelrahmanSalemQotb/Ventilator-by-sensing-vital-human-functions/blob/Main/Block%20Diagram/Block%20Diagram.png)

## Installation Instructions ##
1. Download the latest Arduino IDE from their website [here](https://www.arduino.cc/en/Main/Software).
2. Download the latest posted release of the device firmware [here](https://github.com/AbdelrahmanSalemQotb/Ventilator-by-sensing-vital-human-functions/releases), it should look something like "EGI_Ventilator.zip".
3. Unzip the downloaded release.
4. Add the libraries to the Arduino IDE:
    1. Copy Files to "C:\Users\\"YourUserName"\Documents\Arduino\libraries"
5. Open the slave, master and NodeMCU_V1.0 code.
    1. In the Arduino IDE, go to File->Open...
    2. Navigate to and click on the slave.ino file inside the extracted release.
    3. Repeat this process for the master.ino and NodeMCU_V1.0.ino file.
6. Flash the Arduino boards with the firmware.
    1. Plug the first Arduino board into your computer.
    2. In the Arduino IDE, go to Tools->Board and select the Arduino board you are using.
    3. Next, in the Arduino IDE, go to Tools->Port and select the port for your Arduino board.
    4. Push the arrow button to upload code on the Arduino IDE window with the slave code.
    5. Unplug the first Arduino, plug in the second, and repeat the above step for the master code.
7. Follow the [wiring diagram](https://github.com/AbdelrahmanSalemQotb/Ventilator-by-sensing-vital-human-functions/blob/Main/Wiring%20Diagram/Wring%20Diagram.png) to complete assembly.
