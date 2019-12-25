# Adapted from the 0amazing Adafruit work
# https://learn.adafruit.com/circuitpython-essentials/circuitpython-digital-in-out
# Circuit Python
# Hardware: Adafruit Trinket M0
# mac connect - screen /dev/tty.usb*
# Connections: D0=Neopixels, D1=Up button, D2=Down button, D3=sound module
#  (ground to activate)

import time
import board
import neopixel
import random
from digitalio import DigitalInOut, Direction, Pull, DriveMode

# Variables
randfloor = 0
floortime = 0
randdirection = 0
up_call_state = 0  # 0 is no call and 1 is called
down_call_state = 0   # 0 is no call and 1 is called

# initialize switch pins
upswitch = DigitalInOut(board.D1)
downswitch = DigitalInOut(board.D2)
downswitch.direction = Direction.INPUT
downswitch.pull = Pull.UP
upswitch.direction = Direction.INPUT
upswitch.pull = Pull.UP
sound_module = DigitalInOut(board.D3)
sound_module.direction = Direction.OUTPUT
sound_module.drive_mode = DriveMode.OPEN_DRAIN
sound_module.value = True

# initialize neopixel
pixel_pin = board.D0
num_pixels = 4
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.5, auto_write=True)

# subroutines

# subroutine to read the button states, turn on the light on the button,
# and set the button variable
def read_button_state():
    if not upswitch.value:
        global up_call_state
        up_call_state = 1
        pixels[2] = (255, 255, 255)  # turn on up button
        print("Going Up")
    if not downswitch.value:
        global down_call_state
        down_call_state = 1
        print("Going Down")
        pixels[3] = (255, 255, 255)  # turn on down button

# subroutine to delay while constantly checking if the buttons
# are being pressed
def delay_loop(timedelay):
    print("Time delay ", timedelay)
    while timedelay > 0:
        read_button_state()  # check if a button is being pressed
        time.sleep(0.010)  # pause
        timedelay = timedelay-10  # incrememnt floortime down

# subroutine to clear DOWN call state because elevator arrived
def arrive_going_down():
        global down_call_state
        pixels[0] = (255, 255, 255)  # turn on the down arrow
        # play buzzer
        sound_module.value = False
        sound_module.value = True
        print("Elevator arrived going down")
        delay_loop(1000)  # delay loop
        pixels[0] = (0, 0, 0)  # turn off down lights
        pixels[3] = (0, 0, 0)
        print("Elevator Left")
        down_call_state = 0

# subroutine to clear UP call state because elevator arrived
def arrive_going_up():
        global up_call_state
        pixels[1] = (255, 255, 255)  # turn on the up arrow
        # play buzzer
        sound_module.value = False
        sound_module.value = True
        print("Elevator arrived going up")
        delay_loop(1000)  # delay loop
        pixels[1] = (0, 0, 0)  # turn off up lights
        pixels[2] = (0, 0, 0)
        print("Elevator Left")
        up_call_state = 0

# end subroutines

# turn off pixels before we start
pixels.fill((0, 0, 0))

# main loop
while True:
    # If no buttons are pressed we just keep reading the state
    read_button_state()
# If either or both button is pressed we call the elevator
    if up_call_state + down_call_state > 0:
        #  If either or both button is pressed we call the elevator
        randfloor = random.randint(1, 6)
        # sets how long the elevator will take to arrive
        floortime = randfloor * 1000
        print("Floors away ", randfloor)
        # delay while we wait for the elevator
        delay_loop(floortime)

# elevator arrives and we decide which direction it is going and
# act accordingly
    # are both up and down pressed? If yes we turn off one
        if up_call_state + down_call_state == 2:
            print("if both")
            # sets which direction the elevator arrived going in
            randdirection = random.randint(1, 2)
            if randdirection == 1:
                arrive_going_up()
            else:
                arrive_going_down()
        # only the up button is pressed
        elif up_call_state == 1:
            arrive_going_up()
        # must be only the down button
        else:
            arrive_going_down()