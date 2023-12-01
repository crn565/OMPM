#include <WiFi.h>
#include <Arduino.h>
#include <PZEM004Tv30.h>
#include "time.h"
//gestion de la sd
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "SPIFFS.h"
#include "TimeLib.h"
#include <NTPClient.h>
#include <ezButton.h>
ezButton toggleSwitch(15);  // create ezButton object that attach to pin 7;
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define SDA 13  //Define SDA pins
#define SCL 14  //Define SCL pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);
const int chipSelect = 5;  //10;
#define LED 2


//direcciones de los contadores
uint8_t addr0 = 0x110;  //1primer pzem es reconocido como 10   consumo agregado
uint8_t addr5 = 0x120;  //2primer pzem es reconocido como 30   enchufe 2  LUZ LED
uint8_t addr4 = 0x130;  //4primer pzem es reconocido como 50   enchufe 4  ORDENADOR
uint8_t addr3 = 0x140;  //3primer pzem es reconocido como 40   enchufe 3  LUZ HALOGENA
uint8_t addr2 = 0x150;  //2primer pzem es reconocido como 20  enchufe 1  VENTILADOR
uint8_t addr1 = 0x160;  //5primer pzem es reconocido como 60   enchufe 1 SOLDADOR


//contador de acciones
int ca, cb, cc, cd, ce, cf, i;

//inicializacion de los contadores
PZEM004Tv30 pzem0(Serial2, 16, 17, addr0);
PZEM004Tv30 pzem1(Serial2, 16, 17, addr1);
PZEM004Tv30 pzem2(Serial2, 16, 17, addr2);
PZEM004Tv30 pzem3(Serial2, 16, 17, addr3);
PZEM004Tv30 pzem4(Serial2, 16, 17, addr4);
PZEM004Tv30 pzem5(Serial2, 16, 17, addr5);

//WiFi network info.
const char* ssid = "PONER AQUI SU RED";
const char* password = "CLAVE WIFI";

const char* Horaactual;
// Configuración del servidor NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const int timeZone = -6;  // Zona horaria (horas)

// Objeto para obtener el tiempo del servidor NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, timeZone * 3600, 60000);

long long currentTimestamp;
long long newtimestamp;
tmElements_t my_time;   // time elements structure
time_t unix_timestamp;  // a timestamp
double milliseconds = 0;
float pp = 0;
int64_t timestamp13 = 0;
String Nombrearchivo0, Nombrearchivo1, Nombrearchivo2, Nombrearchivo3, Nombrearchivo4, Nombrearchivo5;
int ano, dia, mes, horas, minutos, segundos, oldsegundos;
String Fecha, Hora, Imprimir, Imprimir3, ts, ts1;
/**variables genericas para capturar todas las medidas*/
float voltage, current, power, frequency, pf, current1, current2, current3, current4, current5, current6;

