# MAVLink v2 library for Arduino

MAVLink is a lightweight communication protocol for communication between drones (and/or ground systems) and their components.

The underlying MAVLink protocol library automatically gets updated once a month. Correspondence between the library release and MAVLink release can be seen in the [`versions.yaml`](versions.yaml) file.

Official MAVLink documentation: https://mavlink.io/en/.

## Usage

```cpp
#include <MAVLink.h>

void setup() {
  Serial.begin(57600);
}

void loop() {
  // Send HEARTBEAT message to Serial once a second
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  mavlink_msg_heartbeat_pack(1, MAV_COMP_ID_AUTOPILOT1, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_FLAG_MANUAL_INPUT_ENABLED, 0, MAV_STATE_STANDBY);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

  Serial.write(buf, len);
  delay(1000);
}
```

The default used [MAVLink dialect](https://mavlink.io/en/messages/#dialects) is `common`. You can use another standard MAVLink dialect like this:

```cpp
#include <MAVLink_ardupilotmega.h>
```

```cpp
#include <MAVLink_matrixpilot.h>
```

## Copyright

Copyright for the Arduino library © Oleg Kalachev, 2024. License: [MIT](LICENSE.txt).
