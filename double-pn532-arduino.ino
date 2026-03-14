#include <SoftwareSerial.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <NfcAdapter.h>

#define READER_TIMEOUT 100

// Reader 1
SoftwareSerial SWSerial1(3, 2); // TX, RX
PN532_SWHSU pn532swhsu1(SWSerial1);
PN532 nfc1(pn532swhsu1);

// Reader 2
PN532_I2C pn532i2c(Wire);
PN532 nfc2(pn532i2c);

void setup(void)
{
  Serial.begin(115200);
  SWSerial1.begin(115200);
  Wire.begin();

  Serial.println("STARTING");

  initReader(1, &nfc1);
  initReader(2, &nfc2);
}


void loop() {
  readNFC(1, &nfc1);
  readNFC(2, &nfc2);
}

void initReader(uint8_t reader, PN532* nfc) {
  nfc->begin();

  while (1) {
    uint32_t versiondata = nfc->getFirmwareVersion();
    if (!versiondata)
    {
      Serial.print("READER_");
      Serial.print(reader);
      Serial.println("_NOT_FOUND");
      delay(5000);
    }
    else {
      break;
    }
  }

  Serial.print("READER_");
  Serial.print(reader);
  Serial.println("_FOUND");

  nfc->SAMConfig();
}

void readNFC(uint8_t reader, PN532* nfc) {
  uint8_t idm[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t pmmBuf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint16_t sysCodeResp = 0;

  if (nfc->felica_Polling(0xFFFF, 0x00, idm, pmmBuf, &sysCodeResp, READER_TIMEOUT)) {
    if (idm[0] == 0 && idm[1] == 0 && idm[2] == 0 && idm[3] == 0 && idm[4] == 0 && idm[5] == 0 && idm[6] == 0 && idm[7] == 0) {
      uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
      uint8_t uidLength;
      if (nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, READER_TIMEOUT)) {
        Serial.print("CARD_");
        Serial.print(reader);
        Serial.print(" MIFARE ");
        for (uint8_t i = 0; i < uidLength; i++) {
          if (uid[i] < 0x10) {
            Serial.print('0');
          }
          Serial.print(uid[i], HEX);
        }
        Serial.println();
      }
      else {
        Serial.print("NO_CARD_");
        Serial.println(reader);
      }
    }
    else {
      Serial.print("CARD_");
      Serial.print(reader);
      Serial.print(" FELICA ");
      for (uint8_t i = 0; i < 8; i++) {
        if (idm[i] < 0x10) {
          Serial.print('0');
        }
        Serial.print(idm[i], HEX);
      }
      Serial.println();
    }
  }
}