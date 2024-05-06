void HVAC(){ // needs to determine I/O status for all A/C Furnace Variables
    //TBD
    switch(Main_AC_Status){
        //0xAC == AC 0xFA == FAN Only  0xF1 == Furnace && Fans
        case 0x00:  //All OFF
            Master_Fan = false;
            Master_AC = false;
            Main_AC = false;
            Main_Fan = false;
            Main_Furnace = false;
            Main_AC_Furn = 0x00; 
            break;

        case 0xAC:  //A/C with auto fan control
            Main_Furnace = false;
            if(Main_Temp>Target_Temp){ //Main A/C Control
                Main_Fan = true;
                Main_AC = true;
                Main_AC_Furn = 0xAC;
            }
            else{
                Main_Fan = false;
                Main_AC = false;
                Main_AC_Furn = 0x00;
            }            
            if(Master_Temp>Target_Temp){ //Master A/C Control
                Master_Fan = true;
                Master_AC = true;
            }
            else{
                Master_Fan = false;
                Master_AC = false;
            }
            break;
            
        case 0xA1:  //A/C with Fans On always
            Master_Fan = true;
            Main_Fan = true;
            Main_Furnace = false;
            if(Main_Temp>Target_Temp){ //Main A/C Control
                Main_Fan = true;
                Main_AC = true;
                Main_AC_Furn = 0xAC;
            }
            else{
                Main_AC = false;
                Main_AC_Furn = 0x00;
            }            
            if(Master_Temp>Target_Temp){ //Master A/C Control
                Master_Fan = true;
                Master_AC = true;
            }
            else{
                Master_AC = false;
            }
            break;

        case 0xF0:  //Furnace Active Master A/C to be deactive
            Master_Fan = false;
            Master_AC = false;
            Main_AC = false;
            Main_Fan = false;
            if(Main_Temp<Target_Temp || Master_Temp<Target_Temp){
                Main_Furnace = true;
                Main_AC_Furn = 0xF0;
            }
            else{
                //Need logic to debounce the off
                Main_Furnace = false;
                Main_AC_Furn = 0x00;
            }
            break;

        case 0xF1:  //Furnace & Fans
            Master_Fan = true;
            Master_AC = false;
            Main_AC = false;
            Main_Fan = true;
            if(Main_Temp<Target_Temp || Master_Temp<Target_Temp){
                Main_Furnace = true;
                Main_AC_Furn = 0xF0;
            }
            else{
                //Need logic to debounce the off
                Main_Furnace = false;
                Main_AC_Furn = 0x00;
            }
            break;

        case 0xFA:  //FANS Only
            Master_Fan = true;
            Master_AC = false;
            Main_AC = false;
            Main_Fan = true;
            Main_Furnace = false;
            break;
        
        default:
            Serial.print("Error with Main_AC_Status");
            Serial.println(Main_AC_Status);
            break;
    }
    // Serial.println(scd30.temperature); //degrees C
    // Serial.println(scd30.relative_humidity);
    // Serial.println(scd30.CO2, 3);
}

void Main_HVAC(){
    //Needs to pull Temp Data to Local Variables
    if(scd30_avail == true){
        if(scd30.dataReady()){
            //Serial.println(scd30.temperature, HEX); //degrees C
            //Serial.println(scd30.relative_humidity, HEX);
            // Serial.println(scd30.CO2, 3, HEX);


            Master_Temp = scd30.temperature;
            Master_Hum = scd30.relative_humidity;
            Master_CO2 = scd30.CO2, 3;
            Serial.println("Variables Updated!");
        }
        Serial.print("T = ");
        Serial.print(Master_Temp);
        Serial.print(" H = ");
        Serial.print(Master_Hum);
        Serial.print(" CO2 = ");
        Serial.println(Master_CO2);
    }
    else{
        Master_Temp = 69;
        Master_Hum = 69;
        Master_CO2 = 69;
    }

    //Needs to output relays based off of HVAC logic
    digitalWrite(mas_ac_out,Master_AC);
    digitalWrite(mas_fan_out,Master_Fan);
}

void AUX_HVAC(){
    //Needs to pull Temp Data to Local Variables
    if(scd30_avail == true){
        if(scd30.dataReady()){
            Main_Temp = scd30.temperature;
            Main_Hum = scd30.relative_humidity;
            Main_CO2 = scd30.CO2, 3;
            Serial.println("Variables Updated!");
        }
        Serial.print("T = ");
        Serial.print(Main_Temp);
        Serial.print(" H = ");
        Serial.print(Main_Hum);
        Serial.print(" CO2 = ");
        Serial.println(Main_CO2);
    }
    else{
        Main_Temp = 0;
        Main_Hum = 0;
        Main_CO2 = 0;
    }
    Request_AC_Status = Main_AC_Status;

    //Needs to output relays based off of CAN input
    if(Main_AC_Status == 0x00)
    {
        digitalWrite(ac_out,false);
        digitalWrite(furn_out,false);
        digitalWrite(fan_out,false);
    }
    else{
        digitalWrite(ac_out,Main_AC);
        digitalWrite(furn_out,Main_Furnace);
        digitalWrite(fan_out,Main_Fan);
    }
}