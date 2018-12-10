void SetFuelGauge(double inval) {
  
  //  SetFuelGauge(map(Fuel_Level_Averager, 10, 340, 20, 1));
  //  inval = map(inval, 10, 340, 20, 1);
//    Serial.print("fualgauge: ");
//    Serial.println(inval);
/*empty  340   empty
5l     336
10l    270
15l    233
20     201
25     172
30     155
35     138
40     116
45     83
50     77
55     68
60     55
65     38
70     0
75     0

*/
  
  inval = map(inval, 1, 320, 20, 1);


  int updatewidth = 19;
  //if ((inval != flpold))
  {
    inval = constrain(inval, 2, 20);
    String Gaugename = "fuel";


    for (int i = 1; i <= 19; i++) {
      if ((i > inval - updatewidth) && (i < inval + updatewidth)) {
        Serial3.print(Gaugename);
        Serial3.print(i);
        if (i < inval) {

          if (i < 5) {
            Serial3.print(".pic=127");
          } else if (i < 9) {
            Serial3.print(".pic=126");
          } else if (i <= 19) {
            Serial3.print(".pic=125");
          }
        } else {
          Serial3.print(".pic=128");

        }
        EndCmd();
      }
    }

    flpold = inval;
  }
}


