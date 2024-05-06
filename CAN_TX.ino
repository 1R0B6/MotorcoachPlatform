void heartbeat(){
  currentMillis = millis();
  if(currentMillis - lastCAN >= canFreq){
      switch(device_Type){
        case 0xF0:
          CAN.beginPacket(0xF0);
          break;
        case 0xF1:
          CAN.beginPacket(0xF1);
          break;
        case 0xF2:
          CAN.beginPacket(0xF2);
          break;
        case 0xF3:
          CAN.beginPacket(0xF3);
          break;
        case 0xF4:
          CAN.beginPacket(0xF4);
          break;
        case 0:
          CAN.beginPacket(0xF5);
          break;
      }

      CAN.write(VERSION);

      switch(device_Type){
	    case 0xF0:
          CAN.write(AUX);
          CAN.write(CAB);
          CAN.write(REAR);
          CAN.write(RPI);
		  break;
		case 0xF1:
		  CAN.write(MAIN);
          CAN.write(CAB);
          CAN.write(REAR);
          CAN.write(RPI);
		  break;
		case 0xF2:
		  CAN.write(AUX);
          CAN.write(MAIN);
          CAN.write(REAR);
          CAN.write(RPI);
		  break;
		case 0xF3:
		  CAN.write(AUX);
          CAN.write(CAB);
          CAN.write(MAIN);
          CAN.write(RPI);
		  break;
		case 0xF4:
		  CAN.write(AUX);
          CAN.write(CAB);
          CAN.write(REAR);
          CAN.write(MAIN);
		  break;
		default:
		  break;
      }

      CAN.endPacket();
	  lastCAN = millis();
  }
}

void AUX_canTX(){
  //Lighting Related
  CAN.beginPacket(0x90);
    CAN.write(Sink_Couch);
    CAN.write(Liv_Din_Ovh); 
    CAN.write(Master_Bed);
    CAN.write(Compt_Lights);
    CAN.write(Bath_Din_Window);
    CAN.write(Kit_Ovh_Bath_Sink);
  CAN.endPacket();

  //Body Control
  // CAN.beginPacket(0xA2);  //CO2 Readings
  //   CAN.write();  //Need Main CO2 Value Here
  // CAN.endPacket();

  // CAN.beginPacket(0xA3);  //Coach Level Values
  //   CAN.write();          //x Values Here
  //   CAN.write();          //y Values Here
  //   CAN.write();          //z Values Here
  // CAN.endPacket();
  
  //HVAC Related
  CAN.beginPacket(0xD1);  //Main HVAC Status
    CAN.write(Target_Temp);  //Chance Temp Request
    CAN.write(Request_AC_Status); //Request AC/Furnace
    CAN.write(Main_Temp);  //Main Temp
    CAN.write(Main_Hum);  //Main Hum
    CAN.write(Main_CO2);  //??Main CO2??
    CAN.write(Main_Fan);  //Main Fan On/OFF
    CAN.write(Main_AC);  //Main A/C On/OFF
    CAN.write(Main_Furnace);  //Main Furnace On/OFF
  CAN.endPacket();

}

void Main_canTX(){
  //Lighting Related
  CAN.beginPacket(0x91);
    CAN.write(Sink_Couch);
    CAN.write(Liv_Din_Ovh);
    CAN.write(Master_Bed);
    CAN.write(Compt_Lights);
    CAN.write(Bath_Din_Window);
    CAN.write(Kit_Ovh_Bath_Sink);
  CAN.endPacket();

  //Body Control
  // CAN.beginPacket(0xA0); //Level Values
	//   CAN.write();
	//   //CAN.write(); 
  // CAN.endPacket();

  // CAN.beginPacket(0xA1); //CO2 Values
	//   CAN.write();
	//   //CAN.write(); 
  // CAN.endPacket();


  //Power Related (Gen/Shore/Inverter/Propane)
  CAN.beginPacket(0xB0);
	  CAN.write(Propane_Level);
	  //CAN.write(); //Generator On/Off
  CAN.endPacket();

  // HVAC Related
  CAN.beginPacket(0xD0);
    CAN.write(Target_Temp);
    CAN.write(Master_Temp);
    CAN.write(Master_Hum);
    CAN.write(Master_CO2);
    CAN.write(Main_Fan);
    CAN.write(Main_AC_Furn);
    CAN.write(Master_Fan); 
    CAN.write(Master_AC);
  CAN.endPacket();

  // Water tank status
  CAN.beginPacket(0xE0);
	  CAN.write(Water_Level);
	  CAN.write(Grey_Level);
	  CAN.write(Black_Level);
	  CAN.write(Tanks_OK);
  CAN.endPacket();

  //Level Test? Water IN/Hot Water IN
  CAN.beginPacket(0xE1);
	  CAN.write(Water_Inside_IN);
	  CAN.write(Water_Outside_IN);
	  CAN.write(Hot_Water_IN);
	  CAN.write(Level_Check_In);
  CAN.endPacket();

  //Water Pump Out & Hot Water Status
  CAN.beginPacket(0xE2);
	  CAN.write(Water_Pump_Active);
	  CAN.write(Water_Pump_State);
	  CAN.write(Water_Error);
	  CAN.write(Hot_Water_Active);
	  CAN.write(Hot_Water_Gas_State);
	  CAN.write(Hot_Water_Elec_State);
	  CAN.write(Hot_Water_Error);
  CAN.endPacket();
}