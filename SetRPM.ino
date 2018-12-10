void SetRPM(int inval) {
  int updatewidth = 6;
  if ((inval != rpmold)) {
    inval = constrain(inval, 2, 57);
    String Gaugename = "rpm";


    for (int i = 1; i <= 56; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth)) {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {

          if (i < 34) {
            Serial3.print(".pic=3");
          } else if (i < 46) {
            Serial3.print(".pic=4");
          } else if (i <= 56) {
            Serial3.print(".pic=5");
          }

        } else {
          Serial3.print(".pic=2");
        }
        EndCmd();
      }
    }
    rpmold = inval;
  }
}