void writeFile(fs::FS& fs, const char* path, const char* message) {
  //  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    //  Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS& fs, const char* path, const char* message) {
  //Serial.println("APPENDFILE.....");
  // Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.println(message)) {
    //  Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}



void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void limpiar() {
  //borrar valores  por debajo del umbral de 0,06Amp
  if (current < 0.06)  //0.05
  {
    current = 0.00;
    power = 0.00;
    pf = 0.00;
  }

  if (power < 0.01)  //0.03
  {
    current = 0.00;
    power = 0.00;
    pf = 0.00;
  }
}



void setup() {

  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  int CONTADOR = 0;

  //gestion del interruptor para grabaciones
  toggleSwitch.setDebounceTime(50);  // set debounce time to 50 milliseconds
  Wire.begin(SDA, SCL);              // attach the IIC pin
  lcd.init();                        // LCD driver initialization
  lcd.backlight();                   // Turn on the backlight
  lcd.setCursor(0, 0);               // Move the cursor to row 0, column 0
  lcd.print("Inic WIFI.. ");         // The print content is displayed on the LCD
                                     //gestion de la wifi
  delay(2000);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) and (CONTADOR < 50)) {
    delay(1000);
    Serial.print(CONTADOR);
    Serial.print(":status=");
    Serial.println(WiFi.status());  //Agrega esta línea para imprimir el estado de la conexión WiFi
    Serial.print(" ");
    CONTADOR++;
    lcd.setCursor(0, 1);  // Move the cursor to row 0, column 0
    lcd.print(CONTADOR);  // The print content is displayed on the LCD
  }

  lcd.setCursor(0, 0);  // Move the cursor to row 1, column 0
  lcd.print("             ");
  lcd.setCursor(0, 1);  // Move the cursor to row 0, column 0
  lcd.print("     ");   // The print content is displayed on the LCD


  if (WiFi.status() != WL_CONNECTED)  //   (CONTADOR=1000000)
  {
    lcd.setCursor(0, 0);       // Move the cursor to row 1, column 0
    lcd.print("NO CONECT  ");  // The print content is displayed on the LCD
    Serial.println(" not CONNECTED");
    delay(1000);
  } else {

    lcd.setCursor(0, 0);       // Move the cursor to row 1, column 0
    lcd.print("CONECT OK  ");  // The print content is displayed on the LCD
    Serial.println(" CONNECTED");
    delay(5000);
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(4000);

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    //printLocalTime();
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    // Configura el servidor NTP y ajusta la zona horaria
    timeClient.begin();
    timeClient.setTimeOffset(3600);  // Ajusta la zona horaria a GMT+1

    //captura de la hora
    ano = timeinfo.tm_year + 1900;
    mes = timeinfo.tm_mon + 1;
    dia = timeinfo.tm_mday;
    horas = timeinfo.tm_hour;
    minutos = timeinfo.tm_min;
    segundos = timeinfo.tm_sec;
    Fecha = (String)ano + "/" + (String)mes + "/" + (String)dia;
    Hora = (String)horas + ":" + (String)minutos + ":" + (String)segundos;
  }


  //ficheros de datos
  Nombrearchivo0 = "/1_" + (String)ano + (String)mes + (String)dia + (String)horas + (String)minutos + (String)segundos + ".csv";  //agregado
  Nombrearchivo1 = "/6_" + (String)ano + (String)mes + (String)dia + (String)horas + (String)minutos + (String)segundos + ".csv";  //soladdor
  Nombrearchivo2 = "/5_" + (String)ano + (String)mes + (String)dia + (String)horas + (String)minutos + (String)segundos + ".csv";  //led
  Nombrearchivo3 = "/4_" + (String)ano + (String)mes + (String)dia + (String)horas + (String)minutos + (String)segundos + ".csv";  //halohena
  Nombrearchivo4 = "/3_" + (String)ano + (String)mes + (String)dia + (String)horas + (String)minutos + (String)segundos + ".csv";  // multimedia
  Nombrearchivo5 = "/2_" + (String)ano + (String)mes + (String)dia + (String)horas + (String)minutos + (String)segundos + ".csv";  //ventilador


  //inicializacion y gestion de la SD
  lcd.setCursor(0, 0);       // Move the cursor to row 1, column 0
  lcd.print("Inic SD    ");  // The print content is displayed on the LCD
  Serial.print("Inicializando tarjeta SD...");
  delay(3000);

  if (!SD.begin(5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  // Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    //   Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    //  Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    // Serial.println("SDHC");
  } else {
    // Serial.println("UNKNOWN");
  }


  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  lcd.setCursor(0, 0);  // Move the cursor to row 1, column 0
  lcd.print("SD=");
  lcd.print(cardSize);
  delay(2000);


  String Imprimir2 = "timestamp,VLN,A,W,f,PF\n";
  lcd.setCursor(0, 0);     // Move the cursor to row 1, column 0
  lcd.print("Files ...");  // The print content is displayed on the LCD
  delay(2000);

  //fichero 1  (MAIN)
  char Buf[50];
  Imprimir2.toCharArray(Buf, 50);
  char Buf2[50];
  Nombrearchivo0.toCharArray(Buf2, 50);
  writeFile(SD, Buf2, Buf);

  //fichero 2 (FAN )
  Imprimir2.toCharArray(Buf, 50);
  Nombrearchivo1.toCharArray(Buf2, 50);
  writeFile(SD, Buf2, Buf);

  //fichero3  (LAPTOP COMPUTER)
  Imprimir2.toCharArray(Buf, 50);
  Nombrearchivo2.toCharArray(Buf2, 50);
  writeFile(SD, Buf2, Buf);

  //fichero4 (INCANDESCENT LAMP)
  Imprimir2.toCharArray(Buf, 50);
  Nombrearchivo3.toCharArray(Buf2, 50);
  writeFile(SD, Buf2, Buf);


  //fichero5 (LED LAMP)
  Imprimir2.toCharArray(Buf, 50);
  Nombrearchivo4.toCharArray(Buf2, 50);
  writeFile(SD, Buf2, Buf);

  //fichero6 (TELEVISION)
  Imprimir2.toCharArray(Buf, 50);
  Nombrearchivo5.toCharArray(Buf2, 50);
  writeFile(SD, Buf2, Buf);


  /* RESET DEL CONTADOR DE ENERGIA */
  pzem0.resetEnergy();
  pzem1.resetEnergy();
  pzem2.resetEnergy();
  pzem3.resetEnergy();
  pzem4.resetEnergy();
  pzem5.resetEnergy();


  // Actualiza el tiempo del servidor NTP
  timeClient.update();

  // Obtiene el tiempo actual en formato Unix
  currentTimestamp = timeClient.getEpochTime();
  // Imprime el tiempo actual en la consola serie
  Serial.print("CurrentTimestamp");
  Serial.println(currentTimestamp);

  lcd.setCursor(0, 0);           // Move the cursor to row 1, column 0
  lcd.print("Fin setup      ");  // The print content is displayed on the LCD
  delay(2000);
  lcd.setCursor(0, 0);                      // Move the cursor to row 1, column 0
  lcd.print("                          ");  // The print content is displayed on the LCD
}


