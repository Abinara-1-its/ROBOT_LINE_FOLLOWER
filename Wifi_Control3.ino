/*Program Control Robot Abinara 2022
 * Nara-5
 * ESP32 Dev Module
 */


#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

const char* ssid ="bukanwifi"; // Variabel nama SSID
const char* password = "ahmadhikam"; // Variable password SSID

char buff_tampil[30];
char buff_kirim[14]={'i','t','s'};

int CommandData[6];
int CommandData2;
int CommandData3;

int kirimIPServer;

String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//membuat task
//TaskHandle_t Task1;

WiFiServer server(80); // Deklarasi server dengan port 80
char StatusGerakDepan = 'P';
char StatusGerakBelakang = 'P';
char StatusGerakKanan = 'P';
char StatusGerakKiri = 'P';
char StatusGerakRotasiKiri = 'P';
char StatusGerakRotasiKanan = 'P';
char StatusGerakChilling = 'P';
char StatusAmbil ='P';
char StatusSemprot = 'P';
char StatusTambahan = 'M';
char StatusTaruhKorban = 'P';
char StatusMajuPelan = 'P';
char StatusMundurPelan ='P';
int ESPMode=1;
float V,W,Z;

int CommandTotal1=0;
int CommandTotal2=0;

//RTOS(GAGAL karena reset2 terus)
//void loop2(void * parameter){
//  for(;;){
//    CommandTotal1=CommandData[0]+CommandData[1]+CommandData[2]+CommandData[3];
//    Serial.println(CommandTotal1);
//    CommandTotal2=CommandData2[0]+CommandData2[1]+CommandData2[2]+CommandData2[3];
//    if(CommandTotal1 == 12)Serial.println("Maju");
//    else if(CommandTotal1 == 44)Serial.println("Kiri");
//    else if(CommandTotal1 == 22)Serial.println("Kanan");
//    else if(CommandTotal1 == 33)Serial.println("Mundur");
//    else if(CommandTotal1 == 56)Serial.println("Maju Serong Kiri");
//    else if(CommandTotal1 == 34)Serial.println("Maju Serong Kanan");
//    else if(CommandTotal1 == 77)Serial.println("Mundur Serong Kiri");
//    else if(CommandTotal1 == 55)Serial.println("Mundur Serong Kanan");
//    else Serial.println("Berhenti");
//    if(CommandData2 == 111)Serial.println("Tambah Kecepatan");
//    else if(CommandData2 == 222)Serial.println("Kurangi Kecepatan");
//    else if(CommandData2 == 333)Serial.println("Tambah Langkah");
//    else if(CommandData2 == 444)Serial.println("Kurangi Langkah");
//    else if(CommandData2 == 555)Serial.println("Tambah Ketinggian");
//    else if(CommandData2 == 666)Serial.println("Tambah Kecepatan");
//    else Serial.println("tak ada tambahan");
    
//    Serial.print("Cmd\t"); Serial.print(CommandData[0]); Serial.print("\t"); Serial.print(CommandData[1]); Serial.print("\t"); Serial.print(CommandData[2]); Serial.print("\t"); Serial.println(CommandData[3]);  
//  }
//}


