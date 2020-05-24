#include <ESP8266WiFi.h>  //导入库
#include <WiFiClient.h>
#define AP_SSID "TP-LINK_DA5E" //这里改成你的wifi名字
#define AP_PSW "feihu2016*."//这里改成你的wifi密码
#define GPIO 2

WiFiClient myClient;
const char* server="192.168.1.107";
String ip = "www.baidu.com";
const int port=8080;
int mybegin=0;

void setup() { 
    Serial.begin(9600);
    //Serial.println("hhhhhh");
    WiFi.mode(WIFI_STA);     //设置模式为STA
    delay(2000);
    WiFi.begin(AP_SSID, AP_PSW);  //启动STA模式，并连接wifi网络
    ////判断网络状态是否连接上，没连接上就延时500ms，并且打出一个点，模拟连接过程
    while (WiFi.status() != WL_CONNECTED){   //判断wifi是否连接成功
        delay(500);
    }
    pinMode(GPIO,INPUT);
}

 
void loop() { 
      //Serial.println("I am loop");
      if(myClient.connected()==0)
      {
          mybegin=1;//说明连接断了
          while(myClient.connect(server,port) != 1)
          {
             //myClient.stop();
             //Serial.println("I am connecting!");
          }
      }
      
      if(mybegin==1)
      {
          myClient.print("{\"hwStatus\":\"begin\",\"clinical\":\"hw\",\"flag\":\"aa\"}");
          mybegin=0;
      }
      bool s=1;
      if((s=check())==0)//如果有水
      {
          int i=20;
          while(myClient.connected()&&i>0)
          {
              s=check();
              if(s != false)
              {
                  myClient.print("{\"hwStatus\":\"end\",\"clinical\":\"hw\",\"flag\":\"aa\"}");
                  Serial.print("E"); 
                  i--;
               }
              delay(8000);
          }
      }
       
      delay(10000);
      //myClient.stop();
 }

 
 bool check()
 {
      int T=100;
      bool st=0;
      int sumture=0;
      int sumfalse=0;
      while(T>0)
      {
          st = digitalRead(GPIO);
          if(st)
          {
              sumture++; 
          }
          else
          {
              sumfalse++; 
          }
          T--; 
      }
      if(sumture>sumfalse)
      {return 1;}
      else
      {return 0;}
 }
