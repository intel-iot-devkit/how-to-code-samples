/*
* Copyright (c) 2015 - 2017 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * @file
 * @ingroup howtocode
 * @brief Color match game in C++
 *
 * This application is part of a series of how-to Intel IoT code
 * sample exercises using the Intel® IoT Developer Kit, Intel® Edison board,
 * cloud platforms, APIs, and other technologies.
 *
 * @hardware Sensors used:\n
 * Grove LCD RGB Backlight\n
 *
 * @cc
 * @cxx -std=c++1y
 * @ld -lupm-i2clcd -lupm-jhd1313m1 -lboost_system -lboost_thread -lboost_filesystem -lboost_date_time -lpthread
 *
 * Additional source files required to build this example:
 * None
 *
 * @date 02/20/2017
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>

#include <string>

#include "../lib/crow/crow_all.h"
#include "../src/html.h"
#include "../src/css.h"
#include "../src/jquery.h"
#include "../src/main.h"

using namespace std;

#include "kits.h"
#if INTEL_IOT_KIT == DFROBOTKIT
#error "DFRobot kit not supported for this example"
#else
#include "grovekit.hpp"
#endif

string colors[] = {"red", "blue", "green", "yellow"};

// State of the game
int gameLevel = 0;
uint gameMove = 0;
std::vector<int> colorSequence;

Devices devices;

void log_level() {
   cout << "Game level:" << gameLevel << endl;
   cout << "Game move:" << gameMove << endl;
   cout << "Color sequence:" << endl;
   for (auto ci = colorSequence.begin(); ci != colorSequence.end(); ++ci)
      cout << *ci << " " << endl;
}

void play(int color) {
   switch (color) {
      case 0:
         devices.color(0xff0000);
         break;
      case 1:
         devices.color(0x0000ff);
         break;
      case 2:
         devices.color(0x008800);
         break;
      case 3:
         devices.color(0xffff00);
         break;
   }
   sleep(1);
   devices.color(0x000000);
   sleep(1);
}

void play_sequence() {
   for (auto ci = colorSequence.begin(); ci != colorSequence.end(); ++ci)
      play(*ci);
}

void start_game() {
   gameLevel = 0;
   gameMove = 0;

   colorSequence.clear();
   colorSequence.push_back(rand() % 4);

   log_level();
   play_sequence();
}

void next_level() {
   cout << "Next level..." << endl;

   gameLevel += 1;
   gameMove = 0;
   colorSequence.push_back(rand() % 4);

   log_level();
   play_sequence();
}

bool match_color(int guess) {
   return (guess == colorSequence[gameMove]);
}

bool end_level() {
   gameMove += 1;
   if (gameMove == colorSequence.size()) {
     cout << "Reached game level end." << endl;
     return true;
   }

   cout << "Continue game level." << endl;
   return false;
}

int get_color(const string& val)
{
   for(int i = 0; i < 4; i++) {
     if(colors[i].find(val, 0) != string::npos) {
       return i;
     }
   }
   return -1; // not found
}


// Exit handler for program
void exit_handler(int param)
{
  devices.cleanup();
  exit(1);
}

// The main function for the example program
int main() {
  srand(time(NULL));

  // Handles ctrl-c or other orderly exits
  signal(SIGINT, exit_handler);

  // create and initialize UPM devices
  devices.init();

  // define web server & routes
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
  ([]() {
    std::stringstream text;
    text << index_html;
    return text.str();
  });

  CROW_ROUTE(app, "/api/colors/<string>")
  ([](string a) {
    int color = get_color(a);
    if (color == -1) {
      return crow::response(404, "Color Not Found");
    }

    if (match_color(color)) {
      if (end_level())
        return crow::response(204, "Color Match - End Level");

      return crow::response(200, "Color Match");
    }

    return crow::response(406, "Color Mismatch");
  });

  CROW_ROUTE(app, "/api/actions/<string>")
  ([](string a) {
    if (a == "startGame") {
      start_game();
      return crow::response(200, "Action Executed");
    } else if (a == "nextLevel") {
      next_level();
      return crow::response(200, "Action Executed");
    }

    return crow::response(404, "Unknown Action");
  });

  CROW_ROUTE(app, "/static/css.css")
  ([]() {
    std::stringstream text;
    text << css_css;
    return text.str();
  });

  CROW_ROUTE(app, "/static/jquery.min.js")
  ([]() {
    std::stringstream text;
    text << jquery_min_js;
    return text.str();
  });

  CROW_ROUTE(app, "/static/main.js")
  ([]() {
    std::stringstream text;
    text << main_js;
    return text.str();
  });

  // start web server
  app.port(3000).multithreaded().run();

  return MRAA_SUCCESS;
}
