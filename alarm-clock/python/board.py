from abc import ABCMeta, abstractmethod

class Board(object):

    __metaclass__ = ABCMeta

    @abstractmethod
    def start_buzzer(self): pass

    @abstractmethod
    def stop_buzzer(self): pass

    @abstractmethod
    def write_message(self, message, line=0): pass

    @abstractmethod
    def change_background(color): pass

    @abstractmethod
    def change_brightness(value): pass