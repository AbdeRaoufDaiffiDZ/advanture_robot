from ast import Pass
from asyncore import dispatcher_with_send
from base64 import encode
import cv2
import mediapipe as mp
import serial #Serial imported for Serial communication
import os
import time #Required to use delay functions

ArduinoSerial = serial.Serial('com6',9600) 



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
    fingerCount = 'O'
    
    
  

    if results.multi_hand_landmarks:

      for hand_landmarks in results.multi_hand_landmarks:
        # Get hand index to check label (left or right)
        handIndex = results.multi_hand_landmarks.index(hand_landmarks)
        handLabel = results.multi_handedness[handIndex].classification[0].label

        # Set variable to keep landmarks positions (x and y)
        handLandmarks = []

        # Fill list with x and y positions of each landmark
        #############################################UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
        
        
        

        
        height, width = image.shape[:2]
        
        start_point = (int(width/2) - 90,0)
  
        # Ending coordinate, here (220, 220)
        # represents the bottom right corner of rectangle
        end_point = (int(width/2) + 110, int(height/2) - 30 )
  
        color = (255, 255, 0)
  
        # Line thickness of 2 px
        thickness = 2
        image = cv2.rectangle(image, start_point, end_point, color, thickness)
        
        
        ######################################################################DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
        
        
        start_point = (int(width/2) - 90,int(height/2) - 30)
  
        # Ending coordinate, here (220, 220)
        # represents the bottom right corner of rectangle
        end_point = (int(width/2) + 110, height)
  
        color = (255, 255, 0)
  
        # Line thickness of 2 px
        thickness = 2
        image = cv2.rectangle(image, start_point, end_point, color, thickness)
        
        
        
        #######################################################################################################LLLLLLLLLLLLLL
        
        start_point = (0,0)
  
        # Ending coordinate, here (220, 220)
        # represents the bottom right corner of rectangle
        end_point = (int(width/2) - 90, height)
  
        color = (255, 255, 0)
  
        # Line thickness of 2 px
        thickness = 2
        image = cv2.rectangle(image, start_point, end_point, color, thickness)
        
        
        
        
        #######################################################################################################RRRRRRRRRRRRRRRR
        
        
        
        
        
        start_point = (int(width/2) + 110,0)
  
        # Ending coordinate, here (220, 220)
        # represents the bottom right corner of rectangle
        
        end_point = (width, height)
  
        color = (255, 255, 0)
  
        # Line thickness of 2 px
        thickness = 2
        image = cv2.rectangle(image, start_point, end_point, color, thickness)
        
        
        
        
        #######################################################################################################
                     
        for landmarks in hand_landmarks.landmark:
          handLandmarks.append([landmarks.x, landmarks.y])




        if handLandmarks[8][1] < handLandmarks[6][1]:       #Index finger
          
          
          if (handLandmarks[8][0] < 0.36):
                fingerCount = 'R'
          if (handLandmarks[8][0] > 0.36 and handLandmarks[8][0] < 0.68):
              if(handLandmarks[8][1] < 0.4):
                fingerCount = 'F'
              else:
                fingerCount = 'B'    
              
          if (handLandmarks[8][0] > 0.68):
              fingerCount = 'L'
              
              
              
         
        if handLandmarks[20][1] < handLandmarks[17][1]:      
          #print(handLandmarks[20][0]) 
          if (handLandmarks[20][0] < 0.68):
                #pass
                fingerCount='c'
          if (handLandmarks[20][0] > 0.36):
             if(handLandmarks[20][1] < 0.4):
                #pass
                fingerCount ='u'
             else:
                #pass
                fingerCount ='d' 

          if (handLandmarks[20][0] > 0.68):
              #  pass
              fingerCount   ='o' 
         
         
        
              
              
     
        
        mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            mp_hands.HAND_CONNECTIONS,
            mp_drawing_styles.get_default_hand_landmarks_style(),
            mp_drawing_styles.get_default_hand_connections_style())
    else:
      fingerCount = 'O'
      
    

    image = cv2.flip(image, 1)
  
    cv2.putText(image, str(fingerCount), (50, 450), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 5)
    

    var = fingerCount
    
    ArduinoSerial.write(var.encode())
    
    
    # print (ArduinoSerial.read())
    
    
    # Display image
    cv2.imshow('MediaPipe Hands', image)
    if cv2.waitKey(5) & 0xFF == 27:
      break
cap.release()