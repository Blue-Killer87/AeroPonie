#include <DHT.h>
#include "GravityTDS.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define tdsPin A1
#define phPin A2
#define ldrPin A0
#define cerpadloPin 3

DHT dht(DHTPIN, DHTTYPE);         //vytvoreni objektu pro mereni teploty a vlhkosti
GravityTDS tds;                   //vytvoreni objektu pro mereni ec

//DHT11
float teplota;
int vlhkost;

//EC
float ppm;
float EC;

//Ph
float phHodnota;                   //realna ph hodnota
float napetiPh;                    
float soucetPh;                    //soucet namerenych hodnot
float prumerPh;                    //prumer namerenych hodnot

//Svetlo
long LUX_CALC_SCALAR = 12518931;   //konstanty pro vypocet luxu
float LUX_CALC_EXPONENT = -1.405;  //....
float ldrZaklad;                   //namerena honota na photoresistoru
float napetiLdr;                   //napeti na photoresistoru
float napetiRes;                   //napeti na resistoru
long ldrOdpor;                     //odpot photoresistoru
int lux;
//int lumen;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  tds.setPin(tdsPin);
  tds.setAref(5.0);                   //nastaveni referencniho napeti pro mereni ec
  tds.setAdcRange(1024);
  tds.begin();
  pinMode(cerpadloPin, OUTPUT);
}

void loop() {
  // -----------DHT11--------------------------------------
  teplota = dht.readTemperature();                //cteni teploty
  vlhkost = dht.readHumidity();                   //cteni vlhkosti
  Serial.print("Teplota: ");
  Serial.println(teplota); //v C na *10na-1
  Serial.print("Vlhkost: ");
  Serial.println(vlhkost); // v %
  
  //----------EC--------------------------------------------
  tds.setTemperature(teplota);                    //nastaveni teploty pro mereni ec
  tds.update();
  ppm = tds.getTdsValue();                        // ppm = parts per milion
  EC = ppm * 2 / 1000;                            //vypocet EC
  Serial.print("EC: ");
  Serial.println(EC);
  
  //----------PH--------------------------------------------
  soucetPh = 0;
  for(int i = 0; i < 10; i++){                    //10x zmereni hodnoty
      soucetPh += analogRead(phPin);
      delay(10);
    }
  prumerPh = soucetPh / 10;                       //vypocteni prumerne hodnoty
  napetiPh = prumerPh /1024 * 5.0;                //prevedeni digitalni hodnoty na napeti
  phHodnota = -5.70 * napetiPh + 27;              //vypocet opravdoveho ph (posledni hodnota nastavuje offset)
  Serial.println(phHodnota);
  
  //---------Svetlo----------------------
  ldrZaklad = analogRead(ldrPin);
  napetiRes = ldrZaklad * 5.0 / 1024;             //prevedeni digitalni hodnoty na napeti (napeti na resistoru)
  napetiLdr = 5.0 - napetiRes;                    //napeti na photoresistoru
  ldrOdpor = napetiLdr/napetiRes * 10000;         //odpor photoresistoru
  lux = LUX_CALC_SCALAR * pow(ldrOdpor, LUX_CALC_EXPONENT);
  //lumen = 500/(ldrOdpor/1000);
  Serial.print("Luxů: ");
  Serial.println(lux);

  //----------cerpadlo-------------------------------------
  //digitalWrite(cerpadloPin, HIGH);             //HIGH zapnuto LOW vypnuto
  delay(2000);
  digitalWrite(cerpadloPin, LOW);
  delay(1000);
}
