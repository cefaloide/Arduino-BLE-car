// Hackster LAB BLE9000 BCN 04/03/2017
// (c) Hackster.IO BCN
# include <CurieBLE.h>

//MotorShield required libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

const int ledPin = 13; // set ledPin to use on-board LED
char state;   // variable to hold a transmitted byte

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
// ====  create Nordic Semiconductor UART service =========
BLEService uartService = BLEService("6E400001B5A3F393E0A9E50E24DCCA9E");
// create characteristics
// BLECharacteristic rxCharacteristic = BLECharacteristic("6E400002B5A3F393E0A9E50E24DCCA9E", BLEWriteWithoutResponse, 20);  // == TX on central (android app)
BLEUnsignedCharCharacteristic rxCharacteristic = BLEUnsignedCharCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWritten);
BLECharacteristic txCharacteristic = BLECharacteristic("6E400003B5A3F393E0A9E50E24DCCA9E", BLENotify, 1); // == RX on central (android app)

//Create the Adafruit_MotorShield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//Create the DC motor object
Adafruit_DCMotor *myMotorM1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorM2 = AFMS.getMotor(2);
	
void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
	// set advertised local name and service UUID:
	blePeripheral.setLocalName("Arduino_BLE_car");
	blePeripheral.setAdvertisedServiceUuid(uartService.uuid());
	// add service, rx and tx characteristics:
	blePeripheral.addAttribute(uartService);
	blePeripheral.addAttribute(rxCharacteristic);
	blePeripheral.addAttribute(txCharacteristic);
	// assign event handlers for connected, disconnected to peripheral
	blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
	blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
	// assign event handler for characteristic
	rxCharacteristic.setEventHandler(BLEWritten, rxCharacteristicWritten);
	// advertise the service
	blePeripheral.begin();
	
	//Connect to the Controller
	AFMS.begin();
	//Set default speed
	myMotorM1->setSpeed(100); 
	myMotorM2->setSpeed(100); 
}

void loop() {
	// poll ble peripheral
	blePeripheral.poll();
}

void blePeripheralConnectHandler(BLECentral & central) {
	// central connected event handler
	Serial.print("Connected event, central: ");
	Serial.println(central.address());
	//Serial.println("LED on");
	digitalWrite(ledPin, HIGH);
}

void blePeripheralDisconnectHandler(BLECentral & central) {
	// central disconnected event handler
	Serial.print("Disconnected event, central: ");
	Serial.println(central.address());
	//Serial.println("LED off");
	digitalWrite(ledPin, LOW);
}

void rxCharacteristicWritten(BLECentral & central, BLECharacteristic & characteristic) {
	// central wrote new value to characteristic, update LED
	Serial.print("Characteristic event, written: ");
	if (characteristic.value())
	{       //null pointer check
		state = *characteristic.value();  //set state to be the value written from the phone/tablet to the Arduino 101
		Serial.println(char(state));      //print out the character to the serial monitor
		switch(state){
			case "f": //Move forward
				myMotorM1->run(FORWARD);
				myMotorM2->run(FORWARD);
				break;
			case "b": //Move backward
				myMotorM1->run(BACKWARD);
				myMotorM2->run(BACKWARD);
				break;
			case "l": //Move left
				myMotorM2->run(FORWARD);
				break;
			case "r": //Move right
				myMotorM1->run(FORWARD);
				break;
			case "s": //Stop motors
				myMotorM1->run(RELEASE);
				myMotorM2->run(RELEASE);
				break;
		}
	}
}
 
