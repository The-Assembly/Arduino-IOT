#include <WiFiNINA.h>
#include <UniversalTelegramBot.h>
#include <Adafruit_Sensor.h>  //for sensor
#include <DHT.h>            //for sensor
#include <DHT_U.h>          //for sensor
#include <Servo.h>        //only for Servo


char SSID[] = "";                            // ENTER NETWORK SSID & PASS
char PASS[] = "";

#define BOTtoken ""  //YOUR BOT TOKEN 
#define LEDPIN 2    //YOUR PIN NUMBER ON BOARD
#define DHTPIN  5
#define LDRPIN A0
#define DHTTYPE DHT11
#define servoPin 13


WiFiSSLClient client;
UniversalTelegramBot bot(BOTtoken, client);
int Bot_mtbs = 1000;
long Bot_lasttime;

Servo myservo;                                // create servo object to control a servo

int pulse = 0;       //pulse initial value

// Seensor configuration
DHT dht(DHTPIN, DHTTYPE);
float myTemperature;
float myHumidity;
int LDRvalue;


void setup()
{
  Serial.begin(9600);     //YOUR SERIAL MONITOR SHOULD BE 9600
  delay(2000);
  Serial.print("Connecting to Wifi Network: ");
  Serial.println(SSID);
  while (WiFi.begin(SSID, PASS) != WL_CONNECTED)
  {
    Serial.println(".. Loading connection .. ");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  Serial.println(" ");
  pinMode(LEDPIN, OUTPUT);
  myservo.attach(servoPin);                   // attaches the servo on pin 13 to the servo object
  myservo.write(0);                           // sets the servo position
}
void loop() {

  LDRvalue = analogRead(LDRPIN);



  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    dht.begin();
    myTemperature = dht.readTemperature();
    myHumidity = dht.readHumidity();
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("Got response from Telegram bot");
      for (int i = 0; i < numNewMessages; i++) {

        if (bot.messages[i].text == "/ON") {
            pulse = pulse+255;              
            bot.sendMessage(bot.messages[i].chat_id,"LED is ON!", "");
            Serial.println(pulse); // will print the value of the pulse in the serial monitor 
          }
        if (bot.messages[i].text == "/Increase")  { // if you sent this command to your bot
          if(pulse <= 225 ) //if pulse is smaller than or equal to 255
          {
            pulse = pulse+30;            // add 30 to the pulse
            bot.sendMessage(bot.messages[i].chat_id,"Brightness Increased!", ""); // the message the bot will send to you
            Serial.println(pulse); // will print the value of the pulse in the serial monitor 
          }
           
          else
          {
            pulse = 255; // if the first condition did not apply then pulse will be 255
            bot.sendMessage(bot.messages[i].chat_id, "Cannot increase further! ", "");
          }
       
        }
          if (bot.messages[i].text == "/Decrease") {   // if you sent this command to your bot
            if(pulse >= 30 ) // if the pulse is greater than or equal to 30
            {
              pulse = pulse -30 ;  // substract 30               
              bot.sendMessage(bot.messages[i].chat_id, "Brightness Decreased!", "");
              Serial.println(pulse); // will print the value of the pulse in the serial monitor 
            
            }  
           else 
            {
              pulse = 0; // otherwise the pulse is 0
              bot.sendMessage(bot.messages[i].chat_id, "Cannot decrease further! ", ""); // the bot will notifiy you that LED cannot be decreased further
            }

           
            }
                if (bot.messages[i].text == "/OFF") { // if you text the bot /OFF 
                pulse = 0; // the pulse will be set to 0 (it will be turned off)             
                bot.sendMessage(bot.messages[i].chat_id,"LED is OFF!", ""); // The bot will notifiy you that it is turned off
                Serial.println(pulse); // will print the value of the pulse in the serial monitor 
          }


if (bot.messages[i].text == "/FADE")
       {

        pulse = 0;
        while (pulse < 255)
        {
          pulse = pulse + 40;
          analogWrite(LEDPIN, pulse);
          delay(300);
        }
        while (pulse > 0)
        {
          pulse = pulse - 40;
          analogWrite(LEDPIN, pulse);
          delay(300);
        }
        pulse = 0;

        bot.sendMessage(bot.messages[i].chat_id,"LED has faded!", ""); // The bot will notifiy you that it is turned off
                
       
       }
        if (bot.messages[i].text == "/TEMP")
          bot.sendMessage(bot.messages[i].chat_id, "Temperature is " + String(myTemperature) + "*C", "");

        if (bot.messages[i].text == "/HUMID")
          bot.sendMessage(bot.messages[i].chat_id, "Humidity is " + String(myHumidity) + "%", "");

        if (bot.messages[i].text == "/LDR")
        {
          if (LDRvalue <= 200)
            bot.sendMessage(bot.messages[i].chat_id, "DARK : Analog Value = " + String(LDRvalue), "");

          else if (LDRvalue > 200 && LDRvalue <= 500)
            bot.sendMessage(bot.messages[i].chat_id, "DIM LIGHT : Analog Value = " + String(LDRvalue), "");

          else if (LDRvalue > 500 && LDRvalue <= 800)
            bot.sendMessage(bot.messages[i].chat_id, "BRIGHT LIGHT : Analog Value = " + String(LDRvalue), "");

          else if (LDRvalue > 800)
            bot.sendMessage(bot.messages[i].chat_id, "FULL DAY LIGHT : Analog Value = " + String(LDRvalue), "");

        }

         if (bot.messages[i].text == "/ONservo") {
          myservo.write(90);                     // sets the servo position
          bot.sendMessage(bot.messages[i].chat_id, "Servo is ON", "");
        }

        if (bot.messages[i].text == "/OFFservo") {
          myservo.detach();                     // sets the servo position
          bot.sendMessage(bot.messages[i].chat_id, "Servo is OFF", "");
        }

        if (bot.messages[i].text == "/TRYservo") {
          myservo.write(0);                     // sets the servo position
          delay(700);
          myservo.write(90);                    // sets the servo position
          delay(700);
          myservo.write(0);  
          delay(700);
          myservo.write(90);                    // sets the servo position
          delay(700);
          myservo.write(0); // sets the servo position
          bot.sendMessage(bot.messages[i].chat_id, "Servo tested", "");
        }

        if (bot.messages[i].text == "help")
        {
          bot.sendMessage(bot.messages[i].chat_id, "Hello ASSEMBLY attendee! I am your bot! \n Use following commands: \n /ON - turns on the LED connected \n /OFF - turns off LED connected \n /TEMP - gives temperature \n /HUMID - gives humidity \n /Increase - increases LED brightness \n /Decrease - decreases LED brightness \n  /FADE - shows led fading \n /ONservo - pushes servo out \n /OFFservo - pulls servo back \n /TRYservo - test servo", "");
        }


        Serial.println("User sent " + bot.messages[i].text);

      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
analogWrite(LEDPIN,pulse);
}
