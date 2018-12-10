float Get_AFR()
{
  float AFR_VAL = (9 + (2 * (analogRead(A8) * 0.0048858125)));
return AFR_VAL;
}

