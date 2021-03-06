//Telegram bot with sensor only

#include <WiFiNINA.h>
#include <UniversalTelegramBot.h>
#include <Adafruit_Sensor.h>  //for sensor
#include <DHT.h>            //for sensor
#include <DHT_U.h>  



char SSID[] = " ";                            // ENTER NETWORK SSID & PASS
char PASS[] = " ";

#define BOTtoken " "    //YOUR BOT TOKEN 
 
#define DHTPIN  5
#define DHTTYPE DHT11


WiFiSSLClient client;
UniversalTelegramBot bot(BOTtoken, client);
int Bot_mtbs = 1000;
long Bot_lasttime;

// Sensor configuration
DHT dht(DHTPIN, DHTTYPE);
float myTemperature;
float myHumidity;

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
 
  
}
void loop() {

  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    dht.begin();
    myTemperature = dht.readTemperature();
    myHumidity = dht.readHumidity();
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("Got response from Telegram bot");
      for (int i = 0; i < numNewMessages; i++) {

        if (bot.messages[i].text == "/TEMP")
          bot.sendMessage(bot.messages[i].chat_id, "Temperature is " + String(myTemperature) + "*C", "");

        if (bot.messages[i].text == "/HUMID")
          bot.sendMessage(bot.messages[i].chat_id, "Humidity is " + String(myHumidity) + "%", "");
        
        if (bot.messages[i].text == "help")
        {
          bot.sendMessage(bot.messages[i].chat_id, "Hello ASSEMBLY attendee! I am your bot! \n Use following commands: \n /ON - turns on the LED connected \n /OFF - turns off LED connected \n /TEMP - gives temperature \n /HUMID - gives humidity \n /Increase - increases LED brightness \n /Decrease - decreases LED brightness \n /FADE - shows led fading \n /ONservo - pushes servo out \n /OFFservo - pulls servo back \n /TRYservo - test servo", "");
        }


        Serial.println("User sent " + bot.messages[i].text);

      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
}
