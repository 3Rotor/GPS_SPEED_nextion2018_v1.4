void SetAfr(int inval) {
  int updatewidth = 6;
  if (inval != afrold) {
    inval = constrain(inval, 63, 93);
    String Gaugename = "afr";


    for (int i = 62; i <= 92; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth)) {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {

          if (i < 85) {
            Serial3.print(".pic=3");
          } else if (i < 89) {
            Serial3.print(".pic=4");
          } else if (i <= 92) {
            Serial3.print(".pic=5");
          }
        } else {
          Serial3.print(".pic=2");

        }
        EndCmd();
      }
    }
    afrold = inval;

  }
}
