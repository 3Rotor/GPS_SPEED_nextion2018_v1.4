#include <EEPROM.h>
#include <Nextion.h>
#include <TinyGPS.h>
//#include <SD.h>
#include <SoftwareSerial.h>
//#include "max6675.h"
#include "Vars.h"
#include "Warning_Indicators.h"
#include "SetTime.h"
#include "EndCmd.h"
#include "SetAfr.h"
#include "SetBst.h"
#include "SetFuelPressure.h"
#include "SetOilPressure.h"
#include "SetTempGauge.h"
#include "SetFuelGauge.h"
#include "SetRPM.h"
#include "SetSpeed.h"
#include "SetCharts.h"
#include "GPS_Loops.h"

// a handy asci to hex convertor page
// https://www.rapidtables.com/convert/number/ascii-hex-bin-dec-converter.html


//start up the GPS module
TinyGPS gps;
static void gpsdump(TinyGPS &gps);


/*
  ░░░░░░░░░░░░░░░░░░░░░░█████████
  ░░███████░░░░░░░░░░███▒▒▒▒▒▒▒▒███
  ░░█▒▒▒▒▒▒█░░░░░░░███▒▒▒▒▒▒▒▒▒▒▒▒▒███
  ░░░█▒▒▒▒▒▒█░░░░██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ░░░░█▒▒▒▒▒█░░░██▒▒▒▒▒██▒▒▒▒▒▒██▒▒▒▒▒███
  ░░░░░█▒▒▒█░░░█▒▒▒▒▒▒████▒▒▒▒████▒▒▒▒▒▒██
  ░░░█████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ░░░█▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒▒▒██
  ░██▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒██▒▒▒▒▒▒▒▒▒▒██▒▒▒▒██
  ██▒▒▒███████████▒▒▒▒▒██▒▒▒▒▒▒▒▒██▒▒▒▒▒██
  █▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒████████▒▒▒▒▒▒▒██
  ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ░█▒▒▒███████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ░██▒▒▒▒▒▒▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█
  ░░████████████░░░█████████████████
*/
void setup()
{
	for (int i = 0; i < AC; i++)RPM_Array[i] = 0;

	pinMode(RightBlinkPin, INPUT); //red/blk
	pinMode(LeftBlinkPin, INPUT); //red/blu
	pinMode(HighbeamPin, INPUT); //grn/blk
	pinMode(OillightPin, INPUT); //grn/yel
	pinMode(ExtraPin, INPUT); //grn/yel

	pinMode(interruptPin, INPUT);

	//start coms with Nextion display
	delay(500);
	Serial3.begin(115200);  // Start Serial comunication at baud=115200


	//start Arduino serial
	Serial.begin(115200);
	delay(500);

	//start coms with the GPS module
	// note that pin2 and pin3 are hardware connected to the Serial1 pins on the Mega(18 & 19)

	Serial1.begin(9600);
	pinMode(GPS_TX_DIGITAL_OUT_PIN, INPUT);
	pinMode(GPS_RX_DIGITAL_OUT_PIN, INPUT);

	unsigned long start = millis();
	while (millis() - start < 2000)
	{
		if (serial_print)Serial.println("trying to get GPS Lock.");
		sprintf(AFR_CHAR, "trying.");
		if (feedgps());
		if (feedgps());
		if (feedgps());
	}

//	attachInterrupt(digitalPinToInterrupt(interruptPin), rippims, FALLING);

	//Serial.println();
	//  Serial.print("what is in the eprom first: ");
	//  Serial.println(EEPROMReadlong(10));
	//  delay(1000);

	//Starting at the first byte on the eeprom.

	//Writing first long.
	// EEPROMWritelong(10, 3465113);
	// EEPROMWritelong(15, 1623);


	//  delay(1000);
	odototal = 1.9 + (EEPROMReadlong(10) * 0.1);
//	odototal = 0 + (EEPROMReadlong(10) * 0.1);
	//  odototal = 350410;  //for correcting
	Setodo(odototal);

	triptotal = 1.9 + (EEPROMReadlong(15) * 0.01);

//	triptotal = 0 + (EEPROMReadlong(15) * 0.01);


	//  Serial.print("what is in the eprom after the write: ");
	//  Serial.println(EEPROMReadlong(10));

	//clear fuel pressure averager
	for (int i = 0; i < FP_AC; i++) {
		FP_Readings[i] = 0;
	}

	//clear OIL pressure averager
	for (int i = 0; i < OP_AC; i++) {
		OP_Readings[i] = 0;
	}

	//clear Engine Temp averager
	for (int i = 0; i < TG_AC; i++) {
		TG_Readings[i] = 0;
	}
	
	//clear Fuel Gauge averager
	for (int i = 0; i < FG_AC; i++) {
		FG_Readings[i] = 0;
	}

	//clear Fuel Gauge averager
	for (int i = 0; i < AFR_AC; i++) {
		AFR_Readings[i] = 0;
	}

	unsigned long duration;
}

