void levelCheck(){  //Only reads in data from level check switches
  digitalWrite(Level_Check,HIGH);
  lastLevelCheck = millis();
  delay(500);
  Water_Level = analogRead(waterLevel);
  delay(10);
  Grey_Level = analogRead(greyLevel);
  delay(10);
  Black_Level = analogRead(blackLevel);
  delay(10);
  Propane_Level = analogRead(propaneLevel);
  delay(500);
  digitalWrite(Level_Check,LOW);
  if(Water_Level > minWater && Grey_Level < maxGrey && Black_Level < maxBlack){
    Tanks_OK = true;
  }
  else{
    Tanks_OK = false;
  }

  #ifdef DEBUG_ACTIVE
    Serial.print("Analog Level Check (W/G/B): ");
    Serial.print(Water_Level);Serial.print(',');
    Serial.print(Grey_Level);Serial.print(',');
    Serial.print(Black_Level);Serial.print(',');
    Serial.print(Propane_Level);Serial.print(',');
  #endif
}

void WaterPump(bool operation){
  Water_Pump_State = operation;
  digitalWrite(Water_Pump_Out,operation);
}

void WaterControl(){
  if(Water_Pump_State == false && Tanks_OK == true && Low_Voltage == false && Water_PSI > minPSI && Water_Error==false && Water_Pump_Active == true){ 
    WaterPump(true); // need to add current monitor confirmation pump turned on.
    Water_On_Time = millis();
  }
  else if(Water_Pump_State==true){ 
    Water_Current_Time = millis();
    Water_Pump_Run_Time = Water_Current_Time - Water_On_Time;
    if(Water_Pump_Run_Time > maxWaterTime){
      WaterPump(false);
      Serial.println("Water OFF");
      delay(2000);
    }
  }
  else{
    Serial.println("Conditions Not Met to Turn on Pump");
  } 
}

void WaterPumpDiagnostics(){
  // by default the sensor does continuous reading, but
  // we can set to triggered mode. to do that, we have to set
  // the mode to trigger a new reading, then wait for a conversion
  // either by checking the ALERT pin or reading the ready register
  // ina228.setMode(INA228_MODE_TRIGGERED);
  // while (!ina228.conversionReady())
  //  delay(1);
  Serial.print("Current: ");
  Serial.print(ina228.readCurrent());
  Serial.println(" mA");

  Serial.print("Bus Voltage: ");
  Serial.print(ina228.readBusVoltage());
  Serial.println(" mV");

  Serial.print("Shunt Voltage: ");
  Serial.print(ina228.readShuntVoltage());
  Serial.println(" mV");

  Serial.print("Power: ");
  Serial.print(ina228.readPower());
  Serial.println(" mW");

  Serial.print("Energy: ");
  Serial.print(ina228.readEnergy());
  Serial.println(" J");

  Serial.print("Temperature: ");
  Serial.print(ina228.readDieTemp());
  Serial.println(" *C");
}