/*
 * Project rotary
 * Description:  get input from a rotary encoder
 * Author: Kirk Carlson
 * Date: 15 Apr 2019
 */

// This file contains the addresses used by rotary

#define NODE_NAME "rotary1"

/**
 * if want to use IP address:
 *   byte server[] = { XXX,XXX,XXX,XXX };
 *   MQTT client(server, 1882, callback);
 * want to use domain name:
 *   MQTT client("iot.eclipse.org", 1882, callback);
 **/
byte mqttServer[] = { 192,168,4,1 };
