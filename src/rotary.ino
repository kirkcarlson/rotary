/**************************************************************************
 * rotary -- encode a direction for a rotary control
 *
 * Uses the KY-040 library
 ***************************************************************************/ 

// **** LIBRARIES ****
#include <MQTT.h>
#include "addresses.h"
#include "button.h"
#include "encoder.h"

// **** HARDWARE PINS ****

#define BUTTON_MANUAL	A0
#define BUTTON1		A1
#define BUTTON2		A2
#define BUTTON3		A3
#define BUTTON4		A4
#define BUTTON5		A5
#define LED1		A6
#define LED2		A7
#define LED3		D0
#define LED4		D1
#define LED5		D7
#define ROTARY1		D5 // select for encoder1
#define ROTARY2		D6 // select for encoder2)
#define ENCODER1A	D2
#define ENCODER1B	D3
#define ENCODER1SW	D4
#define ENCODER2A	D2
#define ENCODER2B	D3
#define ENCODER2SW	D4

//    HEARTBEAT_PERIOD is in milliseconds
#define HEARTBEAT_PERIOD 20*1000
// KEEP_ALIVE is in seconds
#define KEEP_ALIVE 60


// **** GLOBALS ****

unsigned long  heartBeatDue = 0;


// **** MQTT FUNCTIONS ****

MQTT mqttClient( mqttServer, 1883, KEEP_ALIVE, receiveMQTT);


// for QoS2 MQTTPUBREL message.
// this messageid maybe have store list or array structure.
uint16_t qos2messageid = 0;

// MQTT receive message callback
void receiveMQTT(char* topic, byte* payload, unsigned int length) {
    #define MAX_PAYLOAD_SIZE 100
    char lastPayload [MAX_PAYLOAD_SIZE+1];

    // payload must be copied and the terminating null added
    if (length > MAX_PAYLOAD_SIZE) {
      length = MAX_PAYLOAD_SIZE;
    }
    memcpy(lastPayload, payload, length);
    lastPayload[length] = '\0';
    String topicS = String( (const char*) topic);
    String payloadS = String( (const char*) lastPayload);

    Log.trace( "Received message topic: \"" + topicS + "\" payload: \"" + payloadS + "\"");

/*
want to have a switch case statement here to handle various received topics
node/slot1/value	payload = text
*/
}


// QOS ack callback.
// if application use QOS1 or QOS2, MQTT server sendback ack message id.
void qoscallback(unsigned int messageid) {
    Log.trace( "MQTT ACK Message Id:" + messageid);

    if (messageid == qos2messageid) {
        Log.trace ( "Release MQTT QoS2 Message" + messageid);
        //mqttClient.publishRelease(qos2messageid);
    }
}


void publish (String topic, String payload) {
    #define BUFFER_LEN 100
    char topicBuffer[BUFFER_LEN];
    char payloadBuffer[BUFFER_LEN];
    topic = String( NODE_NAME) + String( "/") + topic;
    topic.toCharArray(topicBuffer, BUFFER_LEN);
    payload.toCharArray(payloadBuffer, BUFFER_LEN);
    mqttClient.publish( topicBuffer, payloadBuffer);
    Log.trace( "MQTT: " + topic + ": " + payloadBuffer);
}

void subscribe (String topic) {
#define TOPIC_LEN 100
    char subscribeTopic[TOPIC_LEN];

    topic.toCharArray( subscribeTopic, TOPIC_LEN);
    mqttClient.subscribe( subscribeTopic);
    Log.trace( "MQTT subscribe: " + String(subscribeTopic));
}


//  **** FUNCTIONS ****

void enableEncoder ( uint8_t pin) {
    digitalWrite( pin, LOW);
}

void disableEncoder ( uint8_t pin) {
    digitalWrite( pin, HIGH);
}


// **** OBJECT SETUPS ****

SerialLogHandler logHandler(LOG_LEVEL_TRACE);
SYSTEM_MODE(MANUAL);

Encoder encoder1 ( ENCODER1A, ENCODER1B, ENCODER1SW, "encoder1");
Encoder encoder2 ( ENCODER2A, ENCODER2B, ENCODER2SW, "encoder2");

Button button1 ( BUTTON1, LED1, "button1");
Button button2 ( BUTTON2, LED2, "button2");
Button button3 ( BUTTON3, LED3, "button3");
Button button4 ( BUTTON4, LED4, "button4");
Button button5 ( BUTTON5, LED5, "button5");


void setup()
{
    pinMode( BUTTON_MANUAL, INPUT_PULLUP);
    pinMode( ROTARY1, OUTPUT);
    pinMode( ROTARY2, OUTPUT);
    disableEncoder( ROTARY1);
    disableEncoder( ROTARY2);

    Serial.begin (9600);
    Log.trace ("Starting up");
}


void loop() {
    unsigned long currentTime = millis();

    if (!WiFi.ready()) {
      WiFi.on();
      WiFi.connect();
    }
    if (digitalRead (BUTTON_MANUAL) == LOW) {
      Particle.connect();
    }
    if (Particle.connected()) {
      Particle.process();
    }

    enableEncoder( ROTARY1);
    encoder1.check( currentTime);
    disableEncoder( ROTARY1);

    enableEncoder( ROTARY2);
    encoder2.check( currentTime);
    disableEncoder( ROTARY2);

    button1.check( currentTime);
    button2.check( currentTime);
    button3.check( currentTime);
    button4.check( currentTime);
    button5.check( currentTime);

    if (mqttClient.isConnected()) { // check on MQTT connection
        mqttClient.loop();  // logically this is where receiveMQTT and qoscallback are invoked
        // check heartbeat due
        if (currentTime > heartBeatDue) {
            publish ("heartbeat", "");
            heartBeatDue = currentTime + HEARTBEAT_PERIOD;
        }

    } else {
        // connect to the server
        Log.trace( "Attempting to connect to MQTT broker");
        mqttClient.connect(NODE_NAME);
        delay(10);

        // subscribe to all commands at once with wild card
        subscribe( String ( NODE_NAME) + String( "/command/#"));
    }

    //unsigned long  runTime = Time.now() - currentTime;
    //delay (max(0, 5-runTime)); // 10 debounces well, but can alias; 5 works great
}
