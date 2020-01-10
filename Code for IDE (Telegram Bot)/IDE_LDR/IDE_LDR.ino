//Telegram Bot with Light-Dependent-Resistor

#include <WiFiNINA.h>
#include <UniversalTelegramBot.h>

char SSID[] = " ";                            // ENTER NETWORK SSID & PASS
char PASS[] = " ";

#define BOTtoken " "  //YOUR BOT TOKEN 
#define LDRPIN A0



WiFiSSLClient client;
UniversalTelegramBot bot(BOTtoken, client);
int Bot_mtbs = 1000;      //mean time between messages
long Bot_lasttime;        //last scan messages which is done

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
                          
}
void loop() {

  LDRvalue = analogRead(LDRPIN);



  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("Got response from Telegram bot");
      for (int i = 0; i < numNewMessages; i++) {

       
        if (bot.messages[i].text == "/LDR")     //NOTE to give the LDR ~30 seconds time
        {
          if (LDRvalue <= 200)      //lowest detection of LDR are below 200 (if you cover it with your finger and wait)
            bot.sendMessage(bot.messages[i].chat_id, "DARK : Analog Value = " + String(LDRvalue), "");

          else if (LDRvalue > 200 && LDRvalue <= 500)
            bot.sendMessage(bot.messages[i].chat_id, "DIM LIGHT : Analog Value = " + String(LDRvalue), "");

          else if (LDRvalue > 500 && LDRvalue <= 800)
            bot.sendMessage(bot.messages[i].chat_id, "BRIGHT LIGHT : Analog Value = " + String(LDRvalue), "");

          else if (LDRvalue > 800)    //highest detection of LDR is above 800 (if you point a flash-light and wait)
            bot.sendMessage(bot.messages[i].chat_id, "FULL DAY LIGHT : Analog Value = " + String(LDRvalue), "");

        }

        
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
