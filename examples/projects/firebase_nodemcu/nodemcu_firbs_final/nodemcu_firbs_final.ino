/*
burn this code to nodemcu esp8266.
*/


#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>                                        
#include<SoftwareSerial.h> //Included SoftwareSerial Library


#define FIREBASE_HOST "<your-project>.firebaseio.com"              // the project name address from firebase id
#define FIREBASE_AUTH "<Your-firbase-secret-code>"       // the secret key generated from firebase
#define WIFI_SSID "VEGA"                                              // your ssid                     
#define WIFI_PASSWORD "CDAC1234"                                   //password
                          
SoftwareSerial NodeMCU(13,15);                                       //RX,TX pins of nodemcu
char data,arr[15],a=0;
int indx =0;
char flag=0;
 
String fireStatus = "";                                                     // led status received from firebase                                                     
void setup() 
{
   Serial.begin(9600);
   NodeMCU.begin(9600);                                                    // initialization
   Serial.println("tx rx begin");                                   
   
                   
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                               
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    
      while (WiFi.status() != WL_CONNECTED) 
      {
        Serial.print(".");
        delay(500);
      }
      
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
  Firebase.setString("LED_STATUS", "OFF");                       //send initial string of led status 
}
 
void loop() 
{

//..... communication between nodemcu and vegaboard....
 
 if (NodeMCU.available()>0) // to check the status of vega
  {  
    if(flag==0)
     {
      read_RX(); 
      flag=1;
     }
     delay(500);
    NodeMCU.write("Start\n"); // initial signal to start data txn
    
    
     read_RX(); // initial read command
     
     Serial.println(arr); // string
     Firebase.setString("New Message",arr); // updated in firebase         ......communication to firebase from nodemcu............
     if (Firebase.failed())
     {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());  
        return;
     }
      
  
     NodeMCU.write("Next\n"); // signal for next data to be transmitted
    

      read_RX(); // next read command
     //Serial.println(atoi(arr)); // to integer
     //Serial.println(atof(arr)); // to float

    //communication to firebase from nodemcu............
     
    Firebase.setString("NEW message", arr);
    Firebase.setFloat("number", (atof(arr)+a)); // updated to firebase
   a++;

      if (Firebase.failed())
      {
        Serial.print("setting /number failed:");
        Serial.println(Firebase.error());  
        return;
     }
      
     
    Serial.println("ok");

  //communication from firebase to nodemcu............
  
  fireStatus = Firebase.getString("LED_STATUS");                                      // get ld status input from firebase
  
    if (fireStatus == "ON") 
    {                                                          // compare the input of led status received from firebase
      Serial.println("Led Turned ON");                                                        
    }                                                          
    else if (fireStatus == "OFF") 
    {                                                  // compare the input of led status received from firebase
      Serial.println("Led Turned OFF");
    }
    else 
    {
      Serial.println("Command Error! Please send ON/OFF");
    }

    }
 }



char read_RX()
{
  if (NodeMCU.available()>0)
    {
     do{  
          
          data = NodeMCU.read();
          arr[indx]=data;
          indx++;
          delay(5);
          
          
        } while(data !='\n');
        
     arr[indx-1]='\0';
     indx=0;
    }
    
}
