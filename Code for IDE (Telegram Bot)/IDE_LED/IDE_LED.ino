//This is for the Telegram Bot with LED alone (ON, OFF, FADE)


#include <WiFiNINA.h>  //for creating wifi connection
#include <UniversalTelegramBot.h> //for the Telegram bot
char SSID[] = " ";    //insert network name
char PASS[] = " ";    //insert network password

#define BOTtoken " "    //insert your unique BOT token
#define LEDPIN 2

WiFiSSLClient client;
UniversalTelegramBot bot(BOTtoken, client);
int Bot_mtbs = 1000;
long Bot_lasttime;

void setup()
{
  Serial.begin(9600);     //CHECK YOUR SERIAL MONITOR SHOULD BE 9600
  delay(2000);            //delay for the output to send at baud rate
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
  
  pinMode(PIN, OUTPUT);     //set LED pin as output
 
}


void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)
  {

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
analogWrite(LEDPIN,pulse);
}
