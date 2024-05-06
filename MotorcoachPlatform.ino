/*
 * Adafruit Feather M4 CAN Receiver Example
 */
#include <Adafruit_INA228.h>
#include <Adafruit_SCD30.h>
#include <CANSAME5x.h>

//#define DEBUG_ACTIVE 1
///////////////////////////////////////ENSURE PROPER DEVICE TYPE IS SELECTED FOR BUILD
byte device_Type = 0xF0; //0xF0 = MAIN, 1 = AUX, 2 = CAB, 3 = REAR, 4 = RPI, 5 = DEBUGGER RAW, 6 = DEBUGGER DECODED
byte VERSION = 0x01;


#define LevelFreq 10000
#define PowerFreq 1000
#define BattFreq  1000
#define minBatt 600 //reversed due to resistor
#define minWater 350
#define maxBlack 700
#define maxGrey 700
#define minPSI 700
#define maxWaterTime 10000

//IO Definitions
  //Out
#define Water_Pump_Out 9
#define Level_Check 10

  //In
#define waterLevel A3
#define greyLevel A5
#define blackLevel A4
#define propaneLevel A2
#define Water_In_Inside 5
#define Water_In_Outside 6
#define buzzer 12
#define level_check_in 4
#define hot_water_In 11

/*
  #define shorePower 50
  #define generator 51
  #define diesel 48 
  #define coachBatt A0
  #define coachCurnt A1
  #define freightBatt A2
  #define freightCurnt A3
  #define WaterPSI_in A4
  #define WaterCurrent_in A5
*/





Adafruit_INA228 ina228 = Adafruit_INA228();
Adafruit_SCD30  scd30;
CANSAME5x CAN;
int canFreq = 1000;

//Time / Timing
unsigned long currentMillis;
unsigned long lastCAN;

//CAN
unsigned char len = 0; //move into a function?
unsigned char Rxbuff[8]; //move into a function?
unsigned char Txbuff[8];
int messageID;

//ECU Present
volatile bool MAIN = false;
  byte MAIN_SW;  //Revisit
volatile bool AUX = false;
  byte AUX_SW;
volatile bool CAB = false;
  byte CAB_SW;
volatile bool REAR = false;
  byte REAR_SW;
volatile bool RPI = false;
  byte RPI_SW;

//I2C Periph Available
bool ina228_avail = false;
bool scd30_avail = false;

//Water
volatile bool Water_Pump_Active = false;
volatile bool Water_Pump_State = false;
volatile bool Water_Error = false;
volatile bool Tanks_OK = false;
volatile bool Hot_Water_Active = false;
volatile bool Hot_Water_Gas_State = false;
volatile bool Hot_Water_Elec_State = false;
volatile bool Hot_Water_Error = false;
volatile bool Water_Inside_IN = false;
volatile bool Water_Outside_IN = false;
volatile bool Hot_Water_IN = false;
volatile bool Level_Check_In = false;
volatile unsigned long Water_Level;
volatile unsigned long Grey_Level;
volatile unsigned long Black_Level;
volatile unsigned long Propane_Level;
unsigned long lastLevelCheck;

		unsigned long Water_PSI;
		unsigned long Water_Current; //water pump current monitor?
		unsigned long Water_On_Time;
		unsigned long Water_Current_Time;
		unsigned long Water_Pump_Run_Time;
		unsigned long Water_Pump_Run_Time_Total;

 		//Power
		bool Shore_Power = false;
		bool Generator_Power = false;
		bool Diesel_Power = false;
		bool Low_Voltage = true;
		unsigned long Coach_Batteries;
		unsigned long Freight_Batteries;
		unsigned long Coach_Current;
		unsigned long Freight_Current;
		unsigned long lastPowerCheck;
		unsigned long lastBatteriesCheck;   



