// Hackster LAB BLE9000 BCN 04/03/2017
// (c) Hackster.IO BCN
# include <CurieBLE.h>
        const int ledPin = 13; // set ledPin to use on-board LED
        char state;   // variable to hold a transmitted byte

        BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
        // ====  create Nordic Semiconductor UART service =========
        BLEService uartService = BLEService("6E400001B5A3F393E0A9E50E24DCCA9E");
        // create characteristics
        // BLECharacteristic rxCharacteristic = BLECharacteristic("6E400002B5A3F393E0A9E50E24DCCA9E", BLEWriteWithoutResponse, 20);  // == TX on central (android app)
        BLEUnsignedCharCharacteristic rxCharacteristic = BLEUnsignedCharCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWritten);
        BLECharacteristic txCharacteristic = BLECharacteristic("6E400003B5A3F393E0A9E50E24DCCA9E", BLENotify, 1); // == RX on central (android app)

        void setup() {
            Serial.begin(9600);
            pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
            // set advertised local name and service UUID:
            blePeripheral.setLocalName("BLE_ROV");
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
            }
        }
 
