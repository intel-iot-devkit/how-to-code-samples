#include <iostream>
#include <string>
#include <grove.h>
#include <yg1006.h>
#include <otp538u.h>
#include <GroveSpeaker.h>
#include "../lib/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../lib/crow/crow_all.h"
#include "html.h"

#define OTP538U_AREF 5.0

using namespace std;

float myTemp = 1000;//global temp value for app; will change when you use app

void notify() {//send signal to server
	if (!getenv("SERVER") && !getenv("AUTH_TOKEN")) {
		cerr << "Server not configured." << std::endl;
		return;
	}
	RestClient::headermap headers;
	headers["X-Auth-Token"] = getenv("AUTH_TOKEN");
	RestClient::response r = RestClient::get(getenv("SERVER"), headers);
	cerr << r.code << std::endl;
	cerr << r.body << std::endl;
}

struct Devices
{
	upm::GroveSpeaker* speaker;
	upm::YG1006* flame;
	upm::OTP538U* temps;

	Devices(){
	};

	void init() {

	    // speaker connected to D5 (digital out)
		speaker = new upm::GroveSpeaker(5);
		// flame sensor on D4
		flame = new upm::YG1006(4);
		// Instantiate a OTP538U on analog pins A0 and A1
		// A0 is used for the Ambient Temperature and A1 is used for the
		// Object temperature.
		// only plug ir temp sensor into A0 with this code
		temps = new upm::OTP538U(0, 1, OTP538U_AREF);
	}

	void cleanup() {
		delete speaker;
		delete flame;
		delete temps;
	}

	void alarm() {//starts the alarm
		  speaker->playSound('c', true, "high");
	}

	void senseFlame() {//keeps the flame sensor running
		for(;;)
		    {
		      bool val = flame->flameDetected();
		      if (val){
		    	  cout << "Flame detected." << endl;
		    	  alarm();
		      }

		      else
		      {
		        cout << "No flame detected." << endl;
		        sleep(1);
		      }
		    }
	}

	void senseTemp(){
		//keeps the ir temperature sensor running and checking every minute for object and ambient temp
		//compares object temp to my temp
		//sounds alarm if object temp is higher than set temp
	  for(;;)
		    {

		        cout << "Ambient temp: " << std::fixed << setprecision(2)
		             << temps->ambientTemperature()
		             << " C, Object temp: " << temps->objectTemperature()
		             << " C" << endl;

		      float temp1 = temps->objectTemperature();

		      if(temp1 >= myTemp){
		       	  notify();
		       	  alarm();
		      }
		      else
		    	  sleep(60);
		    }
	}
};

void runFlame(Devices& devices){//keeps the flame sensor running
		devices.senseFlame();
	}

void runTemp(Devices& devices){//keeps the ir temperature sensor running and checking every minute for object and ambient temp
		devices.senseTemp();
	}

int main()
{
	// check that we are running on Galileo or Edison
	mraa_platform_t platform = mraa_get_platform_type();
	if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
		(platform != MRAA_INTEL_GALILEO_GEN2) &&
		(platform != MRAA_INTEL_EDISON_FAB_C)) {
		cerr << "ERROR: Unsupported platform" << endl;
		return MRAA_ERROR_INVALID_PLATFORM;
	}
	// create and initialize UPM devices
	Devices devices;
	devices.init();

	// start worker thread for device communication
    thread t1(runTemp, ref(devices));
    thread t2(runFlame, ref(devices));
	// define web server & routes
	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
		([](const crow::request& req) {

        if(req.url_params.get("temp") != nullptr) {
            double newTemp = boost::lexical_cast<double>(req.url_params.get("temp"));
            cout << "The value of 'temp' is " <<  newTemp << endl;
            myTemp = newTemp;

        }
			stringstream text;
			text << index_html;
			return text.str();
	});

	// start web server
	app.port(3000).multithreaded().run();

	// all done
	t1.join();
    t2.join();
	devices.cleanup();

	return 0;
}
