#ifndef __protocol_h
#define __protocol_h

// Config_mode payloads
/*InoMation
link
nodelink
control
controll
contmation
packetmation
linkaround
nodelink
connect
linknode
automae
autolink
autocontrol
mayshun
mation
wavemation
*/


#define PL_SET_ADDR	1
// payload[1] = PKT_ADDR_L
// payload[2] = PKT_ADDR_H
#define PL_REQ_ADDR 2
#define PL_REQ_CAPS 3
#define PL_CAPS_RESPONSE 4
// payload[1] = CAPS_SENSOR
// payload[2] = CAPS_INPUTS
// payload[3] = CAPS_OUTPUTS
// payload[4] = CAPS_OTHER1
// payload[5] = RESERVED
// payload[6] = RESERVED
// payload[7] = RESERVED
#define PL_ADDR_RESPONSE 5
// payload[1] = PKT_ADDR_L
// payload[2] = PKT_ADDR_H
#define PL_REQ_SENSOR 6
// payload[1] = bit of sensor set

#define PL_SENSOR_RESPONSE 7
// payload[1] = SENSOR
// payload[2] = SENSOR_RES_L
// payload[3] = SENSOR_RES_H

#define PL_REQ_INFORM_ON_CHANGE 8
// payload[1] = CAPS_SENSOR mask
// payload[2] = CAPS_INPUTS mask

#define PL_CHANGE_RESPONSE 9
// payload[1] = CAPS_SENSOR mask
// payload[2] = CAPS_INPUTS mask
// values?

#define PL_SET_OUTPUT 9
// payload[1] = CAPS_OUTPUT mask  - all bits set will get changed
// payload[2] = value_h
// payload[3] = value_l

#define PL_REQ_INFO1 10
// payload[1] = bit of which other to send

#define PL_OTHER_RESPONSE 11
// payload[1] = OTHER
// payload[2] = OTHER_RES_H
// payload[3] = OTHER_RES_L

// EEPROM locations

#define EE_MY_ADDR_H 0x00
#define EE_MY_ADDR_L 0x01

#define EE_MY_LAST_PKT_ID_H 0x02
#define EE_MY_LAST_PKT_ID_L 0x03

#define EE_MY_SENSORS 0x04
#define EE_MY_INPUTS  0x05
#define EE_MY_OUTPUTS 0x06
#define EE_MY_INFO1   0x07

// Bit

// EE_MY_SENSORS bits
#define CAPS_SENSOR_TEMP  			0
#define CAPS_SENSOR_HUMIDITY   		1
#define CAPS_SENSOR_AIR_PRESSURE 	2
#define CAPS_SENSOR_LIGHT       	3
#define CAPS_SENSOR_PRESENCE		4

// EE_MY_INPUTS
#define CAPS_INPUTS_SWITCH1		0
#define CAPS_INPUTS_SWITCH2		1
#define CAPS_INPUTS_SWITCH3		2
#define CAPS_INPUTS_SWITCH4		3
#define CAPS_INPUTS_SWITCH5		4
#define CAPS_INPUTS_SWITCH6		5
#define CAPS_INPUTS_SWITCH7		6
#define CAPS_INPUTS_SWITCH8		7

// EE_MY_OUTPUTS
#define CAPS_OUTPUTS_RELAY1		0
#define CAPS_OUTPUTS_RELAY2		1
#define CAPS_OUTPUTS_RELAY3		2
#define CAPS_OUTPUTS_RELAY4		3
#define CAPS_OUTPUTS_DIMMER1	4
#define CAPS_OUTPUTS_DIMMER2	5
#define CAPS_OUTPUTS_DIMMER3	6
#define CAPS_OUTPUTS_DIMMER4	7

#define CAPS_INFO1_TIME 		0
#define CAPS_INFO1_DATE 		1

#endif
