// nrf24_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_NRF24 class. RH_NRF24 class does not provide for addressing or
// reliability, so you should only use RH_NRF24 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example nrf24_server.
// Tested on Uno with Sparkfun NRF25L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <SPI.h>
#include <RH_NRF24.h>

#define joyVert A0
#define joyHorz A1

#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7

int big = 2400;
int smal = 1700;
int MAPING = 300;

// Define Joystick Values - Start at 512 (middle position)
int joyposVert = 2032;
int joyposHorz = 2032;

// Singleton instance of the radio driver
// RH_NRF24 nrf24;
RH_NRF24 nrf24(9, 10); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

void loop()
{
  Serial.println("Sending to nrf24_server");
  // Send a message to nrf24_server
  uint8_t motorcontrol[9];

  // Read the Joystick X and Y positions
  joyposVert = analogRead(joyVert);
  joyposHorz = analogRead(joyHorz);

  // Determine if this is a forward or backward motion
  // Do this by reading the Verticle Value
  // Apply results to MotorSpeed and to Direction

  if (joyposHorz > big)
  {
    // This is Backward
    // Set Motors backward
    motorcontrol[2] = 0;

    // Determine Motor Speeds
    //  As we are going backwards we need to reverse readings
    motorcontrol[0] = map(joyposVert, smal, 0, 0, MAPING);
    motorcontrol[1] = map(joyposVert, smal, 0, 0, MAPING);

    if (motorcontrol[0] > MAPING)
    {
      motorcontrol[0] = MAPING;
    }
    else
      motorcontrol[0] = motorcontrol[0];

    if (motorcontrol[1] > MAPING)
    {
      motorcontrol[1] = MAPING;
    }
    else
      motorcontrol[1] = motorcontrol[1];
  }
  else if (joyposHorz < smal)
  {
    // This is Forward
    // Set Motors forward
    motorcontrol[2] = 1;

    // Determine Motor Speeds
    motorcontrol[0] = map(joyposVert, big, 4064, 0, MAPING);
    motorcontrol[1] = map(joyposVert, big, 4064, 0, MAPING);

    if (motorcontrol[0] > MAPING)
    {
      motorcontrol[0] = MAPING;
    }
    else
      motorcontrol[0] = motorcontrol[0];

    if (motorcontrol[1] > MAPING)
    {
      motorcontrol[1] = MAPING;
    }
    else
      motorcontrol[1] = motorcontrol[1];
  }
  else
  {
    // This is Stopped
    motorcontrol[0] = 0;
    motorcontrol[1] = 0;
    motorcontrol[2] = 0;
  }

  // Now do the steering
  // The Horizontal position will "weigh" the motor speed
  // Values for each motor

  if (joyposVert < smal)
  {

    // Move Left
    // As we are going left we need to reverse readings
    // Map the number to a value of 255 maximum
    joyposHorz = map(joyposHorz, smal, 0, 0, MAPING);

    motorcontrol[0] = motorcontrol[0] - joyposHorz;
    motorcontrol[1] = motorcontrol[1] + joyposHorz;

    // Don't exceed range of 0-255 for motor speeds
    if (motorcontrol[0] < 0)
      motorcontrol[0] = 0;
    else
      motorcontrol[0] = motorcontrol[0];

    if (motorcontrol[1] > MAPING)
      motorcontrol[1] = MAPING;
    else
      motorcontrol[1] = motorcontrol[1];
  }
  else if (joyposVert > big)
  {
    // Move Right
    // Map the number to a value of 255 maximum
    joyposHorz = map(joyposHorz, big, 4064, 0, MAPING);

    motorcontrol[0] = motorcontrol[0] + joyposHorz;
    motorcontrol[1] = motorcontrol[1] - joyposHorz;

    // Don't exceed range of 0-255 for motor speeds
    if (motorcontrol[0] > MAPING)
      motorcontrol[0] = MAPING;
    else
      motorcontrol[0] = motorcontrol[0];
    //////////////////////////////////////////////
    if (motorcontrol[1] < 0)
      motorcontrol[1] = 0;
    else
      motorcontrol[1] = motorcontrol[1];
  }

  // Adjust to prevent "buzzing" at very low speed
  if (motorcontrol[0] < 20)
    motorcontrol[0] = 0;
  else
    motorcontrol[0] = motorcontrol[0];

  if (motorcontrol[1] < 20)
    motorcontrol[1] = 20;
  else
    motorcontrol[1] = motorcontrol[1];

  // Display the Motor Control values in the serial monitor.
  Serial.print("Motor A: ");
  Serial.print(motorcontrol[0]);
  Serial.print(" - Motor B: ");
  Serial.print(motorcontrol[1]);
  Serial.print(" - Direction: ");
  Serial.print(motorcontrol[2]);

  Serial.print(" -A:  ");
  Serial.print(motorcontrol[3]);

  Serial.println("  - b:  ");
  Serial.print(motorcontrol[4]);

  motorcontrol[3] = digitalRead(A);
  motorcontrol[4] = digitalRead(B);

  motorcontrol[5] = digitalRead(C);
  motorcontrol[6] = digitalRead(D);

  motorcontrol[7] = digitalRead(E);
  motorcontrol[8] = digitalRead(F);

  // Send a message containing Motor Control data to manager_server
  nrf24.send(motorcontrol, sizeof(motorcontrol));

  nrf24.waitPacketSent();
}
