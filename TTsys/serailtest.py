import serial
import time

arduino = serial.Serial(port = 'COM7', timeout=0)

while True:
    cmd = input()
    print ("You Entered :", cmd)

    if(cmd == 'i'):
        arduino.write(str.encode('i'))
        print("initialized x axis")
    if(cmd == 'j'):
        arduino.write(str.encode('j'))
        print("initialized y axis")
    if(cmd[0] == 'x'):
        arduino.write(str.encode(cmd))
        print(cmd)
    if(cmd[0] == 'y'):
        arduino.write(str.encode(cmd))
        print(cmd)     
    if(cmd == 'q'):
        break