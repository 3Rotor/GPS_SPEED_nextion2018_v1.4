void EndCmd() {
  Serial3.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial3.write(0xff);
  Serial3.write(0xff);
}

