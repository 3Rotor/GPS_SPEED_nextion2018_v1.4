void Warning_Indicators() {
  // get right turn
  if (digitalRead(RightBlinkPin)) { //red/blk
    String Gaugename = "rightturn";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(122);
    EndCmd();
  } else {
    String Gaugename = "rightturn";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(124);
    EndCmd();
  }
  // get teft turn
  if (digitalRead(LeftBlinkPin)) {  //red/blu
    String Gaugename = "leftturn";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(123);
    EndCmd();

  } else {
    String Gaugename = "leftturn";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(124);
    EndCmd();
  }

  // get highBeam
  if (digitalRead(HighbeamPin)) { //grn/blk
    String Gaugename = "p0";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(119);
    EndCmd();

  } else {
    String Gaugename = "p0";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(118);
    EndCmd();

  }
  // get Oil warning

  if (digitalRead(OillightPin)) { //grn/yel
    String Gaugename = "p1";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(120);
    EndCmd();

  } else {
    String Gaugename = "p1";
    Serial3.print(Gaugename);
    Serial3.print(".pic=");
    Serial3.print(121);
    EndCmd();
  }
}

