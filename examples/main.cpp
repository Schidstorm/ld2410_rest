#define DEBUG_HTTPCLIENT

#include <memory>

#include "ld2410_state_filler.h"
#include "ld2410_webserver.h"

std::shared_ptr<Ld2410State> state = std::make_shared<Ld2410State>();
ld2410::StreamReader r{};
ld2410::StreamWriter w{};

const uint8_t detection_pin = D8;
Ld2410StateFiller filler{&r, &w, detection_pin, state.get()};
Ld2410Webserver web_server;


void setup(void) {


  Serial.begin(256000);
  filler.setup();
  web_server.begin(new AsyncWebServer(80), state.get(), r, w);
}

void loop(void) {
  filler.loop();
}