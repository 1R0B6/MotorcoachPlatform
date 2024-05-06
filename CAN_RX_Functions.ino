void decodeCAN(bool SerialPrints){
  int packetSize = CAN.parsePacket();
  if (packetSize) {     //received packet
    messageID = CAN.packetId(), HEX;
    #ifdef DEBUG_ACTIVE
	    Serial.print(messageID);
      Serial.print("|");
	  #endif

    #ifdef DEBUG_ACTIVE
      Serial.print(packetSize);
      Serial.print("|");
    #endif
    
    for(int i = 0; i<packetSize; i++){
      Rxbuff[i] = CAN.read();
    }

    switch(messageID){
      case 0x90:  //90 AUX Lighting Commands
        AUX_Sink_Couch = Rxbuff[0];
        AUX_Liv_Din_Ovh = Rxbuff[1]; 
        AUX_Master_Bed = Rxbuff[2];
        AUX_Compt_Lights = Rxbuff[3];
        AUX_Bath_Din_Window = Rxbuff[4];
        AUX_Kit_Ovh_Bath_Sink = Rxbuff[5];
        if(SerialPrints == true){
          Serial.println();
          Serial.print("Sink/Couch = ");
          Serial.print(AUX_Sink_Couch);
          Serial.print(" Liv/Din Ovh = ");
          Serial.print(AUX_Liv_Din_Ovh);
          Serial.print(" Master Bed = ");
          Serial.print(AUX_Master_Bed);
          Serial.print(" Compartment Lights = ");
          Serial.print(AUX_Compt_Lights);
          Serial.print(" Bath & Din Window = ");
          Serial.print(AUX_Bath_Din_Window);
          Serial.print(" Kitchen Ovh & Bath Sink = ");
          Serial.println(AUX_Kit_Ovh_Bath_Sink);
        }
        break;

      case 0x91:  //91 Main Lighting Commands
        Sink_Couch = Rxbuff[0];
        Liv_Din_Ovh = Rxbuff[1]; 
        Master_Bed = Rxbuff[2];
        Compt_Lights = Rxbuff[3];
        Bath_Din_Window = Rxbuff[4];
        Kit_Ovh_Bath_Sink = Rxbuff[5];
        if(SerialPrints == true){
          Serial.println();
          Serial.print("Sink/Couch = ");
          Serial.print(Sink_Couch);
          Serial.print(" Liv/Din Ovh = ");
          Serial.print(Liv_Din_Ovh);
          Serial.print(" Master Bed = ");
          Serial.print(Master_Bed);
          Serial.print(" Compartment Lights = ");
          Serial.print(Compt_Lights);
          Serial.print(" Bath & Din Window = ");
          Serial.print(Bath_Din_Window);
          Serial.print(" Kitchen Ovh & Bath Sink = ");
          Serial.println(Kit_Ovh_Bath_Sink);
        }
        break;

      case 0xD0: //Main A/C Signals
        Target_Temp = Rxbuff[0];
        Master_Temp = Rxbuff[1];
        Master_Hum = Rxbuff[2];
        Master_CO2 = Rxbuff[3];
        Main_Fan = Rxbuff[4];
        Main_AC_Furn = Rxbuff[5];
        switch(Main_AC_Furn){
          case 0x00:
            Main_AC = false;
            Main_Furnace = false;
            break;
          case 0xAC:
            Main_AC = true;
            Main_Furnace = false;
            break;
          case 0xF0:
            Main_AC = false;
            Main_Furnace = true;
            break;
          default:
            Serial.print("Issue with Main_AC_Furn value ");
            Serial.println(Main_AC_Furn);
            break;
        }
        Master_Fan = Rxbuff[6]; 
        Master_AC = Rxbuff[7];
        break;

      case 0xD1:
        Req_Temp = Rxbuff[0];
        Request_AC_Status = Rxbuff[1];
        AUX_Main_Temp = Rxbuff[2];
        AUX_Main_Hum = Rxbuff[3];
        AUX_Main_CO2 = Rxbuff[4];
        Aux_Main_Fan = Rxbuff[5];
        AUX_Main_AC = Rxbuff[6];
        AUX_Main_Furnace = Rxbuff[7];
        if(SerialPrints == true){
          Serial.println();
          Serial.print("Requested Temp = ");
          Serial.print(Req_Temp);
          Serial.print(" Request A/C = ");
          Serial.print(Request_AC_Status);
          Serial.print(" Aux Main Temp = ");
          Serial.print(AUX_Main_Temp);
          Serial.print(" AUX Main Hum = ");
          Serial.print(AUX_Main_Hum);
          Serial.print(" AUX Main CO2 = ");
          Serial.print(AUX_Main_CO2);
          Serial.print(" AUX Main Fan = ");
          Serial.print(Aux_Main_Fan);
          Serial.print(" AUX Main AC = ");
          Serial.print(AUX_Main_AC);
          Serial.print(" AUX Main Furn = ");
          Serial.print(AUX_Main_Furnace);
        }
        break;

      case 0xE0:	//E0 Message
        Water_Level = Rxbuff[0];
        Grey_Level = Rxbuff[1];
        Black_Level = Rxbuff[2];
        Tanks_OK = Rxbuff[3];
        if(SerialPrints == true){
          Serial.println();
          Serial.print("Water = ");
          Serial.print(Water_Level);
          Serial.print(" Grey Level = ");
          Serial.print(Grey_Level);
          Serial.print(" Black Level = ");
          Serial.print(Black_Level);
          Serial.print(" Tanks Ok = ");
          Serial.println(Tanks_OK);
        }
        break;
      case 0xE1:	//E1 Message
        Water_Inside_IN = Rxbuff[0];
        Water_Outside_IN = Rxbuff[1];
        Hot_Water_IN = Rxbuff[2];
        Level_Check_In = Rxbuff[3];
        if(SerialPrints == true){
          Serial.println();
          Serial.print("Water In In = ");
          Serial.print(Water_Inside_IN);
          Serial.print(" Water Out In = ");
          Serial.print(Water_Outside_IN);
          Serial.print(" How Water In = ");
          Serial.print(Hot_Water_IN);
          Serial.print(" Level Check ");
          Serial.println(Level_Check_In);
        }
        break;
      case 0xE2:  //E2 Message
        Water_Pump_Active = Rxbuff[0];
        Water_Pump_State = Rxbuff[1];
        Water_Error = Rxbuff[2];
        Hot_Water_Active = Rxbuff[3];
        Hot_Water_Gas_State = Rxbuff[4];
        Hot_Water_Elec_State = Rxbuff[5];
        Hot_Water_Error = Rxbuff[6];
        if(SerialPrints == true){
          Serial.print("Water Active | State | Error | Hot Water Active | Gas | Elec | Error ");
          Serial.print(Water_Pump_Active);
          Serial.print("|");
          Serial.print(Water_Pump_State);
          Serial.print("|");
          Serial.print(Water_Error);
          Serial.print("|");
          Serial.print(Hot_Water_Active);
          Serial.print("|");
          Serial.print(Hot_Water_Gas_State);
          Serial.print("|");
          Serial.print(Hot_Water_Elec_State);
          Serial.print("|");
          Serial.println(Hot_Water_Error);
        }
        break;
      case 0xF0:	//F0 Message
        MAIN_SW = Rxbuff[0];
        MAIN = true;
        //AUX = Rxbuff[1];
        //CAB = Rxbuff[2];
        //REAR = Rxbuff[3];
        //RPI = Rxbuff[4];
        if(SerialPrints == true){
          Serial.print("Main SW - ");
          Serial.print(MAIN_SW);
          Serial.print(" Aux - ");
          Serial.print(AUX);
          Serial.print(" CAB - ");
          Serial.print(CAB);
          Serial.print(" Rear - ");
          Serial.print(REAR);
          Serial.print(" RPi - ");
          Serial.println(RPI);
        }
        break;
      case 0xF1:	//F1 Message
        AUX_SW = Rxbuff[0];
        AUX = true;
        break;
      case 0xF2:	//F1 Message
        CAB_SW = Rxbuff[0];
        CAB = true;
        break;
      case 0xF3:	//F1 Message
        REAR_SW = Rxbuff[0];
        REAR = true;
        break;
      case 0xF4:	//F1 Message
        RPI_SW = Rxbuff[0];
        RPI = true;
        break;
      case 0:
        break;
    }
  }
  for(int i=0; i <= 7; i++){
    Rxbuff[i] = 0; //To protect against false data.
  }
}

void print_RAW_CAN(){					//For 0xF5 RAW CAN Only
  int packetSize = CAN.parsePacket();
  if(Debug_Message_Focus_Active ==true){
      messageID = CAN.packetId(), HEX;
      if(messageID == Debug_Message_Focus){
        Serial.print("0x");
        Serial.print(CAN.packetId(), HEX);
        Serial.print("|");
        Serial.print(packetSize);
        Serial.print("|");
        while (CAN.available()) {
          // Serial.print((char)CAN.read());
          Serial.print(CAN.read());
        }
        Serial.println();
      }
  }
  else{
    if (packetSize) { //received packet
      Serial.print("0x");
      Serial.print(CAN.packetId(), HEX);
      Serial.print("|");
      Serial.print(packetSize);
      Serial.print("|");
      while (CAN.available()) {
        // Serial.print((char)CAN.read());
        Serial.print(CAN.read());
      }
      Serial.println();
    }
  }
}