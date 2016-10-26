from __future__ import print_function

from board import Board

class SimBoard(Board):

    def __init__(*args, **kwargs): pass

    def start_buzzer(self):
        print("sim: start_buzzer")

    def stop_buzzer(self):
        print("sim: stop_buzzer")        

    def write_message(self, message, line=0):
        print("sim: write_message - [line {1}] {0}".format(message, line))

    def change_background(color):
        print("sim: change_background - {0}".format(color))

    def change_brightness(value):
        print("sim: change_brightness - {0}".format(value))