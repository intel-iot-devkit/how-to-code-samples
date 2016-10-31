from mraa import Gpio
from pyupm_i2clcd import SAINSMARTKS
from pyupm_grove import GroveButton, GroveRotary

from events import scheduler, emitter, ms
from board import Board

class DfrobotBoard(Board):

    def __init__(self, config):

        # pin mappings
        self.buzzer_pin = 15
        self.button_pin = 16
        self.rotary_pin = 3
        
        self.buzzer = Gpio(self.buzzer_pin)
        self.button = GroveButton(self.button_pin)
        self.rotary = GroveRotary(self.rotary_pin)
        self.screen = SAINSMARTKS(8, 9, 4, 5, 6, 7, 0)

        # setup hardware event dispatch
        emitter.on("rotary", self.change_brightness)

        self.prev_button = 0

        def hardware_event_loop():
            rotary = self.rotary
            emitter.emit("rotary", value=rotary.abs_value())

            pressed = self.button.value()

            if pressed and not self.prev_button:
                emitter.emit("button-pressed")

            if not pressed and self.prev_button:
                emitter.emit("button-released")

            self.prev_button = pressed
        
        scheduler.add_job(hardware_event_loop, "interval", seconds=ms(250))

    # hardware functions
    def start_buzzer(self):

        """
        Start the hardware buzzer.
        """

        self.buzzer.write(1)

    def stop_buzzer(self):

        """
        Stop the hardware buzzer.
        """

        self.buzzer.write(0)

    def write_message(self, message, line=0):

        """
        Write message to LCD screen.
        """

        message = message.ljust(16)
        self.screen.setCursor(line, 0)
        self.screen.write(message)

    def change_background(self, color):
        
        """
        Change LCD screen background color.
        No effect on the dfrobot.
        """
        
        pass

    def change_brightness(self, value):

        """
        Change LCD screen brightness.
        Only supports on (value < 512) and off (value >= 512) on the drobot.
        """

        if value < 512:
            screen.displayOff()
        else:
            screen.displayOn()
    