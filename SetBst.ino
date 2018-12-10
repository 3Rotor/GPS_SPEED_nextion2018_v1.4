void SetBst(int inval) {
  int updatewidth = 15;
  if (inval != boostold) {

    inval = constrain(inval, 2, 57);
    String Gaugename = "bst";


    for (int i = 1; i <= 56; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth)) {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {

          if (i < 36) {
            Serial3.print(".pic=3");
          } else if (i < 44) {
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

    boostold = inval;

  }
}


