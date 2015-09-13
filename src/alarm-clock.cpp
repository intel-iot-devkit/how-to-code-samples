#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>

#include <grove.h>
#include <buzzer.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/crow/crow_all.h"

struct Devices
{
	upm::GroveRotary* rotary;
	upm::GroveButton* button;
	upm::Buzzer* buzzer;
	upm::Jhd1313m1* screen;

	Devices(){
	};

	void init() {
		// rotary connected to A0 (analog in)
		rotary = new upm::GroveRotary(0);

		// button connected to D4 (digital out)
		button = new upm::GroveButton(4);

		// buzzer connected to D5 (digital out)
		buzzer = new upm::Buzzer(5);
		buzzer->stopSound();

		// screen connected to the default I2C bus
		screen = new upm::Jhd1313m1(0);
	};

	void cleanup() {
		delete rotary;
		delete button;
		delete buzzer;
		delete screen;
	}

	void display_time() {
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time (&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%I:%M:%S", timeinfo);
		std::string str(buffer);

		screen->setCursor(0,0);
		screen->write(str);
	}

	void adjust_brightness(float& value)
	{
		std::size_t newColor;
		newColor = std::round((value / 1020) * 255);
		if (newColor > 255) newColor = 255;
		if (newColor < 0) newColor = 0;

		screen->setColor(newColor, newColor, newColor);
	}

	void message(const std::string& input, const std::size_t color = 0x0000ff) {
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
};

bool alarmRinging = false;
float rotary_value = 0;


void check_alarm() {

}

void runner(Devices& devices, float& rot) {
	bool wasPressed = false;
	bool currentlyPressed = false;

	for (;;) {
		devices.display_time();

		check_alarm();

		currentlyPressed = devices.button->value();
		if ( currentlyPressed && ! wasPressed ) {
			std::cerr << "Pressed" << std::endl;
		} else if (! currentlyPressed && wasPressed ) {
			std::cerr << "Released" << std::endl;
		}
		wasPressed = currentlyPressed;

		rot = devices.rotary->abs_value();
		std::cerr << "Rotary: " << rot << std::endl;
		devices.adjust_brightness(rot);

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main() {
	Devices devices;
	devices.init();

	std::thread t1(runner, std::ref(devices), std::ref(rotary_value));

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([]() {
		std::stringstream text;
		text << "Rotary: " << rotary_value;
		return text.str();
	});

	app.port(4567).multithreaded().run();

	t1.join();

	devices.cleanup();

	return 0;
}
