void SetOilPressure(int inval) {
  int updatewidth = 16;
  if (inval != oilold)  {
    inval = constrain(inval, 1, 26);
    String Gaugename = "oilp";


    for (int i = 1; i <= 26; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth)) {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {

          if (i < 3) {
            Serial3.print(".pic=5");
          } else if (i < 8) {
            Serial3.print(".pic=4");
          } else if (i <= 26) {
            Serial3.print(".pic=3");
          }


        } else {
          Serial3.print(".pic=2");

        }
        EndCmd();
      }
    }
    oilold = inval;
  }
}

