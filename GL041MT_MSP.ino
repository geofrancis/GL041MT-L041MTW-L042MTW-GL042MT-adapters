
#include <SoftwareSerial.h>
#include <MSP.h>

MSP msp;

#define COM 0x55

// Define variables
uint8_t buffer_RTT[4];
uint8_t CS;
int distance = 0;
int distances = 0;
SoftwareSerial mySerial(10, 11);

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  read_sonar();
  send_MSP();
}



void read_sonar() {
  mySerial.write(COM);
  delay(100);
  if (mySerial.available() > 0) {
    delay(4);
    if (mySerial.read() == 0xFF) {
      buffer_RTT[0] = 0xFF;
      for (int i = 1; i < 4; i++) {
        buffer_RTT[i] = mySerial.read();
      }
      CS = buffer_RTT[0] + buffer_RTT[1] + buffer_RTT[2];
      if (buffer_RTT[3] == CS) {
        distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
        distances = (distance / 10);
      } else {
        distance = 0;
      }
    }
  }
}

void send_MSP() {

  Serial.print("SONAR ");
  Serial.println(distance);
  uint8_t sonarrange[] = { 255, distances };
  msp.send(0x1F01, &sonarrange, sizeof(sonarrange));

}
