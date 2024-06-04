# ENGR-103-Final
My final project for ENGR 103

## What is the game? 
The objective of the game is to move your character (White LED) onto the position of a target LED (Yellow LED) in the allotted time. The time starts generous, but every time you successfully make it to the target, the time decreases down to a minimum of 1 second. Once you fail to make it to the target, or reach round 10, the game ends, and your score is displayed on the LED’s. 

## Controls
There are two methods of controls. While in the menu, you can flip the switch. If the LED’s illuminate yellow, that means the board will read the tilt in order to place your character around the ring. If the LED’s illuminate purple, that means the board will use the buttons to move your character clockwise or counter-clockwise around the board. 

## Starting the game
To start the game, when you are ready, press both buttons while in the menu. 

## Technical stuff for TA's (I/O)
The board will use multiple methods of controls. Firstly, are button controls. Using an interrupt on both buttons, you can press them to move the LED clockwise or counter-clockwise around the ring. The second method (accessed by flipping the switch) will use the accelerometer to read the angle of the board and move the player LED to the bottom edge of the board, letting you rotate the board to move the player. You are free to switch between the modes as you see fit in the menu, but cannot change once you start the game. Using a pre-written function to turn the readings from the accelerometer into an angle, I will then use a map function to turn the angle into an LED, as seen below: 
led = map([angle of board], 0, 360, 0, 9);

