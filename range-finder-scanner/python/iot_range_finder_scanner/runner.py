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
from json import dumps
from pkg_resources import resource_filename
from bottle import Bottle, static_file, response
from .config import HARDWARE_CONFIG
from .scheduler import SCHEDULER

class Runner(object):

    def __init__(self):

        self.project_name = "Range Finder Scanner"

        board_name = HARDWARE_CONFIG.kit
        board_module = "{0}.hardware.{1}".format(__package__, board_name)
        board_class_name = "{0}Board".format(board_name.capitalize())
        self.board = getattr(import_module(board_module), board_class_name)()

        self.server = Bottle()
        self.server.route("/", callback=self.serve_index)
        self.server.route("/data.json", callback=self.serve_data)

        self.data = [False for _ in range(360)]
        self.degree = 0

        self.sweep_job = SCHEDULER.add_job(
            self.sweep,
            "interval",
            seconds=10,
            coalesce=True,
            max_instances=1
        )

    def start(self):

        """
        Start runner.
        """

        self.server.run(
            host="0.0.0.0",
            port=3000
        )

    def sweep(self):

        degree = 0 if self.degree > 359 else self.degree
        self.data[degree] = self.board.detect_object()

        print("Degree: {0}, Object: {1}.".format(degree, self.data[degree]))

        self.degree = degree + 1
        self.board.move()

    # setup HTTP server

    def serve_index(self):

        """
        Serve the 'index.html' file.
        """

        resource_package = __name__
        resource_path = "index.html"
        package_root = resource_filename(resource_package, "")
        return static_file(resource_path, root=package_root)

    def serve_data(self):

        """
        Serve 'data.json' data.
        """

        response.content_type = 'application/json'
        return dumps(self.data)
