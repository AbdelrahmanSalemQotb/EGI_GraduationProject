
# Ventilator-by-sensing-vital-human-functions-using-IOT


# Block Diagram

![Block Diagram](https://github.com/AbdelrahmanSalemQotb/Ventilator-by-sensing-vital-human-functions/blob/Main/Block%20Diagram/Block%20Diagram.png)

# Installation Instructions

1. Download the latest Arduino IDE from their website [here](https://www.arduino.cc/en/software).
2. Download the latest posted release of the device firmware [here](https://github.com/AbdelrahmanSalemQotb/Ventilator-by-sensing-vital-human-functions/releases), it should look something like "EGI_Ventilator.zip".
3. Unzip the downloaded release.
4. Add the libraries to the Arduino IDE:\
       i. Copy Files to to "C:\Users\"YourUserName"\Documents\Arduino\libraries"
5. Open the slave, master and NodeMCU_V1.0 code.\
       i. In the Arduino IDE, go to File->Open...\
      ii. Navigate to and click on the slave.ino file inside the extracted release.\
     iii. Repeat this process for the master.ino and NodeMCU_V1.0.ino file.
6. Flash the Arduino boards with the firmware.\
       i. Plug the first Arduino board into your computer.\
      ii. In the Arduino IDE, go to Tools->Board and select the Arduino board you are using.\
     iii. Next, in the Arduino IDE, go to Tools->Port and select the port for your Arduino board.\
      iv. Push the arrow button to upload code on the Arduino IDE window with the slave code.\
       v. Unplug the first Arduino, plug in the second, and repeat the above step for the master code.
7. Follow the [wiring diagram](https://github.com/AbdelrahmanSalemQotb/Ventilator-by-sensing-vital-human-functions/blob/Main/Wiring%20Diagram/Wring%20Diagram.png) to complete assembly.
