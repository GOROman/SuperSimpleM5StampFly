#pragma once
#define BLE_DEVICE_NAME "STAMPFLY"

// イベント種別
enum BLE_EVENT {
    BLE_EVENT_CONNECTED,
    BLE_EVENT_DISCONNECTED,
    BLE_EVENT_RECEIVED,
};

// イベントパラメータ
typedef struct {
    BLE_EVENT event;
    uint32_t   data;    // 仮
} BLEEventParam_t;

// イベントコールバック関数
typedef void (*BLECallback_t)(BLEEventParam_t* param);

static BLECallback_t _ble_callback;

#if BLE_CENTRAL

static TaskHandle_t _bleTaskHandle = NULL;  // タスクハンドル

static BLEClient* pClient = NULL;
static BLEAdvertisedDevice* targetDevice = NULL;
static BLERemoteCharacteristic* pRemoteCharacteristic = NULL;


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        USBSerial.println(advertisedDevice.toString().c_str());
//        USBSerial.println(advertisedDevice.getName().c_str());

        if (advertisedDevice.getName() == BLE_DEVICE_NAME) {
            USBSerial.println(advertisedDevice.getAddress().toString().c_str());
            advertisedDevice.getScan()->stop();
            targetDevice = new BLEAdvertisedDevice(advertisedDevice);
        }
    }
};


void _BLE_Scan_Task(void* params) {
    USBSerial.println("BLE: Scan...");

    // BLE スキャン
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30, false);

    _bleTaskHandle = NULL;
    vTaskDelete(NULL);
}

// BLE初期化
void BLE_init() {
    USBSerial.println("BLE_CENTRAL");

    BLEDevice::init(BLE_DEVICE_NAME);
}

// BLE接続開始
void BLE_connect()
{
    if ( _bleTaskHandle == NULL ) {
        xTaskCreatePinnedToCore(_BLE_Scan_Task, "BLE Scan Task", 2048, NULL, 1,
                                &_bleTaskHandle, 1);
    }
}

static void _BLE_check() {
    if (targetDevice) {
        if (pClient == NULL) {
            pClient = BLEDevice::createClient();
            pClient->connect(targetDevice);

            BLERemoteService* pRemoteService = pClient->getService("0000");
            pRemoteCharacteristic = pRemoteService->getCharacteristic("0001");
        }
    }
}

// BLE更新
void BLE_update(int value) {
    _BLE_check();
    if (pRemoteCharacteristic) {
        // キャラクタリスティックにデータを書き込む
        pRemoteCharacteristic->writeValue((uint8_t*)&value, sizeof(value));
    }

}

// BLE接続中か？
bool BLE_isConnected() {
    _BLE_check();
    return pClient != NULL;
}

#endif

#if BLE_PERIPHERAL


BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool advertising = false;

class BLECallbacks: public BLEServerCallbacks, public BLECharacteristicCallbacks {
    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
        {
            BLEEventParam_t param;
            param.event = BLE_EVENT_CONNECTED;        
            _ble_callback( &param );
        }
    }

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
        advertising = false;
        {
            BLEEventParam_t param;
            param.event = BLE_EVENT_DISCONNECTED;        
            _ble_callback( &param );
        }

    }

    void onRead(BLECharacteristic *pCharacteristic) override {
    }

    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string value = pCharacteristic->getValue();
        if (value.length() == 4) {
            uint32_t number = (static_cast<uint8_t>(value[3]) << 24) |
                              (static_cast<uint8_t>(value[2]) << 16) |
                              (static_cast<uint8_t>(value[1]) << 8) |
                              (static_cast<uint8_t>(value[0]));
            
            {
                BLEEventParam_t param;
                param.event = BLE_EVENT_RECEIVED;        
                param.data = number;
                _ble_callback( &param );
            }

            USBSerial.println(number);
        } else {
            USBSerial.println("...");
        } 
   }
};

BLECallbacks bleCallbacks;


void BLE_init(BLECallback_t callback) {
    _ble_callback = callback;
    USBSerial.println("BLE_PERIPHERAL");
    BLEDevice::init(BLE_DEVICE_NAME);


        // Create BLE Server
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(&bleCallbacks);

        // Create BLE Service
        BLEService* pService = pServer->createService("0000");

        // Create BLE Characteristic
        pCharacteristic = pService->createCharacteristic(
            "0001",
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE
        );

        pCharacteristic->setCallbacks(&bleCallbacks);

        // Start the service
        pService->start();
        USBSerial.println("pService->start()");

}
void BLE_connect()
{
}

void BLE_send(uint8_t* data, uint8_t len) {
    if (pCharacteristic) {
        pCharacteristic->setValue(data, len);
        pCharacteristic->notify();
    }
}

void BLE_update() {
    if( deviceConnected == false ) {
        if (advertising == false) {
            // Start advertising
            advertising = true;

            BLEAdvertising* pAdvertising = pServer->getAdvertising();
            pAdvertising->start();
            USBSerial.println("pAdvertising->start()");
        }
    }

}

#endif
