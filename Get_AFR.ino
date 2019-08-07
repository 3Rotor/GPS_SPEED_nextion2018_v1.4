float Get_AFR()
{
  float AFR_VAL = (9 + (2 * (analogRead(A8) * AFR_Constant)));
return AFR_VAL;
}

