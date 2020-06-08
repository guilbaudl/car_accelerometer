# car_accelerometer

This project works with an Arduino card and an analog accelerometer. The aim of this work was to create a little device allowing to reduce your oil consumption when you are driving, by using a 9-LED-cross, showing the acceleration on a X-axis and a Y-axis. If you want to reduce your consumption, you have to bright the least amount of time possible the LEDs.

This is not only a mesure of the acceleration at the moment and turn on LEDs in function of the intensity of the acceleration vector. This file creates an orthonormal basis when you start the device, this basis making possible for your device to work everywhere you put the device. For example, if you want to fix the device around your steering wheel in order to keep an eye on the device, surfaces are not flat. So you can put the device on the dashboard, turn it on and wait during the initialization, it keeps just few seconds.

Concretely, LEDs will only show you accelerations from your driving (left, right, front and back accelerations, for turns, brake and speeding up)
