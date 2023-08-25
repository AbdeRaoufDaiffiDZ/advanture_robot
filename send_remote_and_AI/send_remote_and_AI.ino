
#include <SPI.h>
#include <RH_NRF24.h>

#define joyVert    A0 
#define joyHorz    A1

#define A    2
#define B    3
#define C    4
#define D    5
#define E    6
#define F    7

int big = 2400;
int smal = 1700;
int MAPING = 255;


// Define Joystick Values - Start at 512 (middle position)
int joyposVert = 2032;
int joyposHorz = 2032;

// Singleton instance of the radio driver
//RH_NRF24 nrf24;
RH_NRF24 nrf24(9, 10); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini
int data;

int start = 0;


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
   motorcontrol[7] = digitalRead(E);
  motorcontrol[8] = digitalRead(F);
  
  if(motorcontrol[7]== 0){
        start = 2;
        }
 if(motorcontrol[8] == 0){
        start = 3;}


        if ( start == 2){

   // Read the Joystick X and Y positions
  joyposVert = analogRead(joyVert); 
  joyposHorz = analogRead(joyHorz);

  // Determine if this is a forward or backward motion
  // Do this by reading the Verticle Value
  // Apply results to MotorSpeed and to Direction

  if (joyposHorz  > big)
  {
    // This is Backward
    // Set Motors backward
    motorcontrol[2] = 0;

    //Determine Motor Speeds
    // As we are going backwards we need to reverse readings
    motorcontrol[0] = map(joyposVert, smal, 0, 0, MAPING);
    motorcontrol[1] = map(joyposVert, smal, 0, 0, MAPING);

  }
  else if (joyposHorz < smal)
  {
    // This is Forward
    // Set Motors forward
    motorcontrol[2] = 1;

    //Determine Motor Speeds
    motorcontrol[0] = map(joyposVert, big, 4064, 0, MAPING);
    motorcontrol[1] = map(joyposVert, big, 4064, 0, MAPING); 

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
    if (motorcontrol[0] < 0)  motorcontrol[0] = 0;
    if (motorcontrol[1] > MAPING) motorcontrol[1] = MAPING;

  }
  else if (joyposVert > big)
  {
    // Move Right
    // Map the number to a value of 255 maximum
    joyposHorz = map(joyposHorz, big, 4064, 0, MAPING);
  
    motorcontrol[0] = motorcontrol[0] + joyposHorz;
    motorcontrol[1] = motorcontrol[1] - joyposHorz;

    // Don't exceed range of 0-255 for motor speeds
    if (motorcontrol[0] > MAPING)motorcontrol[0] = MAPING; //////////////////////////////////////////////
    if (motorcontrol[1] < 0)motorcontrol[1] = 0;      

  }

  // Adjust to prevent "buzzing" at very low speed
  if (motorcontrol[0] < 5)motorcontrol[0] = 0;
  if (motorcontrol[1] < 5)motorcontrol[1] = 0;

  //Display the Motor Control values in the serial monitor.
  /*Serial.print("Motor A: ");
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
  motorcontrol[6] = digitalRead(D);*/

  
  
  

  
  
  //Send a message containing Motor Control data to manager_server
  nrf24.send(motorcontrol, sizeof(motorcontrol));
  
  nrf24.waitPacketSent();
        }


     else if ( start  == 3){
     

while (Serial.available()){

 
  data = Serial.read();
   Serial.println(data);
  
}

if (data == '5' )
{
motorcontrol[2] = 0; ///////////  MOVE FRONT 
motorcontrol[0] = 255;
motorcontrol[1] = 255;

  
}

else if (data == '0')
{
  motorcontrol[2] = 1;
motorcontrol[0] = 255;
motorcontrol[1] = 255;

}

else if (data == '2')
{
  motorcontrol[2] = 0;
motorcontrol[0] = 255;
motorcontrol[1] = 0;

}

else if (data == '1')
{
  motorcontrol[2] = 0;
motorcontrol[0] = 0;
motorcontrol[1] = 255;

}
else{
   motorcontrol[2] = 0;
motorcontrol[0] = 0;
motorcontrol[1] = 0;
}

//Send a message containing Motor Control data to manager_server
  nrf24.send(motorcontrol, sizeof(motorcontrol));
  
  nrf24.waitPacketSent();


      
     }
  }
  


/*
 * python code assembly by raouf 
 * hand tracker 
 * sends data to arduino ide
 * arduino transmit by nrf24l01
 * 
 *  
 import cv2
import mediapipe as mp
import serial #Serial imported for Serial communication

import time #Required to use delay functions

ArduinoSerial = serial.Serial('com6',9600) 
time.sleep(2)


mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

# For webcam input:
cap = cv2.VideoCapture(0)
with mp_hands.Hands(
    model_complexity=0,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as hands:
  while cap.isOpened():
    success, image = cap.read()
    if not success:
      print("Ignoring empty camera frame.")
      # If loading a video, use 'break' instead of 'continue'.
      continue

    # To improve performance, optionally mark the image as not writeable to
    # pass by reference.
    image.flags.writeable = False
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image)

    # Draw the hand annotations on the image.
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    # Initially set finger count to 0 for each cap
    fingerCount = 0

    if results.multi_hand_landmarks:

      for hand_landmarks in results.multi_hand_landmarks:
        # Get hand index to check label (left or right)
        handIndex = results.multi_hand_landmarks.index(hand_landmarks)
        handLabel = results.multi_handedness[handIndex].classification[0].label

        # Set variable to keep landmarks positions (x and y)
        handLandmarks = []

        # Fill list with x and y positions of each landmark
        for landmarks in hand_landmarks.landmark:
          handLandmarks.append([landmarks.x, landmarks.y])

        # Test conditions for each finger: Count is increased if finger is 
        #   considered raised.
        # Thumb: TIP x position must be greater or lower than IP x position, 
        #   deppeding on hand label.
        if handLabel == "Left" and handLandmarks[4][0] > handLandmarks[3][0]:
          fingerCount = fingerCount+1
        elif handLabel == "Right" and handLandmarks[4][0] < handLandmarks[3][0]:
          fingerCount = fingerCount+1

        # Other fingers: TIP y position must be lower than PIP y position, 
        #   as image origin is in the upper left corner.
        if handLandmarks[8][1] < handLandmarks[6][1]:       #Index finger
          fingerCount = fingerCount+1
        if handLandmarks[12][1] < handLandmarks[10][1]:     #Middle finger
          fingerCount = fingerCount+1
        if handLandmarks[16][1] < handLandmarks[14][1]:     #Ring finger
          fingerCount = fingerCount+1
        #if handLandmarks[20][1] < handLandmarks[18][1]:     #Pinky
          #fingerCount = fingerCount+1 

        # Draw hand landmarks 
        mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            mp_hands.HAND_CONNECTIONS,
            mp_drawing_styles.get_default_hand_landmarks_style(),
            mp_drawing_styles.get_default_hand_connections_style())

    # Display finger count
    cv2.putText(image, str(fingerCount), (50, 450), cv2.FONT_HERSHEY_SIMPLEX, 3, (255, 0, 0), 10)



    var = fingerCount
    print ("you entered", var) #print the intput for confirmation
    ArduinoSerial.write(var.encode())



    
    # Display image
    cv2.imshow('MediaPipe Hands', image)
    if cv2.waitKey(5) & 0xFF == 27:
      break
cap.release()
 */
  
  
