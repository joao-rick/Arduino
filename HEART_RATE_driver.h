class HEART_RATE{
    Serial.print("Heart rate: ");
    Wire.requestFrom(0xA0 >> 1, 1);    // request 1 bytes from slave device

    while(Wire.available()) {          // slave may send less than requested

        uint8_t c = Wire.read();   // receive heart rate value (a byte)

        Serial.println(c, DEC);         // print heart rate value

    }
}
