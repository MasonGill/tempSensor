#include <SPI.h>
#include <DMD.h>
#include <stdlib.h>
#include <TimerOne.h>
#include "Arial14.h"

DMD dmd(1,1);

double tempRead(int analog)
{
  double temp;
  temp = log(((10240000/analog) - 10000));
  temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
  temp -= 273.15;
  return temp;
}

char *tempBuild()
{
  double f = tempRead(analogRead(0));
  int h = f;
  int fPoint = (f - (double)h)*10;
  const char* d = "c";
  const char* j = ".";
  const char b[strlen(d)];
  const char q[strlen(d)];
  itoa(h, b, 10);
  itoa(fPoint, q, 10);
  char* c = malloc(64);
  strcpy(c, b);
  strcat(c, j);
  strcat(c, q);
  strcat(c, d);
  return c;
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
  dmd.drawString( 1, 2, temp, strlen(temp), GRAPHICS_TOGGLE);
  delay(2000);
  dmd.clearScreen(true);
  Serial.print(temp);
  resetFunc();
}

void loop(void){}