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

from __future__ import print_function, division
from time import sleep
from signal import SIGINT, signal
from atexit import register as register_exit
from argparse import ArgumentParser
from project.runner import Runner

def debug():

    """
    Wait for debugger attach.
    """

    try:
        import os
        import inspect
        import ptvsd
        module_root = os.path.dirname(inspect.getfile(main))
        print("Debug mode enabled. Waiting for debugger attach.")
        print("    remote root:", module_root)
        print("    debug port:", 5151)
        print("    debug secret:", "debug_secret")
        ptvsd.enable_attach("debug_secret", address = ('0.0.0.0', 5151))
        ptvsd.wait_for_attach()
    except ImportError:
        print("Debug prerequisites not avalible.")
    
    main()

def main():

    """
    Start main function.
    """

    def signal_handler(signum, frame):
        raise SystemExit

    def exit_handler():
        print("exiting.")
        exit(0)

    register_exit(exit_handler)
    signal(SIGINT, signal_handler)

    runner = Runner()
    print("Running {0} example.".format(runner.project_name))
    runner.start()

    try:
        signal.pause()
    except AttributeError:
        while True:
            sleep(0.5)

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--debug", action="store_true", default=False)
    options = parser.parse_args()
    if options.debug:
        debug()
    else:
        main()
