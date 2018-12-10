void SetTempGauge(int inval) {

  inval = map(inval, 1021, 810, 1, 20);




  int updatewidth = 19;
//  if ((inval != tempold))
  {
    inval = constrain(inval, 2, 20);
    String Gaugename = "temp";
    for (int i = 1; i <= 19; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth))
      {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {

          if (i < 5) {
            Serial3.print(".pic=125");
          } else if (i < 14) {
            Serial3.print(".pic=126");
          } else if (i <= 19) {
            Serial3.print(".pic=127");
          }


        } else {
          Serial3.print(".pic=128");

        }
        EndCmd();
      }
    }

    tempold = inval;

  }

}


