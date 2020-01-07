//This is Telegram with Servo

#include <WiFiNINA.h>
#include <UniversalTelegramBot.h>
#include <Servo.h>        //only for Servo


char SSID[] = " ";                            // ENTER NETWORK SSID & PASS
char PASS[] = " ";

#define BOTtoken " "  //YOUR BOT TOKEN 
#define servoPin 13


WiFiSSLClient client;
UniversalTelegramBot bot(BOTtoken, client);
int Bot_mtbs = 1000;
long Bot_lasttime;

Servo myservo;                                // create servo object to control a servo


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
  myservo.attach(servoPin);                   // attaches the servo on pin 13 to the servo object
  myservo.write(0);                           // sets the servo position
}
void loop() {

  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("Got response from Telegram bot");
      for (int i = 0; i < numNewMessages; i++) {

       

         if (bot.messages[i].text == "/ONservo") {
          myservo.write(90);                     // sets the servo position
          bot.sendMessage(bot.messages[i].chat_id, "Servo is ON", "");
        }

        if (bot.messages[i].text == "/OFFservo") {
          myservo.write(0);                     // sets the servo position
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
          bot.sendMessage(bot.messages[i].chat_id, "Hello ASSEMBLY attendee! I am your bot! \n Use following commands: \n /ON - turns on the LED connected \n /OFF - turns off LED connected \n /TEMP - gives temperature \n /HUMID - gives humidity \n /Increase - increases LED brightness \n /Decrease - decreases LED brightness \n /ONservo - pushes servo out \n /OFFservo - pulls servo back \n /TRYservo - test servo", "");
        }


        Serial.println("User sent " + bot.messages[i].text);

      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }

}
