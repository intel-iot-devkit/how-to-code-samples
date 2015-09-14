#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <ctime>


#include <grove.h>
#include <buzzer.h>
#include <jhd1313m1.h>

#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/crow/crow_all.h"

#include "index_html.h"

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

	void adjust_brightness()
	{
		std::size_t newColor;
		newColor = std::round((rotary->abs_value() / 1020) * 255);
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
std::time_t alarmTime ;

bool time_for_alarm() {
	if (std::difftime(std::time(NULL), alarmTime) < 5 && !alarmRinging) {
		return true;
	} else return false;
}

void runner(Devices& devices, std::time_t& alarmTime) {
	for (;;) {
		devices.display_time();

		if (time_for_alarm()) {
			alarmRinging = true;
			// TODO: get weather data
		}

		if (alarmRinging) {
			if ( devices.button->value() ) {
				alarmRinging = false;
				devices.buzzer->stopSound();
				// TODO: set alarm time to tomorrow
			} else {
				devices.buzzer->setVolume(0.5);
				devices.buzzer->playSound(2600, 0);
			}
		}

		devices.adjust_brightness();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main() {
	Devices devices;
	devices.init();

	std::thread t1(runner, std::ref(devices), std::ref(alarmTime));

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([](const crow::request& req) {
		struct tm * timeinfo;
		timeinfo = localtime(&alarmTime);

		if(req.url_params.get("hour") != nullptr) {
			double hour = boost::lexical_cast<double>(req.url_params.get("hour"));
			timeinfo->tm_hour = hour;
		}

		if(req.url_params.get("minute") != nullptr) {
			double minute = boost::lexical_cast<double>(req.url_params.get("minute"));
			timeinfo->tm_min = minute;
		}

		if(req.url_params.get("second") != nullptr) {
			double second = boost::lexical_cast<double>(req.url_params.get("second"));
			timeinfo->tm_sec = second;
		}

		alarmTime = mktime(timeinfo);

		std::stringstream text;
		text << index_html;
		return text.str();
	});

	CROW_ROUTE(app, "/alarm.json")
	([]() {
		struct tm * timeinfo;
		timeinfo = localtime(&alarmTime);

		crow::json::wvalue a;
		a["hours"] = timeinfo->tm_hour;
		a["minutes"] = timeinfo->tm_min;
		a["seconds"] = timeinfo->tm_sec;
		return a;
	});

	app.port(4567).multithreaded().run();

	t1.join();

	devices.cleanup();

	return 0;
}
