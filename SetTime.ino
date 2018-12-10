void SetTime(String inval) {
  if (inval != timold) {
    String Gaugename = "daytime";
    Serial3.print(Gaugename);
    Serial3.print(".txt=\"");
    Serial3.print(inval);
    Serial3.print("\"");
    EndCmd();

  } timold = inval;
}

