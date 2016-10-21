from __future__ import print_function, division

from math import floor

from pyupm_buzzer import Buzzer
from pyupm_i2clcd import Jhd1313m1
from pyupm_grove import GroveButton, GroveRotary

from mraa import addSubplatform, GENERIC_FIRMATA

from events import scheduler, emitter, ms

# pin mappings
buzzer_pin = 6
button_pin = 5
rotary_pin = 0
i2c_bus = 6

# hardware functions
def start_buzzer():
    buzzer.setVolume(0.5)
    buzzer.playSound(2600, 0)

def stop_buzzer():
    buzzer.stopSound()
    buzzer.stopSound()

def write_message(message, line=0):
    message = message.ljust(16)
    screen.setCursor(line, 0)
    screen.write(message)

def set_screen_background(color):
    colors = {
        "red": lambda: screen.setColor(255, 0, 0),
        "white": lambda: screen.setColor(255, 255, 255)
    }
    colors.get(color, colors["white"])()

# setup hardware event dispatch
def hardware_event_loop():    
    global prev_button
    emitter.emit("rotary", value=rotary.abs_value())
    pressed = button.value()
    prev_button = 0

    if pressed and not prev_button:
        emitter.emit("button-pressed")

    if not pressed and prev_button:
        emitter.emit("button-released")

    prev_button = pressed

scheduler.add_job(hardware_event_loop, "interval", seconds=ms(250))

def brightness_adjustment(value):
    start = 0
    end = 1020
    value = int(floor((value - start) / end * 255))
    value = 0 if value < 0 else 255 if value > 255 else value
    screen.setColor(value, value, value)

emitter.on("rotary", brightness_adjustment)

# hardware init
def init_hardware(config):
    global buzzer_pin
    global button_pin
    global rotary_pin
    global i2c_bus

    global buzzer
    global button
    global rotary
    global screen

    if "platform" in config and config["platform"] == "firmata":
        addSubplatform("firmata", "/dev/ttyACM0")
        buzzer_pin += 512
        button_pin += 512
        rotary_pin += 512
        i2c_bus += 512

    buzzer = Buzzer(buzzer_pin)
    button = GroveButton(button_pin)
    rotary = GroveRotary(rotary_pin)
    screen = Jhd1313m1(i2c_bus, 0x3E, 0x62)