void retardo() {
  delay(3000);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
}




void loop() {

  toggleSwitch.loop();  // MUST call the loop() function first

  int state = toggleSwitch.getState();

  //le sumamos los ms que han pasado desde el inicio
  long long stimestamp = millis();
  long long newtimestamp = currentTimestamp * 1000;
  newtimestamp = newtimestamp + stimestamp;

  char buf[20];  // Make sure the buffer is big enough to hold the string
  dtostrf(newtimestamp, 0, 0, buf);
  Serial.print("TimeStamp actual:");
  Serial.println(buf);



  char Buf[50];
  char Buf2[50];
  float puerto;

  /** MEDIDOR 0 MAIN */
  /** Medimos el consumo agregado de todos los aplicativos */
  puerto = pzem0.readAddress();
  if ((puerto > 0) && (state == HIGH)) {
    digitalWrite(LED, HIGH);
    // Read the data from the sensor
    voltage = pzem0.voltage();  //Volt
    current = pzem0.current();  //Amp
    power = pzem0.power();      //watt
    limpiar();
    Serial.print("M0 AGREGADO  I=");
    Serial.print(current);
    Serial.print(" W=");
    Serial.print(power);
    //float energy = pzem0.energy(); //kwh
    frequency = pzem0.frequency();  //hz
    pf = pzem0.pf();
    //salvamos a la sd
    Imprimir3 = (String)buf + "," + (String)voltage + "," + (String)current + "," + (String)power + "," + (String)frequency + "," + (String)pf;
    /* escritura en la SD */
    Imprimir3.toCharArray(Buf, 50);
    //Serial.println (Buf);
    Nombrearchivo0.toCharArray(Buf2, 50);
    appendFile(SD, Buf2, Buf);
    ca++;
    current1 = current;
  }




  /** MEDIDOR 1  FRYER  SMALL RESISTENCE*/
  puerto = pzem1.readAddress();
  if ((puerto > 0) && (state == HIGH)) {
    /** MEDIDOR 1*/
    digitalWrite(LED, HIGH);
    // Read the data from the sensor
    voltage = pzem1.voltage();  //Volt
    current = pzem1.current();  //Amp
    power = pzem1.power();      //watt
    //float energy = pzem0.energy(); //kwh
    frequency = pzem1.frequency();  //hz
    pf = pzem1.pf();
    limpiar();
    Serial.print("-M1 SOLDADOR I=");
    Serial.print(current);
    Serial.print(" W=");
    Serial.print(power);
    //salvamos a la sd
    Imprimir3 = (String)buf + "," + (String)voltage + "," + (String)current + "," + (String)power + "," + (String)frequency + "," + (String)pf;
    /* escritura en la SD */
    Imprimir3.toCharArray(Buf, 50);
    //Serial.println (Buf);
    Nombrearchivo1.toCharArray(Buf2, 50);
    appendFile(SD, Buf2, Buf);
    /* FIM MEDIDOR 1*/
    cb++;
    current2 = current;
  }






  /** MEDIDOR 2 LED LAMP*/
  puerto = pzem2.readAddress();
  if ((puerto > 0) && (state == HIGH)) {
    digitalWrite(LED, HIGH);
    // Read the data from the sensor
    voltage = pzem2.voltage();  //Volt
    current = pzem2.current();  //Amp
    power = pzem2.power();      //watt
    limpiar();
    Serial.print("-M2 LED I=");
    Serial.print(current);
    Serial.print(" W=");
    Serial.print(power);
    //float energy = pzem0.energy(); //kwh
    frequency = pzem2.frequency();  //hz
    pf = pzem2.pf();
    //salvamos a la sd
    Imprimir3 = (String)buf + "," + (String)voltage + "," + (String)current + "," + (String)power + "," + (String)frequency + "," + (String)pf;
    /* escritura en la SD */
    Imprimir3.toCharArray(Buf, 50);
    // Serial.println (Buf);
    Nombrearchivo2.toCharArray(Buf2, 50);
    appendFile(SD, Buf2, Buf);
    /* FIM MEDIDOR 2*/
    cc++;
    current3 = current;
  }




  /** MEDIDOR 3 INCANDESCENT LAMP */
  puerto = pzem3.readAddress();
  if ((puerto > 0) && (state == HIGH)) {
    digitalWrite(LED, HIGH);
    // Read the data from the sensor
    voltage = pzem3.voltage();  //Volt
    current = pzem3.current();  //Amp
    power = pzem3.power();      //watt
    limpiar();
    Serial.print("-M3 LAMP I=");
    Serial.print(current);
    Serial.print(" W=");
    Serial.print(power);
    frequency = pzem3.frequency();  //hz
    pf = pzem3.pf();
    //salvamos a la sd
    Imprimir3 = (String)buf + "," + (String)voltage + "," + (String)current + "," + (String)power + "," + (String)frequency + "," + (String)pf;
    /* escritura en la SD */
    Imprimir3.toCharArray(Buf, 50);
    Nombrearchivo3.toCharArray(Buf2, 50);
    appendFile(SD, Buf2, Buf);
    /* FIM MEDIDOR 3*/
    cd++;
    current4 = current;
  }


  /** MEDIDOR 4  LAPTOP*/
  puerto = pzem4.readAddress();
  if ((puerto > 0) && (state == HIGH)) {
    digitalWrite(LED, HIGH);
    voltage = pzem4.voltage();  //Volt
    current = pzem4.current();  //Amp
    power = pzem4.power();      //watt
    limpiar();
    Serial.print("-M4 LAPTOP I=");
    Serial.print(current);
    Serial.print(" W=");
    Serial.print(power);
    //float energy = pzem4.energy(); //kwh
    frequency = pzem4.frequency();  //hz
    pf = pzem4.pf();
    //salvamos a la sd
    Imprimir3 = (String)buf + "," + (String)voltage + "," + (String)current + "," + (String)power + "," + (String)frequency + "," + (String)pf;
    /* escritura en la SD */
    Imprimir3.toCharArray(Buf, 50);
    //  Serial.println (Buf);
    Nombrearchivo4.toCharArray(Buf2, 50);
    appendFile(SD, Buf2, Buf);
    /* FIM MEDIDOR 4*/
    ce++;
    current5 = current;
  }

  //* MEDIDOR 5 FAN //
  puerto = pzem5.readAddress();
  if ((puerto > 0) && (state == HIGH)) {
    digitalWrite(LED, HIGH);
    // Read the data from the sensor
    voltage = pzem5.voltage();  //Volt
    current = pzem5.current();  //Amp
    power = pzem5.power();      //watt
    limpiar();
    Serial.print("-M5 FAN I=");
    Serial.print(current);
    Serial.print(" W=");
    Serial.print(power);
    frequency = pzem5.frequency();  //hz
    pf = pzem5.pf();
    //salvamos a la sd
    Imprimir3 = (String)buf + "," + (String)voltage + "," + (String)current + "," + (String)power + "," + (String)frequency + "," + (String)pf;
    /* escritura en la SD */
    Imprimir3.toCharArray(Buf, 50);
    //Serial.println (Buf);
    Nombrearchivo5.toCharArray(Buf2, 50);
    appendFile(SD, Buf2, Buf);
    /* FIM MEDIDOR 5*/
    cf++;
    current6 = current;
  }

  Serial.println();

  //pintamos en el lcd los valores de la corriente de todas las medidas

  //borra contenido de pantalla
  lcd.setCursor(0, 0);  // Move the cursor to row 1, column 0
  lcd.print("               ");
  lcd.setCursor(0, 0);  // Move the cursor to row 1, column 0
  lcd.print("A=");
  lcd.print(current1);

  if (current2 != 0) {
    lcd.print("v=");
    lcd.print(current2);
  }

  if (current3 != 0) {
    lcd.print("m=");
    lcd.print(current3);
  }

  //borra contenido de pantalla
  lcd.setCursor(0, 1);  // Move the cursor to row 1, column 0
  lcd.print("               ");

  lcd.setCursor(0, 1);  // Move the cursor to row 1, column 0

  if (current4 != 0) {
    lcd.print("h=");  //halogena
    lcd.print(current4);
  }

  if (current5 != 0) {
    lcd.print("l=");
    lcd.print(current5);
  }

  if (current6 != 0) {
    lcd.print("s=");
    lcd.print(current6);
  }


  if (state == LOW) {
    digitalWrite(LED, LOW);
  }

  //comprobacion de lecturas para cada medida
  if ((ca == cb) && (cb == cc) && (cc == cd) && (cd == ce) && (ce == cf)) {
    Serial.println("Lecturas OK:");  // The count is displayed every second
                                     //esta ok
  } else {                           //error al desnivelarse las muestras entre contadores
    Serial.print("** POSIBLE   DESFASE **");
    Serial.println("M0=" + (String)ca + ";M1" + (String)cb + ";M2" + (String)cc + ";M3" + (String)cd + ";M4" + (String)ce + ";M5" + (String)cf + ";");
  }
}
