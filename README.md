Bluetooth and RFID-controlled STM32 smart door lock.
- Developed on STM32 NUCLEO-F070RB

Peripherals:
1. 1306OLED Display - I2C
2. 24LC256 EEPROM - I2C
3. TMP102 temperature sensor - I2C
4. MFRC522 RFID - SPI
5. HM-10 BLE - UART

Features:
- Control using up to RFID cards
- Save up to 2 RFID cards
- Control using BLE
- Make 5 wrong attempts before getting locked out and having to wait

![Doorlock Pinout](https://github.com/user-attachments/assets/6221a0f2-9ecb-44b5-9560-efbe94fa672e)
