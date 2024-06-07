# [PRESENTATION](https://docs.google.com/presentation/d/13XVEOHjXQQfCrEEBm5dKIo68Tm5EoYl8R2iEKvkDad8/edit?usp=sharing)
# [VIDEOS](https://youtube.com/playlist?list=PLtVsCGMnCRNLfM454JxUnA87iWlSrBUsN&si=3DzrlU7Uc-D6-U3D)
# Final Robot Project

The robot's main code operates by a couple switch cases that switch modes according to the IR remote's button pressed. Which toggles through the modes of the robot.

## IR Drive Mode:
The robot responds to the arrow buttons and drives that direction using a "Jog Timer" to allow it to move for a bit while still allowing other inputs, which helps with the inconsistency of the IR remote's signals. The (OK) key stops all movement.

## Ultrasonic Sensor Follow Mode:
Press the 1 key on the remote to switch to the ultrasonic sensor mode, which makes it follow the nearest object until it reaches a short distance away from it. This functionality can be interuppted by changing mode or the stop key (OK).

## Line Following Mode:
Press the 2 key: The Robot reads the contrast of the ground beneath its front and moves according to the darkest color it sees, so on a line mat, it will folow the line.

## IR Sensor Avoidance Mode:
Press the 3 key: This mode takes into account the two sensors on the back of the Robot, and it will drive the opposite direction of any object it detects, this includes left and right movement. This functionality can be interuppted by changing mode or the stop key (OK).

## Scream:
Press the 0 key to make the robot scream out of its buzzer.

# REMOTE HEXCODES
Within the code the hex codes need to be defined as 0x<the hex code> for the comparisons.
| Key  | Hex Code |
|------|----------|
| 1    | FFA25D   |
| 2    | FF62FD   |
| 3    | FFC23D   |
| 4    | FF22DD   |
| 5    | FF02FD   |
| 6    | FFC23D   |
| 7    | FFE01F   |
| 8    | FFA857   |
| 9    | FF906F   |
| *    | FF6897   |
| 0    | FF9867   |
| #    | FFB04F   |
| UP   | FF18E7   |
| LEFT | FF10EF   |
| OK   | FF38C7   |
| RIGHT| FF5AA5   |
| DOWN | FF4AB5   |

