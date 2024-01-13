#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>
#include <MAVLink.h>

// Usage: connect to Wi-Fi network and use QGroundControl to open MAVLink connection

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  WiFi.softAP("MAVLink", "12345678");
  udp.begin(14550);
}

void loop() {
  sendMAVLink();
  receiveMAVLink();
}

void sendMAVLink() {
  static uint32_t lastSent = 0;
  if (millis() - lastSent < 1000) return; // Send every second

  // Generate HEARTBEAT message buffer
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  mavlink_msg_heartbeat_pack(1, MAV_COMP_ID_AUTOPILOT1, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_FLAG_MANUAL_INPUT_ENABLED, 0, MAV_STATE_STANDBY);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // Send buffer over UDP
	udp.beginPacket("255.255.255.255", 14550);
	udp.write(buf, len);
	udp.endPacket();

  lastSent = millis();
}

void receiveMAVLink() {
  int packetSize = udp.parsePacket();
  if (!packetSize) return;

  // Read UDP packet
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  udp.read(buf, MAVLINK_MAX_PACKET_LEN);

  // Parse MAVLink message
  mavlink_message_t msg;
  mavlink_status_t status;
  for (int i = 0; i < packetSize; i++) {
    if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
      switch (msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:
          Serial.println("Received HEARTBEAT");
          break;
        case MAVLINK_MSG_ID_MANUAL_CONTROL:
          // Can be used to control the vehicle
          mavlink_manual_control_t manualControl;
          mavlink_msg_manual_control_decode(&msg, &manualControl);
          Serial.print("Received MANUAL_CONTROL:");
          Serial.print(" x=");
          Serial.print(manualControl.x);
          Serial.print(" y=");
          Serial.print(manualControl.y);
          Serial.print(" z=");
          Serial.print(manualControl.z);
          Serial.print(" r=");
          Serial.println(manualControl.r);
          break;
        default:
          Serial.print("Received message with ID ");
          Serial.println(msg.msgid);
          break;
      }
    }
  }
}
