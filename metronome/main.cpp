#include <chrono>
#include <thread>

#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <wiringPi.h>

#include "metronome.hpp"
#include "rest.hpp"

#define LED_RED   17
#define LED_GREEN 27
#define BTN_MODE  23
#define BTN_TAP   24

using namespace std::chrono_literals;

metronome m;

// APIs
void get_bpm(web::http::http_request msg) {
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.set_body(web::json::value::number(m.get_bpm()));
	msg.reply(response);
}

void get_min(web::http::http_request msg) {
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.set_body(web::json::value::number(m.get_bpm_min()));
	msg.reply(response);
}

void get_max(web::http::http_request msg) {
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.set_body(web::json::value::number(m.get_bpm_max()));
	msg.reply(response);
}

void set_bpm(web::http::http_request msg)
{
    int bpm = std::stoi(utility::conversions::to_utf8string(msg.extract_string().get()));
    cout << "receive new bpm: " << bpm << "\n";
    m.set_bpm(bpm);

    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
	msg.reply(response);
}

void reset_max(web::http::http_request msg) {
    m.reset_bpm_max();

    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
	msg.reply(response);
}

void reset_min(web::http::http_request msg) {
    m.reset_bpm_min();

    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
	msg.reply(response);
}

PI_THREAD(blink)
{
	while (true) {
		if (m.is_timing()) {
            delay(10);
            continue;
		}

        // play at recorded frequency
        int bpm = m.get_bpm();

        if (bpm == 0) {
            delay(10);
            continue;
        }

        int interval = 60 * 1000 / bpm / 2;
		digitalWrite(LED_RED, HIGH);
        delay(interval);
        digitalWrite(LED_RED, LOW);
        delay(interval);
	}
}

void switchMode() {
    if (!m.is_timing()) { // is on playing mode
        cout << "switch to learning mode\n";
        m.start_timing();
    }
    else {
        cout << "switch to playing mode\n";
        m.stop_timing();
    }
}

void tap() {
    if (!m.is_timing())
    {
        return;
    }

    cout << "get one tap: ";
    m.tap();

    // light for 200ms for each tap
    digitalWrite(LED_GREEN, HIGH);
    delay(200);
    digitalWrite(LED_GREEN, LOW);
}

int main()
{
    // start APIs
    auto bpm_rest = rest::make_endpoint("/bpm/");
	bpm_rest.support(web::http::methods::GET, get_bpm);
	bpm_rest.support(web::http::methods::PUT, set_bpm);
	bpm_rest.open().wait();

	auto max_rest = rest::make_endpoint("/bpm/max");
	max_rest.support(web::http::methods::GET, get_max);
	max_rest.support(web::http::methods::DEL, reset_max);
	max_rest.open().wait();

	auto min_rest = rest::make_endpoint("/bpm/min");
	min_rest.support(web::http::methods::GET, get_min);
	min_rest.support(web::http::methods::DEL, reset_min);
	min_rest.open().wait();

    wiringPiSetupGpio();

    piThreadCreate(blink);

	wiringPiISR(BTN_MODE, INT_EDGE_RISING, switchMode);
    wiringPiISR(BTN_TAP, INT_EDGE_RISING, tap);

    while (true) {}

    return 0;
}
