#include <iostream>
#include <unistd.h>
#include <sstream>

#include <ttp223.h>
#include <buzzer.h>
#include <jhd1313m1.h>

void message(upm::Jhd1313m1* screen, const std::string& input) {
	// TODO: pad input string to fill up display
	std::stringstream text;
	text << input;
	screen->setCursor(0,0);
	screen->write(text.str());
}

void reset(upm::Jhd1313m1* screen, upm::Buzzer* buzzer) {
	message(screen, "doorbot ready");
	buzzer->setVolume(0.5);
	buzzer->stopSound();
}

void increment() {

}

void dingdong(upm::Jhd1313m1* screen, upm::Buzzer* buzzer) {
	increment();
	message(screen, "ding dong!");
	buzzer->playSound(266, 0);
}

int main()
{
	// check that we are running on Galileo or Edison
	mraa_platform_t platform = mraa_get_platform_type();
	if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
		(platform != MRAA_INTEL_GALILEO_GEN2) &&
		(platform != MRAA_INTEL_EDISON_FAB_C)) {
		std::cerr << "ERROR: Unsupported platform" << std::endl;
		return MRAA_ERROR_INVALID_PLATFORM;
	}

	// touch sensor connected to D4 (digital in)
	upm::TTP223* touch = new upm::TTP223(4);

	// buzzer connected to D5 (digital out)
	upm::Buzzer* buzzer = new upm::Buzzer(5);

	// screen connected to the default I2C bus
	upm::Jhd1313m1* screen = new upm::Jhd1313m1(0);

	// simple error checking
	if ((touch == NULL) || (buzzer == NULL) || (screen == NULL)) {
		std::cerr << "ERROR: Can't connect to all devices." << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	reset(screen, buzzer);

	bool wasPressed = false;

	for (;;) {
		bool currentlyPressed = touch->isPressed();
		if ( currentlyPressed && ! wasPressed ) {
			dingdong(screen, buzzer);
		} else if (! currentlyPressed && wasPressed ) {
			reset(screen, buzzer);
		}

		wasPressed = currentlyPressed;
		sleep(1);
	}

	delete touch;
	delete buzzer;
	delete screen;

	return MRAA_SUCCESS;
}
