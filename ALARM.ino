#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <math.h>
#define LED 5
#define MAX 14

// Replace with your network credentials
const char *ssid     = "Nokia 5.3";
const char *password = "fire4Yoga";
int State = 0;

struct Period
{
  int Hour,Minute;
}Periods[15];


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org");
int H_Periods[MAX] = {8 ,8 ,9 ,10,11,12,13,14,15,15,16,16,16,18};
int M_Periods[MAX] = {40,45,35,25,35,25,30,20,10,25,15,30,45,45};
//int H_Periods[MAX] = {20 ,20 ,21 ,21,21,21,22,23,23,23,23,23,23,23};
//int M_Periods[MAX] = {40,43, 33  ,34,40,43,30,0,5,15,30,45,47};

//int Period_Array[MAX];
  
int count =0;
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void Actuation(int delay_value)
{
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
//  Serial.print("LED OFF");
 delay(delay_value);                       // wait for a second
 digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
// Serial.println("LED ON");
 delay(delay_value);                       // wait for a second
}
void Min_index(struct Period X)
{
//  int tmp = Period_Array[0],diff;
  int tmp,diff;
  for(count = 0;count<MAX;count++)
  {
//    tmp = Period_Array[count];
    //diff = tmp - X.Hour * 60 + X.Minute;
    diff = ((H_Periods[count]*60)+M_Periods[count]) - ((X.Hour * 60 )+ X.Minute);
    Serial.print(diff);
    Serial.print('-');
    Serial.println(count);
    if(diff<0)
    {
      continue;
      }
    if(diff>=0)
    {
      //count;
      break;
      }
     
    }
  //  return count;
    
    
  }
void setup() {
  // Initialize Serial Monitor
  struct Period X;
/*  for(count = 0; count < MAX;count++)
  {
    Period_Array[count] = H_Periods[count] *60 + M_Periods[count];
    }
  */
  pinMode(LED, OUTPUT); 
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, 1);
  delay(100);
/*Periods[0].Hour = 20;
Periods[0].Minute = 50;
Periods[1].Hour = 20;
Periods[1].Minute = 45;
Periods[2].Hour = 20;
Periods[2].Minute = 57;
Periods[3].Hour = 20;
Periods[3].Minute = 58;
Periods[4].Hour = 20;
Periods[4].Minute = 46;
Periods[5].Hour = 20;
Periods[5].Minute = 35;
Periods[6].Hour =20 ;
Periods[6].Minute =25 ;
Periods[7].Hour = 20;
Periods[7].Minute = 21;
Periods[8].Hour = 21;
Periods[8].Minute = 25;
Periods[9].Hour = 21;
Periods[9].Minute = 10;
Periods[10].Hour = 21;
Periods[10].Minute = 25;
Periods[11].Hour = 21;
Periods[11].Minute = 15;
Periods[12].Hour = 21;
Periods[12].Minute = 30;
Periods[13].Hour = 21;
Periods[13].Minute = 45;
Periods[14].Hour = 21;
Periods[14].Minute = 45;*/

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
   //GMT +5 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(19800);
  
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
 // Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println(WiFi.macAddress());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   timeClient.forceUpdate();    
  String formattedTime = timeClient.getFormattedTime();
  X.Hour = timeClient.getHours();
  X.Minute = timeClient.getMinutes();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime); 
  //count  = Min_index(X);
  Min_index(X);
  Serial.println("Nearest Index...");
  Serial.println(count);
 
  digitalWrite(LED_BUILTIN, State);
  delay(1000);
  
  
       
 }
     
   

void loop() {
 //int index;
  //Serial.print("Found Index at :");
  //  Serial.print(count);
  //delay(1000); 
    timeClient.update();
    //index = TimeInit();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    int index_day = timeClient.getDay();
    //Serial.println(timeClient.getFormattedTime());
   // Serial.println(index);    
  //Actuation(10000);
   
  if(weekDays[index_day] != "Sunday")
  {
    if(count < MAX && currentHour == H_Periods[count])
    //if(count < 15 && currentHour == 23)
    
    {
      if(currentMinute == M_Periods[count])
      {
        State =~State;
        digitalWrite(LED_BUILTIN, State);

        Actuation(10000);
        if(count <MAX)
        { 
          Serial.println("Not Reached Max");
          Serial.println(count);
          
          while(timeClient.getSeconds())
          {
            delay(100);
          }
          count++;
          Serial.println(count);
          
        }
        else if(count >=MAX)
        {
          Serial.println("Reached Max");
          count = 0;
        }
        
       }
     }
 }

 // time_t epochTime = timeClient.getEpochTime();
  //Serial.print("Epoch Time: ");
  //Serial.println(epochTime);
  

  //Serial.print("Hour: ");
  //Serial.println(currentHour);  
  //Serial.print("Minutes: ");
  //Serial.println(currentMinute); 
   
  //int currentSecond = timeClient.getSeconds();
 // Serial.print("Seconds: ");
  //Serial.println(currentSecond);  

  //Serial.print("Week Day: ");
  
  /*//Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");
*/
  }




  int TimeInit()
{
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int j=0;
    //  0, 1 ,2, 3, 4 ,5 ,6, 7, 8 , 9,10,11,12,13
    //  8 ,8 ,9 ,10,11,12,13,14,15,15,16,16,17,18
    //  40,45,35,25,35,25,30,20,10,25,15,30,45,45

  Serial.println("TimeInit");

    for(count =0;count<=13;count++)
    {
     Serial.println(currentHour);
     delay(1000);
     if(currentHour >7 && currentHour <19)
     {
      

      if(currentHour - Periods[count].Hour == 0)
      { 
        Serial.println("Hourly   :");
        Serial.print(Periods[count].Hour);
        Serial.print("\t:");
        Serial.print(count);
        for(;(currentMinute - Periods[count+j].Minute)>0;j++)
        {
          Serial.println("Minite loop");
          
        Serial.print(Periods[count].Minute);
        Serial.print("\t:");
        Serial.print(count);
          Serial.print(currentMinute - Periods[count+j].Minute);
          }
        }
        else{
//        Serial.print(currentHour - Periods[count].Hour);

        continue;}
      }
      
     else
     {
      break;
      }
        Serial.print("Returning Time Init");

      return (count + j);
    }}
    /*  if(currentHour <= H_Periods[count] && currentMinute <=M_Periods[count])
      {
        Serial.print(count);
        delay(100);
        continue;}
       
      else
      {
        Serial.print("Found Index at ");
        Serial.print(count);
       delay(100);
        break;
        }
       
     if(currentHour <= H_Periods[count])
     {
      Serial.println("Hour Check");
     // Serial.println(currentHour);
      Serial.print(count);
      continue;
      }   
      else 
      {if(currentMinute <M_Periods[count])
        {
          Serial.println("Minute Check Break");
          Serial.print(count);
          break;
          }
      else
      { 
        Serial.println("Minute Check Continue");
        continue;
      }}    
      
    }
  }
 */