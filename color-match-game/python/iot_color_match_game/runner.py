# Copyright (c) 2015 - 2016 Intel Corporation.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

from __future__ import print_function
from importlib import import_module
from collections import namedtuple
from random import randint
from time import sleep
from pkg_resources import Requirement, resource_filename
from bottle import Bottle, static_file, request, HTTPResponse
from .config import HARDWARE_CONFIG

GameColor = namedtuple("GameColor", "red blue green yellow")
GAME_COLOR = GameColor(
    red=0,
    blue=1,
    green=2,
    yellow=3
)

class Runner(object):

    def __init__(self):

        self.project_name = "Color Match Game"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.server = Bottle()
        self.server.route("/", callback=self.serve_index)
        self.server.route("/static/<filepath:path>", callback=self.serve_static)
        self.server.route("/api/colors/<color>", callback=self.handle_color)
        self.server.route("/api/actions/<action>", callback=self.handle_action)

        self.game_level = 0
        self.game_move = 0
        self.color_sequence = []

        self.board.change_background("white")

    # game methods

    def start_game(self):

        print("Starting new color game.")

        self.game_level = 0
        self.game_move = 0
        self.color_sequence = [randint(0, 3)]

        print("Game level: {0}.".format(self.game_level))
        print("Game move: {0}.".format(self.game_move))
        print("Color sequence: {0}.".format(self.color_sequence))

        self.play_sequence()

    def next_level(self):

        print("Start new game level.")

        self.game_level += 1
        self.game_move = 0
        self.color_sequence.append(randint(0, 3))

        print("Game level: {0}.".format(self.game_level))
        print("Game move: {0}.".format(self.game_move))
        print("Color sequence: {0}.".format(self.color_sequence))

        self.play_sequence()

    def match_color(self, color_index):

        given_color = color_index
        expected_color = self.color_sequence[self.game_move]
        result = given_color == expected_color

        print("Color match: {0} given, {1} expected, result {2}".format(
            given_color,
            expected_color,
            result
        ))

        return result

    def end_level(self):

        print("Checking level end.")

        self.game_move += 1
        if self.game_move == len(self.color_sequence):
            print("Reached game level end.")
            return True
        else:
            print("Continue game level.")
            return False

    def play_sequence(self):

        board_colors = {
            GAME_COLOR.red: lambda: self.board.change_background("red"),
            GAME_COLOR.blue: lambda: self.board.change_background("blue"),
            GAME_COLOR.green: lambda: self.board.change_background("green"),
            GAME_COLOR.yellow: lambda: self.board.change_background("yellow"),
        }

        for color in self.color_sequence:
            action = board_colors.get(color)
            action()
            sleep(1)
            self.board.change_background("white")
            sleep(1)

    # server methods

    def start(self):

        """
        Start runner.
        """

        self.server.run(
            host="0.0.0.0",
            port=3000
        )

    def serve_index(self):

        """
        Serve the 'index.html' file.
        """

        resource_package = __name__
        resource_path = "index.html"
        package_root = resource_filename(resource_package, "static")
        return static_file(resource_path, root=package_root)

    def serve_static(self, filepath):

        """
        Serve static files.
        """

        resource_package = __name__
        resource_path = filepath
        package_root = resource_filename(resource_package, "static")
        return static_file(resource_path, root=package_root)

    def handle_action(self, action):

        """
        Handle API actions call.
        """

        print("HTTP API /actions call: {0}.".format(action))

        actions = {
            "startGame": self.start_game,
            "nextLevel": self.next_level,
        }
        handler = actions.get(action)
        print("handler:", handler)
        if handler:
            handler()
            return HTTPResponse(status="200 Action Executed")
        else:
            return HTTPResponse(status="404 Unknown Action")

    def handle_color(self, color):

        """
        Handle API colors call.
        """

        print("HTTP API /colors call: {0}.".format(color))

        colors = {
            "red": GAME_COLOR.red,
            "blue": GAME_COLOR.blue,
            "green": GAME_COLOR.green,
            "yellow": GAME_COLOR.yellow
        }
        raw_color = colors.get(color)
        if not raw_color is None:
            color_match = self.match_color(raw_color)
            if color_match:
                level_end = self.end_level()
                if level_end:
                    return HTTPResponse(status="204 Color Match - End Level")
                else:
                    return HTTPResponse(status="200 Color Match")
            else:
                return HTTPResponse(status="406 Color Mismatch")
        else:
            return HTTPResponse(status="404 Color Not Found")
