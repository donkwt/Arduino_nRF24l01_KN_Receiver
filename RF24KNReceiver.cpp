//
//
//

#include "RF24KNReceiver.h"


RF24 radio(CE_PIN, CS_PIN);


void RF24KNReceiver::init()
{
  binding_addr[0] = 'K';
  binding_addr[1] = 'N';
  binding_addr[2] = 'D';
  binding_addr[3] = 'Z';
  binding_addr[4] = 'K';
  radio.begin();
}

bool RF24KNReceiver::bind()
{
  // initial setup
  static bool first_time = true;
  if (first_time) {
    radio.startListening();
    radio.stopListening();

    radio.powerDown();
    delay(10);

    radio.setRetries(0, 0);
    radio.setCRCLength(RF24_CRC_16);
    radio.setAutoAck(false);
    radio.enableDynamicPayloads();

    radio.setChannel(binding_channel);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_1MBPS);
    radio.openReadingPipe(0, binding_addr);

    radio.powerUp();
    delay(10);
    radio.startListening();
    first_time = false;
  }

  // binding
  if (radio.available()) {
    radio.read(&buffer, sizeof(buffer));

    if (buffer[0] == 0x4b && buffer[1] == 0x4e && buffer[2] == 0x44 && buffer[3] == 0x5a) {
      operation_addr[0] = buffer[4];
      operation_addr[1] = buffer[5];
      operation_addr[2] = buffer[6];
      operation_addr[3] = buffer[7];
      operation_addr[4] = 'K';

      channels[0] = buffer[8];
      channels[1] = buffer[9];
      channels[2] = buffer[10];
      channels[3] = buffer[11];

      byte conn_speed = 0;
      conn_speed = (buffer[15] & 0b00000001);

      radio.stopListening();
      radio.closeReadingPipe(0);
      conn_speed == 1 ? radio.setDataRate(RF24_1MBPS) : radio.setDataRate(RF24_250KBPS);
      radio.setChannel(channels[current_channel]);
      radio.openReadingPipe(1, operation_addr);  // 0x4b41000300
      radio.startListening();

      tries_receive = -5000 ;
      return true;
    }
  }
  return false;
}

bool RF24KNReceiver::receive()
{
  if (radio.available()) {
    if (radio.getDynamicPayloadSize() > 1) {

      radio.read(&buffer, sizeof(buffer));

      left_y  = (((int16_t)buffer[0] << 8) | buffer[1]);
      right_x = (((int16_t)buffer[2] << 8) | buffer[3]);
      right_y = (((int16_t)buffer[4] << 8) | buffer[5]);
      left_x  = (((int16_t)buffer[6] << 8) | buffer[7]);
      left_y_trim  = buffer[8];
      right_x_trim = buffer[9];
      right_y_trim = buffer[10];
      left_x_trim  = buffer[11];
      switch_dual_rate     = (bool(buffer[12] & 0b00000001));
      switch_throttle_hold = (bool((buffer[12] >> 1) & 0b00000001));
      switch_idle          = (bool((buffer[12] >> 2) & 0b00000001));
      switch_3d_6g         = (bool((buffer[12] >> 6) & 0b00000011));
      byte fq = (buffer[13] >> 2) & 0b00000011;
      byte pk = (buffer[13] >> 5) & 0b00000011;

      // switch to next channel if last packet
      if (pk == 3) {
        radio.stopListening();
        radio.closeReadingPipe(1);
        current_channel >= 3 ? current_channel = 0 : current_channel++;
        radio.setChannel(channels[current_channel]);
        radio.openReadingPipe(1, operation_addr);
        radio.startListening();
      }

      tries_receive = 0;
      return true;
    }
  }
  tries_receive++;
  return false;
}

bool RF24KNReceiver::available()
{
  return tries_receive < tries_max;
}
