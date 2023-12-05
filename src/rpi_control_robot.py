#pip3 install pynput
import serial
import time
from pynput import keyboard
from time import sleep

ser = serial.Serial('/dev/ttyUSB0', 9600)
##ser.reset_input_buffer()

def on_press(key):
    global steer
    try:
        print('alphanumeric key {0} pressed'.format(key.char))
        if key.char == 'w':
            ser.write(b'0')
        elif key.char == 'd':
            ser.write(b'1')
        elif key.char == 'a':
            ser.write(b'2')
        elif key.char == 's':
            ser.write(b'3')
        elif key.char == 'x':
            ser.write(b'4')
        elif key.char == 'backspace':
            ser.write(b'5')
        elif key.char == 'enter':
            ser.write(b'6')

    except AttributeError:
        print('special key {0} pressed'.format(key))

def on_release(key):
    #print('{0} released'.format(key))
    if key == keyboard.Key.esc:
        # Stop listener
        return False

with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()

# ...or, in a non-blocking fashion:
#listener = keyboard.Listener(on_press=on_press, on_release=on_release)
#listener.start()