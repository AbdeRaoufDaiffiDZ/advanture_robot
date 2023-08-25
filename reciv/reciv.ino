// nrf24_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_NRF24 class. RH_NRF24 class does not provide for addressing or
// reliability, so you should only use RH_NRF24  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example nrf24_client
// Tested on Uno with Sparkfun NRF25L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <SPI.h>
#include <RH_NRF24.h>
#include <Servo.h>
int start = 0;

Servo servo;
Servo servo1;
int servo_pin = 6;
int servo_pin1 = 7;

int up_value = 0;
int down_value = 0;

int griper = 0;

// Define addresses for radio channels
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Motor A Connections
int enA = A0;
int in1 = 5;
int in2 = 3;

// Motor B Connections
int enB = A1;
int in3 = 2;
int in4 = 4;

int maxspeed = 300;

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

  servo.attach(servo_pin);
  servo1.attach(servo_pin1);

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop()
{
  if (nrf24.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
      if (buf[7] == 0)
      {
        start = 0;
      }

      if (buf[8] == 0)
      {
        start = 1;
      }

      if (start == 0)
      {
        Serial.print("start= ");
        Serial.print(start);

        if (buf[1] > maxspeed)
          buf[1] = maxspeed;
        if (buf[0] > maxspeed)
          buf[0] = maxspeed;

        // Set Motor Direction
        if (buf[2] == 0)
        {
          // Motors are backwards

          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);

          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);

          // Drive Motors
          analogWrite(enA, buf[1]);
          analogWrite(enB, buf[0]); /////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else
        {
          // Motors are forwards

          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);

          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);

          // Drive Motors

          analogWrite(enA, buf[1]);
          analogWrite(enB, buf[0]); /////////////////////////////////////////////////////////////////////////////////////////////////////
        }

        Serial.print(": MotorA = ");
        Serial.print(buf[0]);
        Serial.print(" MotorB = ");
        Serial.print(buf[1]);
        Serial.print(" Dir = ");
        Serial.print(buf[2]);
        Serial.print(" down = ");
        Serial.println(buf[6]);

        if (buf[3] == 0)
        {

          servo.write(70);
        }

        if (buf[5] == 0)
        {
          servo.write(125);
        }

        if (buf[4] == 0)
        {

          servo1.write(20);
        }
        if (buf[6] == 0)
        {

          servo1.write(150);
        }
      }
      else
      {

        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);

        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);

        // Drive Motors

        analogWrite(enA, 0);
        analogWrite(enB, 0);

        Serial.print("start= ");
        Serial.println(start);
      }
    }
  }
}