/*
void rippims()
{
	//Each rotation, this interrupt function is run twice, so take that into consideration for
	//calculating RPM
	//Update count
	rpmcount++;
}*/

long getFrequency(int pin) {
#define SAMPLES 4
	long freq = 0;
	for (unsigned int j = 0; j < SAMPLES; j++) freq += 190000 / pulseIn(pin, HIGH, 100000);
	return freq / SAMPLES;
}

/*
  ░░▄▀░░░░░░░░░░░░░░░▀▀▄▄░░░░
  ░░▄▀░░░░░░░░░░░░░░░░░░░░▀▄░░░
  ░▄▀░░░░░░░░░░░░░░░░░░░░░░░█░░
  ░█░░░░░░░░░░░░░░░░░░░░░░░░░█░
  ▐░░░░░░░░░░░░░░░░░░░░░░░░░░░█
  █░░░░▀▀▄▄▄▄░░░▄▌░░░░░░░░░░░░▐
  ▌░░░░░▌░░▀▀█▀▀░░░▄▄░░░░░░░▌░▐
  ▌░░░░░░▀▀▀▀░░░░░░▌░▀██▄▄▄▀░░▐
  ▌░░░░░░░░░░░░░░░░░▀▄▄▄▄▀░░░▄▌
  ▐░░░░▐░░░░░░░░░░░░░░░░░░░░▄▀░
  ░█░░░▌░░▌▀▀▀▄▄▄▄░░░░░░░░░▄▀░░
  ░░█░░▀░░░░░░░░░░▀▌░░▌░░░█░░░░
  ░░░▀▄░░░░░░░░░░░░░▄▀░░▄▀░░░░░
  ░░░░░▀▄▄▄░░░░░░░░░▄▄▀▀░░░░░░░
  ░░░░░░░░▐▌▀▀▀▀▀▀▀▀░░░░░░░░░░░
  ░░░░░░░░█░░░░░░░░░░░░░░░░░░░░
  ░░╔═╗╔═╗╔═╗░░░░░║░║╔═╗║░║░░░░
  ░░╠═╣╠╦╝╠╣░░░░░░╚╦╝║░║║░║░░░░
  ░░║░║║╚═╚═╝░░░░░░║░╚═╝╚═╝░░░░
  ║╔═░╦░╦═╗╦═╗╦╔╗║╔═╗░░╔╦╗╔═╗╔╗
  ╠╩╗░║░║░║║░║║║║║║═╗░░║║║╠╣░╔╝
  ║░╚░╩░╩═╝╩═╝╩║╚╝╚═╝░░║║║╚═╝▄░
*/