void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Version: ");
  Serial.println(VERSION);
  switch(device_Type){
    case 0xF0:
      Serial.println("MAIN ECU");
      break;
    case 0xF1:
      Serial.println("AUX ECU");
      break;
    case 0xF2:
      Serial.println("CAB ECU");
      break;
    case 0xF3:
      Serial.println("REAR ECU");
      break;
    case 0xF4:
      Serial.println("RPI ECU");
      break;
    case 0xF5:
      Serial.println("Debugger RAW");
      break;
    case 0xF6:
      Serial.println("Debugger Decoded");
      break;
    default:
      Serial.println("Error with ECU");
      break;  

  }


  //Configure CAN
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

  // start the CAN bus at 250 kbps
  if (!CAN.begin(250000)) {
    Serial.println("Starting CAN failed!");
    while (1) delay(10);
  }
  #ifdef DEBUG_ACTIVE
    Serial.println("CAN Started!");
  #endif

  //Configure I2C Periphials

  //SCD30 Temp/Hum/CO2
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    scd30_avail = false;
	#ifdef DEBUG_ACTIVE
      Serial.println("No SCD30 Found, setting to false");
    #endif
  }
  else{
    scd30_avail = true;
	#ifdef DEBUG_ACTIVE
      Serial.println("SCD30 Found!");
      Serial.print("Measurement Interval: "); 
      Serial.print(scd30.getMeasurementInterval()); 
      Serial.println(" seconds");
    #endif
  }

  // if (!scd30.setMeasurementInterval(10)){
  //   Serial.println("Failed to set measurement interval");
  //   while(1){ delay(10);}
  // }

  //INA228 Voltage /Current/Power
  if (!ina228.begin()) {
    Serial.println("Couldn't find INA228 chip, setting to false.");
    ina228_avail = false;
  }
  else{
    ina228_avail = true;
    #ifdef DEBUG_ACTIVE
      Serial.println("Found INA228 chip");
    #endif
  
    // we need to set the resistance (default 0.1 ohm) and our max expected
    // current (no greater than 3.2A)
    ina228.setShunt(0.1, 1.0);
  
    ina228.setAveragingCount(INA228_COUNT_16);
    uint16_t counts[] = {1, 4, 16, 64, 128, 256, 512, 1024};
      #ifdef DEBUG_ACTIVE
        Serial.print("Averaging counts: ");
        Serial.println(counts[ina228.getAveragingCount()]);
      #endif
    // set the time over which to measure the current and bus voltage
    ina228.setVoltageConversionTime(INA228_TIME_150_us);
      #ifdef DEBUG_ACTIVE
        Serial.print("Voltage conversion time: ");
        switch (ina228.getVoltageConversionTime()) {
        case INA228_TIME_50_us:
          Serial.print("50");
          break;
        case INA228_TIME_84_us:
          Serial.print("84");
          break;
        case INA228_TIME_150_us:
          Serial.print("150");
          break;
        case INA228_TIME_280_us:
          Serial.print("280");
          break;
        case INA228_TIME_540_us:
          Serial.print("540");
          break;
        case INA228_TIME_1052_us:
          Serial.print("1052");
          break;
        case INA228_TIME_2074_us:
          Serial.print("2074");
          break;
        case INA228_TIME_4120_us:
          Serial.print("4120");
          break;
        }
        Serial.println(" uS");
      #endif
  
    ina228.setCurrentConversionTime(INA228_TIME_280_us);
      #ifdef DEBUG_ACTIVE
        Serial.print("Current conversion time: ");
        switch (ina228.getCurrentConversionTime()) {
        case INA228_TIME_50_us:
          Serial.print("50");
          break;
        case INA228_TIME_84_us:
          Serial.print("84");
          break;
        case INA228_TIME_150_us:
          Serial.print("150");
          break;
        case INA228_TIME_280_us:
          Serial.print("280");
          break;
        case INA228_TIME_540_us:
          Serial.print("540");
          break;
        case INA228_TIME_1052_us:
          Serial.print("1052");
          break;
        case INA228_TIME_2074_us:
          Serial.print("2074");
          break;
        case INA228_TIME_4120_us:
          Serial.print("4120");
          break;
        }
        Serial.println(" uS");
      #endif
    
    // default polarity for the alert is low on ready, but
    // it can be inverted!
    // ina228.setAlertPolarity(1);
  }
   
  /*
  //INTERRUPTS
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), HMI_Pressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), HMI_Released, RISING);
  */

  //Configure Device Input/Outputs
    switch(device_Type){
	  case 0xF0:										//Main ECU
        pinMode(Water_In_Inside, INPUT_PULLUP);
        pinMode(Water_In_Outside, INPUT_PULLUP);
        pinMode(hot_water_In, INPUT_PULLUP);
        pinMode(level_check_in, INPUT_PULLUP);

        pinMode(propaneLevel, INPUT_PULLUP);
        pinMode(waterLevel, INPUT_PULLUP); 
        pinMode(greyLevel, INPUT_PULLUP);
        pinMode(blackLevel, INPUT_PULLUP);

      //OUTPUTS
        //RELAYS
        pinMode(Water_Pump_Out,OUTPUT);
          digitalWrite(Water_Pump_Out,HIGH);
        pinMode(Level_Check,OUTPUT);
          digitalWrite(Level_Check,HIGH);
        delay(2000);
        digitalWrite(Water_Pump_Out,LOW);
        digitalWrite(Level_Check,LOW);
      
        levelCheck();
		
		break;
	  case 0xF1:									  //AUX ECU
	    //Need to configure Relay Outputs here
		break;
	  case 0xF2:									  //CAB ECU
	    //Need to configure Relay Outputs here
		break;
	  case 0xF3:									  //REAR ECU
	    //Need to configure Relay Outputs here
		break;
	  case 0xF4:									  //RPI ECU
	    //Need to configure Relay Outputs here
		break;
	  default:
		  break;
    }

  //vehiclePowerState();




    /*
    pinMode(shorePower, INPUT_PULLUP);
    pinMode(generator, INPUT_PULLUP);
    pinMode(diesel, INPUT_PULLUP);
    pinMode(coachBatt, INPUT_PULLUP);
    pinMode(coachCurnt, INPUT_PULLUP);
    pinMode(freightBatt, INPUT_PULLUP);
    pinMode(freightCurnt, INPUT_PULLUP);
    pinMode(WaterPSI_in, INPUT_PULLUP);
    pinMode(WaterCurrent_in, INPUT_PULLUP);
  */
}

void loop() {
  Serial.print("AUX Preset = "); Serial.println(AUX);
  heartbeat();
  // if(scd30_avail = true){
  //   HVAC();
  // }

  // if(ina228_avail = true){
  //   WaterPumpDiagnostics();
  // }
  
  switch(device_Type){
    case 0xF0:				//MAIN ECU
	    PollUpdates();
      WaterControl();
      Main_canTX();
	  #ifdef  LIVE_VALUES
        debugPrints();
      #endif
	  break;
	case 0xF1:				//AUX ECU
	  
	  break;
	case 0xF2:				//CAB ECU
	  
	  break;
	case 0xF3:				//REAR ECU
	  
	  break;
	case 0xF4:				//RPI ECU
	  
	  break;
	case 0xF5:				//DEBUGGER RAW
	  print_RAW_CAN();
	  break;
	case 0xF6:				//DEBUGGER DECODED
	  decodeCAN(true);
	  break;
	default:
	  Serial.println("Error in Device Address");
	  break;
  }
  decodeCAN(false);
  //canRX(); stripdown
  delay(1000);
}