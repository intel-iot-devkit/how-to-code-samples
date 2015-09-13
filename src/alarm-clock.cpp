#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <thread>

#include <grove.h>
#include <buzzer.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/crow/crow_all.h"


void message(upm::Jhd1313m1* screen, const std::string& input, const std::size_t color = 0x0000ff) {
	std::size_t red   = (color & 0xff0000) >> 16;
	std::size_t green = (color & 0x00ff00) >> 8;
	std::size_t blue  = (color & 0x0000ff);

	// TODO: pad input string to fill up display
	std::stringstream text;
	text << input;
	screen->setCursor(0,0);
	screen->write(text.str());
	screen->setColor(red, green, blue);
}

void runner(upm::GroveButton* button) {
	bool wasPressed = false;
	bool currentlyPressed = false;

	for (;;) {
		currentlyPressed = button->value();
		if ( currentlyPressed && ! wasPressed ) {
			std::cerr << "Pressed" << std::endl;
		} else if (! currentlyPressed && wasPressed ) {
			std::cerr << "Released" << std::endl;
		}

		wasPressed = currentlyPressed;
		sleep(1);
	}
}

int main() {
	// rotary connected to A0 (analog in)
	upm::GroveRotary* rotary = new upm::GroveRotary(0);

	// button connected to D4 (digital out)
	upm::GroveButton* button = new upm::GroveButton(4);

	// buzzer connected to D5 (digital out)
	upm::Buzzer* buzzer = new upm::Buzzer(5);

	// screen connected to the default I2C bus
	upm::Jhd1313m1* screen = new upm::Jhd1313m1(0);

	std::thread t1(runner, button);

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([]() {
		return "Hello world!";
	});

	app.port(4567).multithreaded().run();

	t1.join();

	delete rotary;
	delete button;
	delete buzzer;
	delete screen;

	return 0;
}
