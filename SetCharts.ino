void setwaveTop(int c1, int c2, int c3, int c4) {
  // This sends data to the waveform. There are 3 numbers you have to put with a comma
  // between them: [objectID],[Channel],[Value]
  int Id = 8;


  Serial3.print("add ");
  Serial3.print(Id);
  Serial3.print(",0,");
  Serial3.print(c1);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("add ");
  Serial3.print(Id);
  Serial3.print(",1,");
  Serial3.print(c2);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("add ");
  Serial3.print(Id);
  Serial3.print(",2,");
  Serial3.print(c3);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);

  Serial3.print("add ");
  Serial3.print(Id);
  Serial3.print(",3,");
  Serial3.print(c4);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);
}

void setwaveBottom(int c1, int c2, int c3, int c4) {
  // This sends data to the waveform. There are 3 numbers you have to put with a comma
  // between them: [objectID],[Channel],[Value]

  Serial3.print("add 1,0,");
  Serial3.print(c1);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.print("add 1,1,");
  Serial3.print(c2);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.print("add 1,2,");
  Serial3.print(c3);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.print("add 1,3,");
  Serial3.print(c4);
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);
}

