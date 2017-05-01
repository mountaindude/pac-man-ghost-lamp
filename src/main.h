
void setup();
void reconnect() ;
void setup_wifi();
void colorWipe(uint32_t c, uint8_t wait);
void callback(char* topic, byte* payload, unsigned int length);
void theaterChaseRainbow(uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);
void rainbowCycle(uint8_t wait);
void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);
