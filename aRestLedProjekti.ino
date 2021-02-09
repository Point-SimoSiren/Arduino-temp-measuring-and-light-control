#include <SPI.h>
#include <WiFi101.h>
#include <aREST.h>

// wifi status
int status = WL_IDLE_STATUS;

// aRest instanssin luominen
aREST rest = aREST();

// WiFi parametrit
char ssid[] = "WiFin nimi";
char password[] = "WiFin salasana";

// Kuunneltava portti IP osoitteen juureen, eli ei portti numeroa
#define LISTEN_PORT     80

// Palvelimen instanssin luominen
WiFiServer server(LISTEN_PORT); 

// Muuttujien alustaminen
int temperature;
int humidity;

// Setup ajetaan vain kerran alussa
void setup() {
  
  // Asetetaan faud rate serial monitorille
  Serial.begin(115200);

  // Laitteen nimi ja id
  rest.set_id("1");
  rest.set_name("mkr1000");

  // Muuttujien asettaminen ja esittely Rest API:lle
  temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Funktio valon ohjaamiseen
  rest.function("led",ledControl);

  // Yhdistä WiFiin
  while(status != WL_CONNECTED) {
    Serial.print("Yritetään muodostaa SSID yhteys: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);

    // Odota yhteyttä 10 sekuntia
    delay(10000);
  }
  Serial.println("WiFi yhdistetty");

// Käynnistä palvelin
  server.begin();
  Serial.println("Server started");

  // Tulostetaan IP osoite serial monitoriin
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {

  // API kutsujen käsittely
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

}

// Funktio jota ohjataan API:n kautta
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6,state);
  return 1;
}
