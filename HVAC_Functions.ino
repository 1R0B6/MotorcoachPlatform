void HVAC(){ // needs more thought
    Serial.println(scd30.temperature); //degrees C
    Serial.println(scd30.relative_humidity);
    Serial.println(scd30.CO2, 3);
}