void decodeCAN(bool SerialPrints){
  int packetSize = CAN.parsePacket();
  if (packetSize) { //received packet
    messageID = CAN.packetId(), HEX;
    #ifdef DEBUG_ACTIVE
	  Serial.print(messageID);
      Serial.print("|");
	#endif

    if (CAN.packetRtr()) {
      Serial.print("RTR requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      #ifdef DEBUG_ACTIVE
	    Serial.print(packetSize);
        Serial.print("|");
	  #endif
      for(int i = 0; i<packetSize; i++){
        Rxbuff[i] = CAN.read();
      }
      // while (CAN.available()) {
      //   // Serial.print((char)CAN.read());
      //   PACKAGE(CAN.read());
      // }
      // only print packet data for non-RTR packets
      switch(messageID){
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

    //Serial.println();
  }
}

void canRX(){
  int packetSize = CAN.parsePacket();
  if (packetSize) { //received packet
    messageID = CAN.packetId(), HEX;
    Serial.print(messageID);
    Serial.print("|");
    Serial.print(packetSize);
    Serial.print("|");
    for(int i = 0; i<packetSize; i++){
      Rxbuff[i] = CAN.read();
    }
    
    switch(messageID){
      case 241: //F1 Message
        AUX = true;
        break;
      case 242: //F2 Message
        CAB = true;
        break;
      case 243: //F3 Message
        REAR = true;
        break;
      case 244:	//F4 Message
        RPI = true;
        break;
      case 0:
        break;
    }
    Serial.println();
  }
}

void print_RAW_CAN(){					//For 0xF5 RAW CAN Only
  int packetSize = CAN.parsePacket();
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