#define GPS_TX_DIGITAL_OUT_PIN 2
#define GPS_RX_DIGITAL_OUT_PIN 3
const int AC = 19 ; //average count, scope(how many samples)
bool file_name_set = false;//has a new file name been derived from GPS date/time
bool serial_print = false; //to activate serial logging
bool log_array = false;//to include the averaging array in the serial log
bool array2file = false;//to include the averaging array in the file log
bool RPM_Graph = true;
bool OIL_Graph = true;
bool FUELP_Graph = true;
bool MAP_Graph = true;
bool ENGT_Graph = true;
bool AFR_Graph = true;
bool pcot2_Graph = false;
bool pcot3_Graph = false;
char daytime[9];

//setting up EGT Module
//int ktcSO = 8;
//int ktcCS = 9;
//int ktcCLK = 10;
//MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

//digital inputs
int RightBlinkPin = 34;   //red/blk
int LeftBlinkPin = 38;    //red/blu
int HighbeamPin = 42; //grn/blk
int OillightPin = 46; //grn/yel
int ExtraPin = 50; //not connected yet
int pcob0 = 0;
int pcob1 = 0;
int pcob2 = 0;
int pcob3 = 0;
int pcot0 = 0;
int pcot1 = 0;
int pcot2 = 0;
int pcot3 = 0;

//RPM counter variables
const byte interruptPin = 21;    //20 is ir sensor,  21 is xms5b
volatile float rpmcount;
double rpm = 0;
double rpmsaved = 0;
unsigned long timeold = 0;
//unsigned long fueltimer = millis();
double RPM_Averager = 0;
double RPMC_Averager = 0;
//double Fuel_Level_Averager =  0;
//float AFR_Averager = 0;
float VLT = 0;
char AFR_CHAR[9];

// oil Pressure Averager
const int OP_AC = 30;
double OP_Readings[OP_AC];
int OP_ArrayIndex = 0;
double OP_Total = 0;
double OP_AverageLevel = 00;
double OP_Level = 0;

// Fuel Pressure Averager
const int FP_AC = 30;
double FP_Readings[FP_AC];
int FP_ArrayIndex = 0;
double FP_Total = 0;
double FP_AverageLevel = 00;
double FP_Level = 0;


// Temp Gauge Averager
const int TG_AC = 60;
double TG_Readings[TG_AC];
int TG_ArrayIndex = 0;
double TG_Total = 1;
double TG_AverageLevel = 01;
double TG_Level = 0;

// Fuel Gauge Averager
const int FG_AC = 60;
double FG_Readings[FG_AC];
int FG_ArrayIndex = 0;
double FG_Total = 0;
double FG_AverageLevel = 00;
double FG_Level = 0;

// AFR Averager
const int AFR_AC = 15;
double AFR_Readings[AFR_AC];
int AFR_ArrayIndex = 0;
double AFR_Total = 0;
double AFR_AverageLevel = 00;
double AFR_Level = 0;



//analog inputs
//these are Pulled up through a 1k0 resistor to 5v
//Siutible for use on resistive sensors like thermistors
// pin3          fuelgauge    red/yel
// pin1          Engine temp  red/blk  A1
// pin2                       red/blu

// pin4          oc
// pin5          oc



// pin8          AFR  0~5v signal from controller A8
// pin10         Boost input from MPX4250  0~5v

String timold = "";
int boostold = 0;
int rpmold = 0;
int spdold = 0;
int afrold = 0;
int oilold = 0;
int flpold = 0;
int tempold = 0;
long timer = millis();
long odotimer = millis();
long kkkkkk = 346138;
long odoaddress = 10;
long tripaddress = 15;

double odototal;
double triptotal;
double acumilator;


//general Page-control variables
int number = 48;
String PG;
int CurrentPage = 0;
uint32_t number6 = 66;
const int chipSelect = 10;
char date_name[16] = {
  0
};


//testing with pot
int analogPin = 2;
int val = 0;

String dataString = "";
String filename;
float RPM_Array[AC];
float RPMC_Array[AC];
int ii = 1;
float AFR_Array[AC];
int iii = 1;
//int fuelcounter = 1;
//double  Fuel_Level_Array[AC];

