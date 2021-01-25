#include "mathlib.h"
#include <math.h>
#include <stdio.h>
const int nMaxTerms = 20;
double factorials[20];

double factorial(int n) {
  if (n == 1)
    return 1;
  else
    return (double)n * factorial(n - 1.0);
}

void precalcFactorials() {
  for (int i = 1; i < nMaxTerms + 1; i++) {
    factorials[i - 1] = factorial(i);
  }
}

double Sin(double rads) {
  precalcFactorials();
  double result = rads;
  for (int curTerm = 1; curTerm <= (nMaxTerms / 2) - 1; curTerm++) {
    double curTermValue = pow(rads, (curTerm * 2) + 1);
    curTermValue /= factorials[curTerm * 2];
    if (curTerm & 0x01)
      result -= curTermValue;
    else
      result += curTermValue;
  }

  return result;
}
double Cos(double rads) {
  precalcFactorials();
  double result = 1.0;
  for (int curTerm = 1; curTerm <= (nMaxTerms / 2) - 1; curTerm++) {
    double curTermValue = pow(rads, (curTerm * 2));
    curTermValue /= factorials[(curTerm * 2) - 1];
    if (curTerm & 0x01)
      result -= curTermValue;
    else
      result += curTermValue;
  }
  return result;
}
double Tan(double x) { return Sin(x) / Cos(x); }
double Exp(double x) {
  double sum = 1.0; // initialize sum of series
  for (int i = nMaxTerms - 1; i > 0; --i)
    sum = 1 + x * sum / i;
  return sum;
}
double Log(double x) {
  double count = 1, totalValue = 0, z, powe = 1, y;
  z = (x + 1) / (x - 1);
  double step = ((x - 1) * (x - 1)) / ((x + 1) * (x + 1));
  while (count <= nMaxTerms) {

    z *= step;
    y = (1 / powe) * z;
    totalValue = totalValue + y;
    powe = powe + 2;
    count++;
  }
  return 2 * totalValue;
}