void loop()
{
	rpm = 60 * getFrequency(interruptPin);


	RPM_Array[ii] = rpm * 1.0;
	ii++; if (ii >= AC)ii = 1;


	while (Serial3.available() > 0) {
		PG = Serial3.read();
		Serial.print("New Data from screen: ");
		Serial.println(PG);
		//delay(100);
		if ((PG.toInt() < 55) && (PG.toInt() > 47))
		{
			number = PG.toInt();
			//SetAfr7(PG);

			// delay(20);
		}
		if (PG.toInt() == 65)
		{
			if (AFR_Graph) { //A for afr on graphs
				AFR_Graph = false;
			}
			else {
				AFR_Graph = true;
			}
		}

		if (PG.toInt() == 69)
		{
			if (ENGT_Graph) //E for Engine temp on graphs
			{
				ENGT_Graph = false;
			}
			else {
				ENGT_Graph = true;
			}
		}

		if (PG.toInt() == 77) {
			if (MAP_Graph) //M for Engine temp on graphs
			{
				MAP_Graph = false;
			}
			else {
				MAP_Graph = true;
			}
		}

		if (PG.toInt() == 70) {
			if (FUELP_Graph) //F for Engine temp on graphs
			{
				FUELP_Graph = false;
			}
			else {
				FUELP_Graph = true;
			}
		}

		if (PG.toInt() == 111) {

			if (OIL_Graph) //F for Engine temp on graphs
			{
				OIL_Graph = false;
			}
			else {
				OIL_Graph = true;
			}
		}

		if (PG.toInt() == 82) {
			if (RPM_Graph) //F for Engine temp on graphs
			{
				RPM_Graph = false;
			}
			else {
				RPM_Graph = true;
			}
		}    if (PG.toInt() == 84) {
			//reset trip counter to 0
			triptotal = 0;
			EEPROMWritelong(tripaddress, triptotal);
		}
	}


	if (number == 48) {
		SetTime(daytime);

		// RPM screen
		for (int i = 1; i < AC; i++) {
			RPM_Averager = RPM_Averager + RPM_Array[i];
		}
		RPM_Averager = (RPM_Averager / (AC));
		//   RPM_Averager = rpm;
		SetRPM(map(RPM_Averager, 0, 11000, 1, 56));
		Serial3.print("rpm7seg");
		Serial3.print(".txt=\"");
		Serial3.print(RPM_Averager);
		Serial3.print("\"");
		EndCmd();
		//    Serial.print("rpms: ");
		//    Serial.println(RPM_Averager);

		Warning_Indicators();

		//mmmmm
		AFR();
		SetAfr(map(AFR_AverageLevel * 100, 1000, 1900, 62, 93));
		int d1 = AFR_AverageLevel;
		float f2 = AFR_AverageLevel - d1;
		int d2 = trunc(f2 * 100);
		sprintf(AFR_CHAR, "%02d.%02d   ", d1, d2);
		Serial3.print("AFR7");
		Serial3.print(".txt=\"");
		Serial3.print(AFR_CHAR);
		Serial3.print("\"");
		EndCmd();

		// SetTempGauge(map(analogRead(A1), 1021, 810, 1, 20));
		TGA();
		SetTempGauge(TG_AverageLevel);

		//fuelgauge   red/yel   Pin A3
		FGA();
		SetFuelGauge(FG_AverageLevel);

		OPA();
		//bar
		//long boostval = map(OP_AverageLevel, 10, 125, 100, 700);
		//PSI
	//	long boostval = 14.5038* map(OP_AverageLevel, 10, 105, 190, 490);
		long boostval = (83 * OP_AverageLevel);

		Serial3.print("bst7seg");
		Serial3.print(".txt=\"");
		Serial3.print(boostval*0.01);
		Serial3.print("\"");
		EndCmd();
	}
	{ //print the time of day on LCD

		char sz[11] = "*********";
		int year;
		byte month, day, hour, minute, second, hundredths;
		unsigned long age;
		gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
		if (age == TinyGPS::GPS_INVALID_AGE) {
			if (serial_print)Serial.print(".No GPS.");
			sprintf(sz, ".No GPS.");
		}
		else
		{
			//set timezone
			hour = hour + 2;
			if (hour > 23)hour = hour - 24;
			sprintf(sz, "%02d:%02d:%02d", hour, minute, second);
			if (serial_print)Serial.print(sz);
		}
	}

	if (number == 49) {// speed screen
		SetTime(daytime);
		//  Serial.println(gps.f_speed_kmph());
		SetSpeed(map(1 + gps.f_speed_kmph(), 0, 220, 1, 56));
		Serial3.print("spd7seg");
		Serial3.print(".txt=\"");
		Serial3.print(gps.f_speed_kmph());
		Serial3.print("\"");
		EndCmd();
		Warning_Indicators();


		AFR();
		SetAfr(map(AFR_AverageLevel * 100, 1000, 1900, 62, 93));

		int d1 = AFR_AverageLevel;
		float f2 = AFR_AverageLevel - d1;
		int d2 = trunc(f2 * 100);
		sprintf(AFR_CHAR, "%02d.%02d   ", d1, d2);
		Serial3.print("AFR7");
		Serial3.print(".txt=\"");
		Serial3.print(AFR_CHAR);
		Serial3.print("\"");
		EndCmd();

		// SetTempGauge(map(analogRead(A1), 1021, 810, 1, 20));
		TGA();
		SetTempGauge(TG_AverageLevel);

		//fuelgauge   red/yel   Pin A3
		FGA();
		SetFuelGauge(FG_AverageLevel);



		// Engine temp  red/blk Pin1
		//    settmpsml(map(analogRead(A1), 000, 922, 20, 30)); //engine temp


		Serial3.print("bst7seg");
		Serial3.print(".txt=\"");//micc
		OPA();
		//bar
		//long boostval = map(OP_AverageLevel, 10, 125, 100, 700);
		//PSI
//		long boostval = 14.5038* map(OP_AverageLevel, 10, 105, 190, 490);
//		long boostval = 14.5038* map(OP_AverageLevel, 30, 120, 250, 600);
		long boostval = (83 * OP_AverageLevel);

		//    long boostval = map(analogRead(A10), 80, 990, 30 , 250);
		//	Serial3.print((boostval * 0.01) - 1);
		Serial3.print(boostval*0.01);

		Serial3.print("\"");
		EndCmd();

		//  setegtsml(map(ktc.readCelsius(), 350, 800, 74, 113));

	}


	if (number == 50) {// Boost screen
		SetTime(daytime);

		//    Serial.print("Boostpin: ");
		//  Serial.println(map(analogRead(A10), 0, 1023, 0, 1023 ));


		SetBst(map(analogRead(A10), 80, 990, 1, 56));
		Serial3.print("bst7seg");
		Serial3.print(".txt=\"");
		//    long boostval = map(analogRead(A10), 0, 1023, 0, 1023);
		long boostval = map(analogRead(A10), 30, 920, 30, 250);
		Serial3.print((boostval * 0.01) - 1);
		//    Serial3.print(boostval);

		Serial3.print("\"");
		EndCmd();

		Warning_Indicators();

	
		AFR();
		SetAfr(map(AFR_AverageLevel * 100, 1000, 1900, 62, 93));

		int d1 = AFR_AverageLevel;
		float f2 = AFR_AverageLevel - d1;
		int d2 = trunc(f2 * 100);
		sprintf(AFR_CHAR, "%02d.%02d   ", d1, d2);


		Serial3.print("AFR7");
		Serial3.print(".txt=\"");
		Serial3.print(AFR_CHAR);
		Serial3.print("\"");
		EndCmd();

	}


	if (number == 51) { //pressure screen
		SetTime(daytime);

		for (int i = 1; i < AC; i++) {
			RPM_Averager = RPM_Averager + RPM_Array[i];
		}
		RPM_Averager = (RPM_Averager / (AC + 0));

		OPA();
		SetOilPressure(map(OP_AverageLevel, 10, 125, 1, 27));
	

		FPA();
		SetFuelPressure(map(FP_AverageLevel, 50, 165, 27, 53));

		Serial3.print("spd7seg");
		Serial3.print(".txt=\"");
		Serial3.print(1 + gps.f_speed_kmph());
		Serial3.print("\"");
		EndCmd();

		Serial3.print("rpm7seg");
		Serial3.print(".txt=\"");
		Serial3.print(RPM_Averager);
		Serial3.print("\"");
		EndCmd();

		//    SetTempGauge(analogRead(A1), 1021, 810, 1, 20));
		TGA();
		SetTempGauge(TG_AverageLevel);


		//fuelgauge   red/yel   Pin A3

		FGA();
		SetFuelGauge(FG_AverageLevel);


		AFR();
		int d1 = AFR_AverageLevel;
		float f2 = AFR_AverageLevel - d1;
		int d2 = trunc(f2 * 100);
		sprintf(AFR_CHAR, "%02d.%02d   ", d1, d2);
		Serial3.print("AFR7");
		Serial3.print(".txt=\"");
		Serial3.print(AFR_CHAR);
		Serial3.print("\"");
		EndCmd();
	}

	if (number == 52) {  //Graphs screen
		double RPM_Averager = 0;

		for (int i = 1; i < AC; i++) {
			RPM_Averager = RPM_Averager + RPM_Array[i];
		}
		RPM_Averager = (RPM_Averager / (AC + 0));

		if (AFR_Graph) {
			AFR();
			pcob0 = map(100 * AFR_AverageLevel, 1000, 2000, 0, 255);
		}
		else {
			pcob0 = 0;
		}
		if (!ENGT_Graph) {
			TGA();
			pcob1 = map(TG_AverageLevel, 1023, 870, 1, 255);
		}
		else {
			pcob1 = 0;
		}

		if (MAP_Graph) {
			pcob2 = map(analogRead(A10), 0, 990, 30, 255);
		}
		else {
			pcob2 = 0;
		}

		if (FUELP_Graph) {//    SetFuelPressure(map(analogRead(A5), 50, 165, 27, 53));
			FPA();
			pcob3 = map(FP_AverageLevel, 1, 150, 0, 255);
		}
		else {
			pcob3 = 0;
		}
		setwaveBottom(pcob0, pcob1, pcob2, pcob3);
		//        Serial.print("oilp: ");
		//        Serial.println((OIL_Graph));

		if (OIL_Graph) {
			OPA();
			pcot0 = map(OP_AverageLevel, 1, 230, 0, 255);
		}
		else {

			pcot0 = 0;
			//pcot0 = map(analogRead(A4), 10, 200, 0, 255);
		}

		//        Serial.println(analogRead(A4));
		if (RPM_Graph) {//      map(RPM_Averager, 0, 10500, 1, 56)
			pcot1 = map(RPM_Averager, 1, 11000, 1, 255);
		}
		else {
			pcot1 = 0;
		}
		pcot2 = 0;
		pcot3 = 0;
		setwaveTop(pcot0, pcot1, pcot2, pcot3);

	}


	//  bool newdata = false;
	unsigned long start = millis();

	// get GPS
	while (millis() - start < 20)
	{
		if (feedgps());//newdata = true;
		//Serial.println("getting gps");

	}



	gpsdump(gps);
	{ //print the time of day on LCD

	  //daytime[11] = "*********";
		strcpy(daytime, "         ");

		int year;
		byte month, day, hour, minute, second, hundredths;
		unsigned long age;
		gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
		if (age == TinyGPS::GPS_INVALID_AGE) {
			if (serial_print)Serial.print(".No GPS.");
			sprintf(daytime, ".No GPS. ");
		}
		else
		{
			//set timezone
			hour = hour + 2;
			if (hour > 23)hour = hour - 24;
			sprintf(daytime, "%02d:%02d:%02d", hour, minute, second);

			if (serial_print)Serial.print(daytime);
		}
		//    Serial.print("time: ");
		//    Serial.println(daytime);

	}

	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	//if(serial_print)
	//  Serial.print("Opening the file in RW:");
	//if(serial_print)
	//  Serial.print(date_name);
	//  File dataFile = SD.open(date_name, FILE_WRITE);
	// if the file is available, write to it:
	//  if (dataFile) {
	//    dataFile.println(dataString);
	//    dataFile.close();
	dataString = "";
	//  } else {
	// Serial.println("file not available");
	//  }


	{ //print the time of day on LCD

		char sz[11] = "*********";
		int year;
		byte month, day, hour, minute, second, hundredths;
		unsigned long age;
		gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
		if (age == TinyGPS::GPS_INVALID_AGE) {
			if (serial_print)Serial.print(".No GPS.");
			sprintf(sz, ".No GPS.");
		}
		else
		{
			//set timezone
			hour = hour + 2;
			if (hour > 23)hour = hour - 24;
			sprintf(sz, "%02d:%02d:%02d", hour, minute, second);
			if (serial_print)Serial.print(sz);
		}
	}


	float timewait = 1000;
	if (millis() - odotimer > timewait) {// do this every 5 seconds
		float Dist = 0;
		//to prevent runaway odometer, make sure the speed is not something ridiculous 
		if (gps.f_speed_kmph() < 221) Dist = (gps.f_speed_kmph() * (timewait / 1000) / 3600);
		//  Serial.print("distance: ");
		//  Serial.println(Dist, 4);
		//this is where we get the distance traveled
		acumilator = acumilator + (1.2*Dist);  //adding 0.2 for gps laggy startups

		//   Serial.print("trip: ");
		//   Serial.println(acumilator, 4);
		if (acumilator > 0.1) {
			odototal += acumilator;
			triptotal += acumilator;
			acumilator = 0;
		}
		//   Serial.print("Odo: ");
		//   Serial.println(odototal, 4);

		Setodo(odototal);
		Settrip(triptotal);
		//Writing first long.
		EEPROMWritelong(odoaddress, (odototal * 10));
		EEPROMWritelong(tripaddress, (triptotal * 100));
		odotimer = millis();
	}



}  //end of the loop


