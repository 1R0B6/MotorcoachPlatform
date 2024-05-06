void PollUpdates(){
  currentMillis = millis();
    if(currentMillis - lastLevelCheck >= LevelFreq){
      levelCheck();
    }
    if(currentMillis - lastPowerCheck >= PowerFreq){
      //vehiclePowerState();
    }
    if(currentMillis - lastBatteriesCheck >= BattFreq){
      //BatteryState();
    }
    //Water_PSI = analogRead(WaterPSI_in);
    //Water_Current = analogRead(WaterCurrent_in);

    Water_Inside_IN = digitalRead(Water_In_Inside);
    Water_Outside_IN = digitalRead(Water_In_Outside);
    Hot_Water_IN = digitalRead(hot_water_In);
    Level_Check_In = digitalRead(level_check_in);
}