void setup() {
  Serial.begin(9600); // Komunikasi serial Baud Rate 115200
  delay(100); 
  delay(5000);
//  xTaskCreatePinnedToCore(loop2,"loop2",1000,NULL,1,&Task1,0);
  
  Serial.println("Menghubungi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Otentifikasi sambungan Wifi

  // Mengecek apakah Wifi tersambung
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi tersambung..!");
  Serial.print("Nomor IP: ");  Serial.println(WiFi.localIP()); // Nomor IP Modul
  server.begin();
  Serial.println("Memulai HTTP server");
  delay(5000);
  
}

void loop() {
  WiFiClient client = server.available(); 

if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
//    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
//        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Perintah dalam server
            if (header.indexOf("GET /MajuMaju") >= 0) {
              StatusGerakDepan = 'W';
              
            } else if (header.indexOf("GET /MajuStop") >= 0) {
              StatusGerakDepan = 'P';
              
            } else if (header.indexOf("GET /MundurMundur") >= 0) {
              StatusGerakBelakang = 'S';

            } else if (header.indexOf("GET /SemprotSemprot") >= 0){
              StatusSemprot = 'G';

            } else if (header.indexOf("GET /SemprotStop") >= 0){
              StatusSemprot = 'P';
              
            } else if (header.indexOf("GET /TaruhTaruh") >= 0) {
              StatusTaruhKorban = 'T';

            } else if (header.indexOf("GET /TaruhStop") >= 0) {
              StatusTaruhKorban = 'P';
              
            } else if (header.indexOf("GET /MundurStop") >= 0) {
              StatusGerakBelakang = 'P';
            
            }else if (header.indexOf("GET /ChillingChilling") >= 0){
              StatusGerakChilling = 'R';

            }else if (header.indexOf("GET /ChillingStop") >= 0){
              StatusGerakChilling ='P';
              
            }else if (header.indexOf("GET /KananKanan") >= 0) {
              StatusGerakKanan = 'D';
              
            } else if (header.indexOf("GET /KananStop") >= 0) {
              StatusGerakKanan = 'P';
            
            } else if (header.indexOf("GET /KiriKiri") >= 0) {
              StatusGerakKiri = 'A';
              
            } else if (header.indexOf("GET /KiriStop") >= 0) {
              StatusGerakKiri = 'P';

            } else if (header.indexOf("GET /Kecepatanplus") >= 0) {
              StatusTambahan = 'Z';
              V+=1;
            }

            else if (header.indexOf("GET /Kecepatanmin") >= 0) {
              StatusTambahan = 'X';
              V-=1;
            }

            else if (header.indexOf("GET /Langkahplus") >= 0) {
              StatusTambahan = 'C';
              W+=0.1;
            }

            else if (header.indexOf("GET /Langkahmin") >= 0) {
              StatusTambahan = 'V';
              W-=0.1;
            }

            else if (header.indexOf("GET /Tinggiplus") >= 0) {
              StatusTambahan = 'B';
              Z+=0.1;
            }

            else if (header.indexOf("GET /Tinggimin") >= 0) {
              StatusTambahan = 'N';
              Z-=0.1;
            }
            else if (header.indexOf("GET /RotateKiri") >= 0) {
              StatusGerakRotasiKiri = 'Q';
            }
            else if (header.indexOf("GET /RotKirStop") >= 0) {
              StatusGerakRotasiKiri = 'P';
            }
            else if (header.indexOf("GET /RotateKanan") >= 0) {
              StatusGerakRotasiKanan = 'E';
            }
            else if (header.indexOf("GET /RotKanStop") >= 0) {
              StatusGerakRotasiKanan = 'P';
            }
            else if (header.indexOf("GET /ESPMode1") >= 0) {
              ESPMode = 1;
            }
            else if (header.indexOf("GET /ESPMode2") >= 0) {
              ESPMode = 2;
            }
                    
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
        
            // CSS to style the on/off buttons 
            client.println("<style>html { font-family: Monaco; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; border-radius: 6px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #FF0000;}");//#FF0000
            client.println(".Yellow {background-color: #FF8C00;}");//#FF0000
            client.println(".buttonMaju{margin-top: 10px;}");
            client.println(".buttonKanan{margin-left: 50px; margin-top: 50px;}");
            client.println(".buttonKiri{margin-right: 50px; margin-top: 50px;}");
            client.println(".buttonMundur{margin-top: 10px;}");
            client.println(".class1{position: relative; display: flex; justify-content: center; align-items: center;}");
            client.println(".class2{display: flex; flex-direction: column;}");
            client.println(".flex-row{position: relative; display: flex; flex-direction: row; justify-content: center;}");
            client.println(".buttonadt{padding: 5px 50px;}");
            client.println(".buttonRotate{position: relative; border: none; color: white; padding: 20px 40px; top: 60px;}");
//            client.println(".TampilParameter{position: relative; bottom: 800px; left: 200px;}"); ngebuggg
            client.println(".ModeButton{margin-top: 15px; padding: 15px 30px;}");
            client.println(".blackcolor{background-color: #000000; color: white; border: 0;}"); 
            client.println(".head{font-size: 16px; background-color: blue;}");
            client.println(".korban{background: #FF0000;border-radius: 20px; font-weight: bold;}");





            
            client.println(".whitecolor{background-color: #FFFFFF; color: black; border: 1;}</style></head>");
            
            // Web Page Heading
            
            client.println("<body><h2>ESP32 Control</h2>");
            client.println("<div class=\"class1\">");
                // Tombol kiri
                client.println("<div class=\"buttonKiri class2\">");
                      client.println("<p>Gerak - State " + String(StatusGerakRotasiKiri) + "</p>");    
                      if (StatusGerakKiri=='A') {
                        client.println("<p><a href=\"/KiriStop\"><button class=\"button button2\">Berhenti</button></a></p>");
                      }
                       
                      else if ( StatusGerakKiri=='P'){
                        client.println("<p><a href=\"/KiriKiri\"><button class=\"button\">KIRI</button></a></p>");
                      }
                      //else if (StatusMajuPelan == '')

                      if(StatusGerakRotasiKiri=='P') {// 
                        client.println("<p><a href=\"/RotateKiri\"><button class=\"buttonRotate Yellow\">Rotate Kiri</button></a></p>");
                      }

                      else if (StatusGerakRotasiKiri=='Q') {
                        client.println("<p><a href=\"/RotKirStop\"><button class=\"buttonRotate button2\">Berhenti</button></a></p>");//buttonRotate
                      }
                      
                client.println("</div>");

                //Tombol Maju-Mundur
                client.println("<div>");
                      client.println("<div class=\"buttonMaju class2\">");//tombol maju
                        if (StatusTaruhKorban=='T') {//jika maju
                          client.println("<p><a href=\"/TaruhStop\"><button class=\"button korban\">Berhenti</button></a></p>");           
                        } else {
                          client.println("<p><a href=\"/TaruhTaruh\"><button class=\"button korban\">TARUH</button></a></p>");
                        }    
                        if (StatusGerakDepan=='W') {//jika maju
                          client.println("<p><a href=\"/MajuStop\"><button class=\"button button2\">Berhenti</button></a></p>");           
                        } else {
                          client.println("<p><a href=\"/MajuMaju\"><button class=\"button\">MAJU</button></a></p>");
                        } 
                      client.println("</div>");
                      client.println("<div class=\"buttonMundur class2\">");//tombol mundur
                        client.println("<p>Gerak - State " + String(StatusGerakBelakang) + "</p>");
                        if (StatusSemprot == 'G') {
                          client.println("<p><a href=\"/SemprotStop\"><button class=\"button korban\">Berhenti</button></a></p>");
                        } else {
                          client.println("<p><a href=\"/SemprotSemprot\"><button class=\"button korban\">SEMPROT</button></a></p>");
                        }
                        if (StatusGerakBelakang=='S') {
                          client.println("<p><a href=\"/MundurStop\"><button class=\"button button2\">Berhenti</button></a></p>");
                        } else {
                          client.println("<p><a href=\"/MundurMundur\"><button class=\"button\">MUNDUR</button></a></p>");
                        }
                        if (StatusGerakChilling == 'R') {
                          client.println("<p><a href=\"/ChillingStop\"><button class=\"button button2\">Berhenti</button></a></p>");
                        } else {
                          client.println("<p><a href=\"/ChillingChilling\"><button class=\"button korban\">TurunLengan</button></a></p>");
                        }
                        
                      client.println("</div>");
                client.println("</div>");//div maju-mundur

                client.println("<div class=\"buttonKanan class2\">");
                        client.println("<p>Gerak - State " + String(StatusGerakKanan) + "</p>");
                        if (StatusGerakKanan=='D') {
                          client.println("<p><a href=\"/KananStop\"><button class=\"button button2\">Berhenti</button></a></p>");
                        } else {
                          client.println("<p><a href=\"/KananKanan\"><button class=\"button\">KANAN</button></a></p>");
                        }
                        
                        if(StatusGerakRotasiKanan=='P') {// 
                          client.println("<p><a href=\"/RotateKanan\"><button class=\"buttonRotate Yellow\">Rotate Kanan</button></a></p>");
                        }
                        else if (StatusGerakRotasiKanan=='E') {
                         client.println("<p><a href=\"/RotKanStop\"><button class=\"buttonRotate button2\">Berhenti</button></a></p>");//buttonRotate
                        }
                client.println("</div>");
            client.println("</div>");//div akhir
            client.println("<br /><br />");
            
            client.println("<div class=\"class2\">");
              client.println("<div class=\"flex-row\">");
                client.println("<p><a href=\"/Kecepatanplus\"><button class=\"buttonadt\">V++</button></a></p>");
                client.println("<p><a href=\"/Kecepatanmin\"><button class=\"buttonadt\">V--</button></a></p>");              
              client.println("</div>");
              
              client.println("<div class=\"flex-row\">");
                client.println("<p><a href=\"/Langkahplus\"><button class=\"buttonadt\">W++</button></a></p>");
                client.println("<p><a href=\"/Langkahmin\"><button class=\"buttonadt\">W--</button></a></p>");              
                client.println("</div>");

              client.println("<div class=\"flex-row\">");
                client.println("<p><a href=\"/Tinggiplus\"><button class=\"buttonadt\">Z++</button></a></p>");
                client.println("<p><a href=\"/Tinggimin\"><button class=\"buttonadt\">Z--</button></a></p>");              
              client.println("</div>");

              client.println("<div>");
              if (ESPMode==1) {
                client.println("<p><a href=\"/ESPMode2\"><button class=\"ModeButton blackcolor\">MODE 1</button></a></p>");
              } else {
                client.println("<p><a href=\"/ESPMode1\"><button class=\"ModeButton whitecolor\">MODE 2</button></a></p>");   
              }      
              client.println("</div>");

              client.println("<div class=\"TampilParameter\">");
                client.println("<p>V :" + String(V) + "</p>");
                client.println("<p>W :" + String(W) + "</p>");
                client.println("<p>Z :" + String(Z) + "</p>");   
              client.println("</div>");
              
            client.println("</div>");
            
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
//    Serial.println("Client disconnected.");
//    Serial.println("");
  }
  
  if(StatusGerakDepan=='W'){//    Serial.println("Maju");
    CommandData[0]=12;         
    
  } else if(StatusGerakDepan=='P'){//    Serial.println("StopMaju");
    CommandData[0]=0;
  }
  
  if(StatusGerakBelakang=='S'){//    Serial.println("Mundur");
    CommandData[2]=33;
  }
  else if(StatusGerakBelakang=='P'){//    Serial.println("StopMundur");
    CommandData[2]=0;
  }

  if (StatusGerakChilling =='R'){ //gerakchilling
    CommandData[6]=23; 
  }else if (StatusGerakChilling == 'P'){
    CommandData[6]=0;  
  }
  if (StatusTaruhKorban == 'T') {
    CommandData[7]=24;
    
  } else if (StatusTaruhKorban == 'P') {
    CommandData[7]=0;
  }
  
  if(StatusGerakKanan=='D'){//    Serial.println("Kanan");
    CommandData[3]=22;
  }
  else if(StatusGerakKanan=='P'){//    Serial.println("StopKanan");
    CommandData[3]=0;
  }
  
  if(StatusGerakKiri=='A'){//    Serial.println("Kiri");
    CommandData[1]=44;
  }
  else if(StatusGerakKiri=='P'){//    Serial.println("StopKiri");
    CommandData[1]=0;
  }
  
  if(StatusGerakRotasiKiri=='Q'){//    Serial.println("RotasiKiri");
    CommandData[4]=85;
  }
  else if(StatusGerakRotasiKiri=='P'){//    Serial.println("StoprotasiKiri");
    CommandData[4]=0;
  }

  if(StatusGerakRotasiKanan=='E'){//    Serial.println("RotasiKanan");
    CommandData[5]=99;
  }
  else if(StatusGerakRotasiKanan=='P'){//    Serial.println("Stoprotasikanan");
    CommandData[5]=0;
  }

  if(StatusTambahan == 'Z'){
    CommandData2=111;
    StatusTambahan = 'M';
  }
  else if(StatusTambahan == 'X'){
    CommandData2=222;
    StatusTambahan = 'M';
  }
  else if(StatusTambahan == 'C'){
    CommandData2=333;
    StatusTambahan = 'M';
  }
  else if(StatusTambahan == 'V'){
    CommandData2=444;
    StatusTambahan = 'M';
  }
  else if(StatusTambahan == 'B'){
    CommandData2=555;
    StatusTambahan = 'M';
  }
  else if(StatusTambahan == 'N'){
    CommandData2=666;
    StatusTambahan = 'M';
  }
  else if(StatusTambahan == 'M'){
    CommandData2=0;
  }
 if(StatusSemprot == 'G'){
    CommandData3=878;
  }
  else if(StatusSemprot == 'P'){
    CommandData3=787;
  }
//   if(ESPMode == 2){
//    CommandData3=878;
//  }
//  else if(ESPMode==1){
//    CommandData3=787;
//  }
  
  CommandTotal1=CommandData[0]+CommandData[1]+CommandData[2]+CommandData[3]+ CommandData[4] + CommandData[5] + CommandData[6] + CommandData[7];
//  Serial.println(CommandTotal1);
//    if(CommandTotal1 == 12)Serial.println("Maju");
//    else if(CommandTotal1 == 44)Serial.println("Kiri");
//    else if(CommandTotal1 == 22)Serial.println("Kanan");
//    else if(CommandTotal1 == 33)Serial.println("Mundur");
//    else if(CommandTotal1 == 56)Serial.println("Maju Serong Kiri");
//    else if(CommandTotal1 == 34)Serial.println("Maju Serong Kanan");
//    else if(CommandTotal1 == 77)Serial.println("Mundur Serong Kiri");
//    else if(CommandTotal1 == 55)Serial.println("Mundur Serong Kanan");
//    else if(CommandTotal1 == 85)Serial.println("Rotasi Kiri");
//    else if(CommandTotal1 == 99)Serial.println("Rotasi Kanan");
////    87 = rotasi kiri
////    99 = rotasi kanan
//    else Serial.println("Berhenti");
//    if(CommandData2 == 111)Serial.print("Tambah Kecepatan");
//    else if(CommandData2 == 222)Serial.print("Kurangi Kecepatan");
//    else if(CommandData2 == 333)Serial.print("Tambah Langkah");
//    else if(CommandData2 == 444)Serial.print("Kurangi Langkah");
//    else if(CommandData2 == 555)Serial.print("Tambah Ketinggian");
//    else if(CommandData2 == 666)Serial.print("Tambah Kecepatan");
//
//    if(CommandData3==878)Serial.print("MODE 2");
//    else if(CommandData3==787)Serial.print("MODE 1");
//  Serial.print("Cmd\t"); Serial.print(CommandData[0]); Serial.print("\t"); Serial.print(CommandData[1]); Serial.print("\t"); Serial.print(CommandData[2]); Serial.print("\t"); Serial.println(CommandData[3]);
  
//  memcpy((buff_kirim+3), &CommandData[0], sizeof(CommandData[0]));
//  memcpy((buff_kirim+5), &CommandData[1], sizeof(CommandData[1]));
//  memcpy((buff_kirim+7), &CommandData[2], sizeof(CommandData[2]));
//  memcpy((buff_kirim+9), &CommandData[2], sizeof(CommandData[3]));

  memcpy((buff_kirim+3), &CommandTotal1, sizeof(CommandTotal1));
  memcpy((buff_kirim+5), &CommandTotal1, sizeof(CommandTotal1));
  memcpy((buff_kirim+7), &CommandData2, sizeof(CommandData2));
  memcpy((buff_kirim+9), &CommandData2, sizeof(CommandData2));
  memcpy((buff_kirim+11), &CommandData3, sizeof(CommandData3));
  memcpy((buff_kirim+13), &CommandData3, sizeof(CommandData3));

//  if(kirimIPServer==0){
//    Serial.write(WiFi.localIP());
//    Serial.write('S');
//    kirimIPServer=1;
//  }
//  else if(kirimIPServer==1){


//PROGRAM KIRIM
    for(int i = 0;i<14;i++){
    Serial.write(buff_kirim[i]);
    }//end for
    
//  }//end elif
}