void Setodo(double inval) {
	String Gaugename = "odo";
	Serial3.print(Gaugename);
	Serial3.print(".txt=\"");
	Serial3.print(inval, 1);
	Serial3.print("\"");
	Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
	Serial3.write(0xff);
	Serial3.write(0xff);

}

void Settrip(double inval) {
	String Gaugename = "trip";
	Serial3.print(Gaugename);
	Serial3.print(".txt=\"");
	Serial3.print(inval, 2);
	Serial3.print("\"");
	Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
	Serial3.write(0xff);
	Serial3.write(0xff);

}



//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value)
{
	//Decomposition from a long to 4 bytes by using bitshift.
	//One = Most significant -> Four = Least significant byte
	byte four = (value & 0xFF);
	byte three = ((value >> 8) & 0xFF);
	byte two = ((value >> 16) & 0xFF);
	byte one = ((value >> 24) & 0xFF);

	//Write the 4 bytes into the eeprom memory.
	if (EEPROM.read(address) != four) {
		EEPROM.write(address, four);
		Serial.println("wrote four");
	}
	if (EEPROM.read(address + 1) != three) {
		EEPROM.write(address + 1, three);
		Serial.println("wrote three");
	}
	if (EEPROM.read(address + 2) != two) {
		EEPROM.write(address + 2, two);
		Serial.println("wrote two");
	}
	if (EEPROM.read(address + 3) != one) {
		EEPROM.write(address + 3, one);
		Serial.println("wrote one");
	}
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address)
{
	//Read the 4 bytes from the eeprom memory.
	long four = EEPROM.read(address);
	long three = EEPROM.read(address + 1);
	long two = EEPROM.read(address + 2);
	long one = EEPROM.read(address + 3);

	//Return the recomposed long by using bitshift.
	return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

}

