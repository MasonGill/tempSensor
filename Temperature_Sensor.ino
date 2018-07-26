#include <SPI.h>
#include <DMD.h>
#include <stdlib.h>
#include <TimerOne.h>
#include "Arial14.h"

DMD dmd(1, 1);

double tempRead(int analog)
{
  double temp;
  temp = log(((10240000 / analog) - 10000));
  temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp )) * temp );
  temp -= 273.15;
  return temp;
}

char *tempBuild()
{
  double f = tempRead(analogRead(0));
  int fullNum = f;
  int fPoint = (f - (double)fullNum) * 10;
  const char* letter = "c";
  const char* dot = ".";
  const char beforep[4];
  const char afterp[2];
  itoa(fullNum, beforep, 10);
  itoa(fPoint, afterp, 10);
  char* temp = malloc(16);
  strcpy(temp, beforep);
  strcat(temp, dot);
  strcat(temp, afterp);
  strcat(temp, letter);
  return temp;
}

void(* resetFunc) (void) = 0;

void scan()
{
  dmd.scanDisplayBySPI();
}

void setup(void)
{
  Timer1.initialize(500);
  Timer1.attachInterrupt(scan, 500);
  dmd.selectFont(Arial_14);
  Serial.begin(9600);
  char *temp = tempBuild();
  Serial.print(temp);
  dmd.drawString( 1, 2, temp, strlen(temp), GRAPHICS_TOGGLE);
  delay(2000);
  dmd.clearScreen(true);
  resetFunc();
}

void loop(void) {}
