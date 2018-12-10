static void gpsdump(TinyGPS & gps)
{
  // for use while calibrating
  /*
    simulate speed reading from GPS with pot
    val = analogRead(analogPin);    // read the input pin
    val=map(val, 0,912 ,0, 180);//pwmout=310;
  */
  float flat, flon;
  long lat, lon;
  unsigned long fix_age, time, date, speed, course;

  gps.f_get_position(&flat, &flon, &fix_age);
  if (serial_print)Serial.print(" LT:");
  dataString += String(" LT:");
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 9, 5);
  dataString +=  long(flat * 100000);

  if (serial_print)Serial.print(" LN:");
  dataString += String(" LN:");
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 10, 5);
  dataString +=  long(flon * 100000);

  // time in hhmmsscc, date in ddmmyy
  if (serial_print)Serial.print(" DATE:");
  dataString += String(" DATE:");
  print_date(gps);

  //speed
  if (serial_print)Serial.print("Km/h:");
  dataString += String("Km/h:");
  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  dataString +=  long(gps.f_speed_kmph() * 1);
  //**********************************************************************************
  // This is where Shit get's real.
  //feed speed into averaging array
  //  readingarray[ii]=gps.f_speed_kmph();
  //  readingarray[ii]=gps.f_speed_kmph();
  //  readingarray[ii]=val; //comment this line to switch to the GPS value
  //**********************************************************************************

  //  if (ii > AC)ii = 0;
  //  ii++;//get average of the full array
  //  if (serial_print && log_array)Serial.print("ar[");
  //  if (array2file)dataString += String("ar[");

  //dataString += String("PWM:");

  if (serial_print)Serial.println();

}

void print_float(float val, float invalid, int len, int prec)
{
  char sz[32];
  if (val == invalid)
  {
    strcpy(sz, "*******");
    sz[len] = 0;
    if (len > 0)
      sz[len - 1] = ' ';
    for (int i = 7; i < len; ++i)
      sz[i] = ' ';
    if (serial_print)Serial.print(sz);
  }
  else
  {
    if (serial_print)Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1);
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      if (serial_print)Serial.print(" ");
  }
  feedgps();
}

void print_date(TinyGPS & gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE) {
    if (serial_print)Serial.print(" No GPS Lock ");
    dataString += String(" No GPS Lock ");
  }
  else
  {
    char sz[20];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d", month, day, year, hour, minute, second);
    if (serial_print)Serial.print(sz);
    dataString += String(sz);
  }
  feedgps();
}

void get_file_name(TinyGPS & gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);

  if (age == TinyGPS::GPS_INVALID_AGE) {
    if (serial_print)Serial.println("Waiting for GPS lock.");
    file_name_set = false;
  }
  else
  {
    sprintf(date_name, "%02d%02d%02d%02d%02d.log", year , month, day, hour, minute);
    if (serial_print)Serial.print(date_name);
    file_name_set = true;
  }
  feedgps();
}

static bool feedgps()
{
  while (  Serial1.available())
  {
    if (gps.encode(  Serial1.read()))
      return true;
  }
  return false;
}


