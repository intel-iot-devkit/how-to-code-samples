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
		stop_buzzing();

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

	void start_buzzing() {
		buzzer->setVolume(0.5);
		buzzer->playSound(2600, 0);
	}

	void stop_buzzing() {
		buzzer->stopSound();
		buzzer->stopSound();
	}
};

bool alarmRinging = false;
std::time_t alarmTime ;

double countdown(std::time_t& target) {
	time_t rawtime;
	struct tm* timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	return std::difftime(mktime(timeinfo), target);
}

bool time_for_alarm(std::time_t& alarm) {
	double remaining = countdown(alarm);

	if (remaining > 0 && remaining < 5 && !alarmRinging) {
		return true;
	} else return false;
}

void log_wakeup() {
	double duration = countdown(alarmTime);
	std::cerr << "Seconds to wakeup: " << std::to_string(duration) << std::endl;

	if (!getenv("SERVER") && !getenv("AUTH_TOKEN")) {
		std::cerr << "Server not configured." << std::endl;
		return;
	}

	std::stringstream text;
	text << "{\"value\": \"" << std::to_string(duration) << "\"}";

	RestClient::headermap headers;
	headers["X-Auth-Token"] = getenv("AUTH_TOKEN");

	RestClient::response r = RestClient::put(getenv("SERVER"), "text/json", text.str(), headers);
	std::cerr << r.code << std::endl;
	std::cerr << r.body << std::endl;
}

std::string get_weather() {
	if (!getenv("API_KEY")) {
		std::cerr << "Weather Underground API_KEY not configured." << std::endl;
		return "";
	}

	std::stringstream url;
	url <<  "http://api.wunderground.com/api/" << getenv("API_KEY") << "/conditions/q/CA/San_Francisco.json";

	RestClient::headermap headers;
	headers["Accept"] = "application/json";

	RestClient::response r = RestClient::get(url.str(), headers);

	auto x = crow::json::load(r.body);
	std::string result(x["current_observation"]["weather"].s());
	return result;
}

void runner(Devices& devices, std::time_t& alarmTime) {
	for (;;) {
		devices.display_time();

		if (time_for_alarm(alarmTime)) {
			alarmRinging = true;
			devices.message(get_weather());
		}

		if (alarmRinging) {
			if ( devices.button->value() ) {
				alarmRinging = false;
				devices.stop_buzzing();

				// log how long it took to wake up
				log_wakeup();

				// set alarm time to tomorrow
				struct tm* timeinfo;
				timeinfo = localtime(&alarmTime);
				timeinfo->tm_mday++;
				alarmTime = mktime(timeinfo);
				std::cerr << "New alarm time: " << alarmTime << std::endl;

			} else {
				devices.start_buzzing();
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
		bool timeChanged = false;
		std::time_t newTime = std::time(NULL);
		struct tm* timeinfo;
		timeinfo = localtime(&newTime);

		if(req.url_params.get("hour") != nullptr) {
			double hour = boost::lexical_cast<double>(req.url_params.get("hour"));
			timeinfo->tm_hour = hour;
			timeChanged = true;
		}

		if(req.url_params.get("minute") != nullptr) {
			double minute = boost::lexical_cast<double>(req.url_params.get("minute"));
			timeinfo->tm_min = minute;
			timeChanged = true;
		}

		if(req.url_params.get("second") != nullptr) {
			double second = boost::lexical_cast<double>(req.url_params.get("second"));
			timeinfo->tm_sec = second;
			timeChanged = true;
		}

		if (timeChanged) {
			alarmTime = mktime(timeinfo);
			std::cerr << "Set alarm time: " << alarmTime << std::endl;
		}

		std::stringstream text;
		text << index_html;
		return text.str();
	});

	CROW_ROUTE(app, "/alarm.json")
	([]() {
		struct tm* timeinfo;
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
