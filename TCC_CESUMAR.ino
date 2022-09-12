/*
programa ok
TCC Unicesumar


*/
#define BLYNK_TEMPLATE_ID "TMPL2T0ePA-W"
#define BLYNK_DEVICE_NAME "TemplateMarcelo"
#define BLYNK_AUTH_TOKEN "C8R8rASddou7A_QSCkCEEkTTpFThzNMg"

#define BLYNK_PRINT Serial                      //Define o serial do Blynk
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <WiFi.h>                               //Inclusão das bibliotecas
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Definição dos pinos do display TFT
#define TFT_DC      2       // register select (stands for Data Control perhaps!)
#define TFT_RST     4       // Display reset pin
#define TFT_CS      5       // Display enable (Chip select), if not enabled will not talk on SPI bus
#define TFT_SCK     18      // Pino SCK ou CLK do display TFT no Pino D3 do NodeMCU
#define TFT_SDA     23      // Pino SDA do display TFT no Pino D6 do NodeMCU
#define TFT_A0      2       // Pino A0 ou RS do display TFT no Pino D4 do NodeMCU
#define TFT_RESET   4       // Pino RESET ou RST do display TFT no Pino D8 do NodeMCU
#define TFT_CS      5       // Pino CS do display TFT no Pino D5 do NodeMCU

// Definição das cores
#define PRETO     0x0000
#define AZUL      0x0C3A
#define CINZA     0xD6BA
#define MAGENTA   0xF81F
#define VERMELHO  0xF800 
#define BRANCO    0xFFFF

// initialise the routine to talk to this display with these pin connections (as we've missed off
// TFT_SCLK and TFT_MOSI the routine presumes we are using hardware SPI and internally uses 13 and 11
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST); 
unsigned char tempAmbiente,a,b=22,c;
//////////////////

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);






//////////////////////////////
hw_timer_t * timer = NULL;

void cb_timer(){
    static unsigned int counter = 1;
    Serial.println("cb_timer(): ");
    Serial.print(counter);
    Serial.print(": ");
    Serial.print(millis()/500);
    Serial.println(" MEIO segundos");

    if (counter == 15){
        stopTimer();
    }
    counter++;
}

void startTimer(){
    //inicialização do timer. Parametros:
    /* 0 - seleção do timer a ser usado, de 0 a 3.
      80 - prescaler. O clock principal do ESP32 é 80MHz. Dividimos por 80 para ter 1us por tick.
    true - true para contador progressivo, false para regressivo
    */
    timer = timerBegin(0, 80, true);

    /*conecta à interrupção do timer
     - timer é a instância do hw_timer
     - endereço da função a ser chamada pelo timer
     - edge=true gera uma interrupção
    */
    timerAttachInterrupt(timer, &cb_timer, true);

    /* - o timer instanciado no inicio
       - o valor em us para 1s
       - auto-reload. true para repetir o alarme
    */
    timerAlarmWrite(timer, 1000000, true); 

    //ativa o alarme
    timerAlarmEnable(timer);
}

void stopTimer(){
    timerEnd(timer);
    timer = NULL; 
}

//////////////////////////////
char auth[] = BLYNK_AUTH_TOKEN;                      //Inclua aqui o token recebido por email
char ssid[] = "Protecoteste";               //Insira aqui o nome da sua rede Wi-Fi
char pass[] = "!zl431A200";              //Insira aqui a senha da sua rede Wi-Fi

void setup() {
  startTimer();//inicia timer
  
  Serial.begin(9600);                           //Inicialização do Monitor Serial
  Blynk.begin(auth, ssid, pass);                //Inicia o Blynk

  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
///////////
 // Start the DS18B20 sensor
  sensors.begin();
///////////
  //Inicializa o display TFT
  tft.initR(INITR_BLACKTAB);
  tft.fillRect( 0, 0, 128, 160, PRETO);

  
  pinMode(23, OUTPUT);                          //Definição dos pinos a serem utilizados (botão estará na porta 21)
  pinMode(22, OUTPUT);
  pinMode(21, INPUT);
}

void loop() {
  Blynk.run();                                  //Ativa o Blynk
  if (digitalRead(21)) {                        //Verifica se o botão foi pressionado
    Blynk.notify("O botao foi pressionado");    //Se sim, envia uma notificação ao app
  }
    tft.setTextColor(BRANCO);
  tft.setCursor(2,0);
  tft.setTextSize(1);
  tft.println("TCC ENG. DE SOFTWARE");
  tft.setCursor(2,15);
  tft.println("ALUNO:MARCELO GARBULI");
  tft.setTextColor(MAGENTA);
  tft.setCursor(0,30);
  tft.println("=====================");
  tft.setCursor(0,45);
  tft.setTextColor(CINZA);
  tft.print("TEMP. AMBIENTE=");
  tft.setCursor(0,60);
  tft.print("UMIDADE SOLO=");
  tft.setTextColor(MAGENTA);
  tft.setCursor(0,75);
  tft.println("=====================");
  tft.setTextColor(BRANCO);
  tft.setCursor(0,90);
  tft.printf("BOMBA LIGADA? %2u",tempAmbiente);
  //tft.print("NODEMCU + ST7735 TFT");
  //tft.printf( "%02u:%02u:%02u", a,b,c );

  tft.setCursor(0,105);
  tft.print("ENVIANDO DADOS");

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);

  
}
