void debugPrints(){
  //Water | Grey | Black | Propane | Water Pump State | Water Pump Error | Water PSI | Water Current | Shore | Generator | Diesel | Coach V | Coach I | Freight V | Freight I | Temp | Humidity | CO2
  Serial.print(Water_Level);Serial.print(',');
  Serial.print(Grey_Level);Serial.print(',');
  Serial.print(Black_Level);Serial.print(',');
  Serial.print(Propane_Level);Serial.print(',');
  Serial.print(Water_Inside_IN);Serial.print(',');
  Serial.print(Water_Outside_IN);Serial.print(',');
  Serial.print(Water_Pump_State);Serial.print(',');
  Serial.print(Hot_Water_IN);Serial.print(',');
  //Serial.print(Water_Error);Serial.print(',');
  //Serial.print(Water_PSI);Serial.print(',');
  //Serial.print(Water_Current);Serial.print(',');
  //Serial.print(Shore_Power);Serial.print(',');
  //Serial.print(Generator_Power);Serial.print(',');
  //Serial.print(Diesel_Power);Serial.print(',');
  //Serial.print(Coach_Batteries);Serial.print(',');
  //Serial.print(Coach_Current);Serial.print(',');
  //Serial.print(Freight_Batteries);Serial.print(',');
  //Serial.print(Freight_Current);Serial.print(',');
  if(scd30_avail = true){
    Serial.print(scd30.temperature);Serial.print(',');
    Serial.print(scd30.relative_humidity);Serial.print(',');
    Serial.print(scd30.CO2, 3);Serial.print(',');
  }
  //Serial.print();Serial.print(',');
  Serial.println();


/*  
  Serial.print();Serial.print(',');
  Serial.print();Serial.print(',');
  Serial.print();Serial.print(',');
  Serial.print();Serial.print(',');
unsigned long Water_PSI;
unsigned long Water_Current;
unsigned long Water_Pump_Run_Time;
unsigned long Water_Pump_Run_Time_Total;

  */
}