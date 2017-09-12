# Color match game in C++

## What it is

Using a compatible Intel® IoT Platform, this project lets you create a color match game.<br>
- a UI of the game is exposed through an embedded server, providing with buttons for color matching as well as a "start game" button<br>
- when starting the game, for each level a sequence of colors is displayed on the RGB LCD screen of the Intel® IoT Platform. The player needs to click on the corresponding buttons on the website.
- for each level the sequence has one more color.<br>
- when a user clicks a color, the same color is displayed on the RGB LCD screen.<br>

## First time setup
For all the samples in this repository, see the ![General Setup Instructions](./../../README.md#setup) for required boards and libraries.  You need either Grove or DFRobot sensors but not both.

## Hardware requirements

### Grove\*

Sensor | Pin
--- | ---
Grove RGB LCD | I2C

For more specific information on the hardware requirements see ![Hardware Details](./../README.md#hardware-requirements)

## Software requirements

1. ![MRAA](https://github.com/intel-iot-devkit/mraa) and ![UPM](https://github.com/intel-iot-devkit/upm)
Note: The following libraries are included with the repo and already linked to in the code -jsoncpp -restclient

### Running the program

To run this example on the board, simply enter

    $ make
    $ build
    $ ./color-match-game

After running the program, you should see output similar to the one in the image below.<br>
![](./../../images/cpp/cpp-run-eclipse-successful-build.png)

## Regenerating the HTML and CSS

If you make any changes to either the **index.html** or **styles.css** file, you need to regenerate the .hex file used to serve up the assets via the built-in Crow\* web server.
We have a useful tutorial on how to use the shell script here:

[how-to-run-the-shellscript.md](./../../docs/cpp/how-to-run-the-shellscript.md)


IMPORTANT NOTICE: This software is sample software. It is not designed or intended for use in any medical, life-saving or life-sustaining systems, transportation systems, nuclear systems, or for any other mission-critical application in which the failure of the system could lead to critical injury or death. The software may not be fully tested and may contain bugs or errors; it may not be intended or suitable for commercial release. No regulatory approvals for the software have been obtained, and therefore software may not be certified for use in certain countries or environments.
