# pip3 install pynput

from pynput import keyboard
from time import sleep


def stop():
    pass


def backward(x, t):
    pass


def forward(x, t):
    pass
    # pwmA.ChangeDutyCycle(x)
    # GPIO.output(In2A, GPIO.HIGH)
    # GPIO.output(In1A, GPIO.LOW)

    # sleep(t)
    # pwmA.ChangeDutyCycle(0)
    # pwmB.ChangeDutyCycle(0)


def turn_left(x, t):
    pass


def turn_right(x, t):
    pass


def on_press(key):
    global steer
    try:
        #print('alphanumeric key {0} pressed'.format(key.char))
        if key.char == 'w':
            forward(100, 0.3)
        elif key.char == 'd':
            turn_left(100, 0.2)
        elif key.char == 'a':
            turn_right(100, 0.2)

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
listener = keyboard.Listener(on_press=on_press, on_release=on_release)
listener.start()
