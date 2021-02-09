#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#define SENSOR 10


LiquidCrystal lcd(24,25,26,31,33,35,37); //(RS,RW, E, D4,D5, D6, D7)

const byte filas = 4;
const byte columnas = 4;
byte pinsFilas[filas]={ 44, 45, 46, 47 };    
byte pinsColumnas[columnas]={ 48, 49, 50, 51 };  

char teclas[filas] [columnas]={
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

Keypad teclado = Keypad(makeKeymap(teclas), pinsFilas, pinsColumnas, filas, columnas);
char tecla;
//variables
bool conf_pas=0;
String pasadas="0";
int nuevo=0;
int ad=1;
int k=0;
int nue[]={0x33,0x30,0x34,0x34};
int address = 1;
int long_num=8;
int c_num=0;
char value;
int a = 0;
int n = 0;
unsigned long tsen=0,tled=0,ti=0;
boolean par=true;
//funciones
boolean isNumber(char c);

void setup() {
  Serial.begin (112500);
  pinMode(SENSOR, INPUT);
  pinMode(53,OUTPUT);
  
  if(pasadas.equals("0")){
    pasadas="";
    for(int i=0;i<long_num;i++){
      value = EEPROM.read(address);
      if(String(value).equals("0")||String(value).equals("1")||String(value).equals("2")||
  String(value).equals("3")||String(value).equals("4")||String(value).equals("5")||
  String(value).equals("6")||String(value).equals("7")||String(value).equals("8")||String(value).equals("9")){
        pasadas=pasadas+String(value);
        address=address+1;
        c_num++;
      }else{
        break;
      }
    }
    long_num=c_num;
    Serial.println(pasadas);
  }
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Contador-");
  lcd.print(pasadas);
  lcd.setCursor(0,1);
  lcd.print("****************");
  teclado.addEventListener(keypadEvent); // Add an event listener for this keypad
}

void loop() {
  tecla=teclado.getKey();
  if (tecla) {
      Serial.println(tecla);
   }

  
  if(conf_pas){
    //digitalWrite(53,HIGH);
    if(String(tecla).equals("1") || String(tecla).equals("2") || String(tecla).equals("3") || String(tecla).equals("4") || String(tecla).equals("5") || String(tecla).equals("6") || String(tecla).equals("7") || String(tecla).equals("8") || String(tecla).equals("9") || String(tecla).equals("0")){
      pasadas+=tecla;
      lcd.setCursor(0,1);
      lcd.print(pasadas);
      lcd.print("               "); 
    }
  }
  if(!conf_pas){
    //digitalWrite(53,HIGH);
    if(digitalRead(SENSOR)==0 && a==0){
      n++;
      a=1;
      lcd.setCursor(0,1);
      lcd.print(n);
      lcd.print("  Pasadas      ");
      Serial.println(n);
    }else if(digitalRead(SENSOR)==1 && a==1){
      a=0;
    }

    if(millis()>tled+100 && n>(pasadas.toInt()-3) && par){
      tled=millis();
      digitalWrite(53,HIGH);
      par=false;
    }
    if(millis()>tled+100 && n>(pasadas.toInt()-3) && !par){
      tled=millis();
      digitalWrite(53,LOW);
      par=true;
    }
  }
}

void keypadEvent(KeypadEvent tecla){
    switch (teclado.getState()){
    case PRESSED:
        if (tecla == 'A') {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("> Nro - pasadas");
          lcd.setCursor(0,1);
          lcd.print(" -----  ");
          pasadas="";
          conf_pas=true;
          digitalWrite(53,LOW);
        }else if(tecla == 'D'){
          if(pasadas.toInt()>=1){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Contador-");
            lcd.print(pasadas);
            lcd.setCursor(0,1);
            lcd.print("****************");
            pasadas="0"+pasadas;
            int p = pasadas.length()+1;
            char msg[p];
            
            pasadas.toCharArray(msg,p);
            Serial.println(msg); 
            for(int i=1;i<p;i++){
              EEPROM.write(i,msg[i]);
            }
            char h[1]={"?"};
            int f = p+1;
            //EEPROM.write(f,h[1]);
            conf_pas=false;
            n=0;
          }else{
            lcd.setCursor(0, 0);
            lcd.print("Ingresar numero    ");
          }

        }
        break;

}}
