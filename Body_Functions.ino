/*
void vehiclePowerState(){ //Aimed to determine current power states
  lastPowerCheck = millis();
  Shore_Power = digitalRead(shorePower);
  Generator_Power = digitalRead(generator);
  Diesel_Power = digitalRead(diesel);

  #ifdef DEBUG_ACTIVE
    Serial.print("Vehicle State Check (S/G/D): ");
    Serial.print(Shore_Power);Serial.print(',');
    Serial.print(Generator_Power);Serial.print(',');
    Serial.print(Diesel_Power);Serial.print(',');
  #endif
}
*/

/*
void BatteryState(){
  lastBatteriesCheck = millis();
  Coach_Batteries = analogRead(coachBatt);
  Freight_Batteries = analogRead(freightBatt);
  Coach_Current = analogRead(coachCurnt);
  Freight_Current = analogRead(freightCurnt);
  if(Coach_Batteries < minBatt){
    Low_Voltage = false;
  }
  else{
    Low_Voltage = true;
  }

  #ifdef DEBUG_ACTIVE
    Serial.print("Battery States (C V|I, F V|I: ");
    Serial.print(Coach_Batteries);Serial.print(',');
    Serial.print(Coach_Current);Serial.print(',');
    Serial.print(Freight_Batteries);Serial.print(',');
    Serial.print(Freight_Current);Serial.println(',');
  #endif
}
*/