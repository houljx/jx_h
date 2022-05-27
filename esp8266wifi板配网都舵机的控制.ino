#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
 #include <Servo.h>//伺服舵机Servo库
Servo servo; // 建立Servo对象,对象名称是 'servo'
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
 
void setup(void){
  Serial.begin(115200);   // 启动串口通讯
 
  pinMode(LED_BUILTIN, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
  
  wifiMulti.addAP("TP-LINK_A7ED", "xiang/*0621?"); // 将需要连接的一系列WiFi ID和密码输入这里
//  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
//  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP  便于观察WiFi链接情况
 
  esp8266_server.begin();                           // 启动网站服务
  esp8266_server.on("/", HTTP_GET, handleRoot);     // 设置服务器根目录即'/'的函数'handleRoot'
  esp8266_server.on("/LED", HTTP_GET, handleLED); // 设置处理LED控制请求的函数'handleLED' 意思就是客户端发过来的信息进行处理 这是第一个按钮信息
  esp8266_server.on("/LED2", HTTP_GET, handleLED2);// 设置处理LED2控制请求的函数'handleLED2'意思就是客户端发过来的信息进行处理 这是第二个按钮信息
  //esp8266_server.on("/LED2", HTTP_POST, handleLED3);// 设置处理LED2控制请求的函数'handleLED2'意思就是客户端发过来的信息进行处理 这是第二个按钮信息
  esp8266_server.onNotFound(handleNotFound);        // 设置处理404情况的函数'handleNotFound'  崩溃信息
 
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动  观察ESP8266网络服务是否启动成功
}
 
void loop(void){
 servo.attach(D0);
  esp8266_server.handleClient();                     // 检查http服务器访问   一直检查有没有客服端访问服务器
}
 
/*设置服务器根目录即'/'的函数'handleRoot'
  该函数的作用是每当有客户端访问NodeMCU服务器根目录时，
  NodeMCU都会向访问设备发送 HTTP 状态 200 (Ok) 这是send函数的第一个参数。
  同时NodeMCU还会向浏览器发送HTML代码，以下示例中send函数中第三个参数，
  也就是双引号中的内容就是NodeMCU发送的HTML代码。该代码可在网页中产生LED控制按钮。 
  当用户按下按钮时，浏览器将会向NodeMCU的/LED页面发送HTTP请求，请求方式为POST。
  NodeMCU接收到此请求后将会执行handleLED函数内容*/
void handleRoot() {
  
    String str=
    "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>Welcome home</title><style>.button{background-color:DodgerBlue;border-radius:12px;color:white;padding:130px 150px;text-align:center;text-decoration:none;display:inline-block;font-size:80px;margin:4px 2px;cursor:pointer}.button:hover{background-color:Plum}.button2{background-color:Maroon}.button:active{background-color:DodgerBlue;box-shadow:0 5px#666;transform:translateY(4px);</style></head><body><h1 style=\"text-align:center;color:DarkOrchid  ;font-size:40px;\">欢迎回家Welcome home<div align=\"center\"><a href=\"/LED\"method=\"POST\"class=\"button\">开门</a><head></head><div align=\"center\"><a href=\"/LED2\"method=\"POST\"class=\"button button2\">关门</a><head>";
    esp8266_server.send(200, "text/html", str); //给客户端返回一段信息，这段信息是一段HTML文件
}
 
//处理LED控制请求的函数'handleLED'
void handleLED() {                                    // 假如按第一个按钮执行以下代码
  
  servo.write(180);delay(300);                        // 控制舵机旋转到180度
  servo.write(0);delay(300);                          // 控制舵机旋转到0度
  Serial.println("正在输出LED");
  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转 
} 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){                                  
  esp8266_server.send(404, "text/plain", "404: Not found"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}
void handleLED2() {  // 假如按第二个按钮执行以下代码
    servo.write(90);delay(300);                          // 控制舵机旋转回到90度初始位置
    Serial.println("正在输出LED2");                       //打印数据便于观察
    esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
    esp8266_server.send(303); 
    // 发送Http相应代码303 跳转 
  }
void handleLED3() {                                    // 开发板按钮执行以下代码
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));// 改变LED的点亮或者熄灭状态 
  Serial.println("正在输出LED3");
  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转 
} 