void FPA() {
	//fuel Pressure , Analog pin 5; WTE
	// Fuel Pressure Averager

	FP_Level = analogRead(A5);
	FP_Total = FP_Total - FP_Readings[FP_ArrayIndex];
	FP_Readings[FP_ArrayIndex] = FP_Level;
	FP_Total = FP_Total + FP_Readings[FP_ArrayIndex];
	FP_ArrayIndex += 1;
	// At the end of the array  then start again
	if (FP_ArrayIndex >= FP_AC)FP_ArrayIndex = 0;
	FP_AverageLevel = FP_Total / FP_AC;

}


void OPA() {

	//oil Pressure , Analog pin 4; GRN

	OP_Level = analogRead(A4);
	OP_Total = OP_Total - OP_Readings[OP_ArrayIndex];
	OP_Readings[OP_ArrayIndex] = OP_Level;
	OP_Total = OP_Total + OP_Readings[OP_ArrayIndex];
	OP_ArrayIndex += 1;
	// At the end of the array  then start again
	if (OP_ArrayIndex >= OP_AC)OP_ArrayIndex = 0;
	OP_AverageLevel = OP_Total / OP_AC;
}




void TGA() {

	// pin1          Engine temp  red/blk  A1

	TG_Level = analogRead(A1);
	TG_Total = TG_Total - TG_Readings[TG_ArrayIndex];
	TG_Readings[TG_ArrayIndex] = TG_Level;
	TG_Total = TG_Total + TG_Readings[TG_ArrayIndex];
	TG_ArrayIndex += 1;
	// At the end of the array  then start again
	if (TG_ArrayIndex >= TG_AC)TG_ArrayIndex = 0;
	TG_AverageLevel = TG_Total / TG_AC;
}


