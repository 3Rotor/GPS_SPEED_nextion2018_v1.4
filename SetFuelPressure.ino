
void SetFuelPressure(int inval) {
  int updatewidth = 16;
  if (inval != flpold) {
    inval = constrain(inval, 27, 52);
    String Gaugename = "flpp";


    for (int i = 27; i <= 52; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth)) {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {


          Serial3.print(".pic=3");



        } else {
          Serial3.print(".pic=2");

        }
        EndCmd();
      }
    }
    flpold = inval;
  }
}