void FGA() {

	//fuelgauge   red/yel   Pin3

	FG_Level = analogRead(A3);
	FG_Total = FG_Total - FG_Readings[FG_ArrayIndex];
	FG_Readings[FG_ArrayIndex] = FG_Level;
	FG_Total = FG_Total + FG_Readings[FG_ArrayIndex];
	FG_ArrayIndex += 1;
	// At the end of the array  then start again
	if (FG_ArrayIndex >= FG_AC)FG_ArrayIndex = 0;
	FG_AverageLevel = FG_Total / FG_AC;
}

void AFR() {

	// AFR comes in on analog 8 and is a 0~5v signal  blu/yel
	// float AFR_VAL = (10 + (2 * (analogRead(A8) * AFR_Constant)));

	AFR_Level = (9 + (2 * (analogRead(A8) * AFR_Constant)));
	AFR_Total = AFR_Total - AFR_Readings[AFR_ArrayIndex];
	AFR_Readings[AFR_ArrayIndex] = AFR_Level;
	AFR_Total = AFR_Total + AFR_Readings[AFR_ArrayIndex];
	AFR_ArrayIndex += 1;
	// At the end of the array  then start again
	if (AFR_ArrayIndex >= AFR_AC)AFR_ArrayIndex = 0;
	AFR_AverageLevel = AFR_Total / AFR_AC;
}

