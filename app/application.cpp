#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <webserver.h>
#include <mqtt.h>
#include <Libraries/Adafruit_SSD1306/Adafruit_SSD1306.h>

Timer procMqttInit;
Timer interruptTimer;
int previousState = HIGH;
Adafruit_SSD1306 display(4);
void screenDisplay(String str);

void checkButton() {
	int currentState = digitalRead(FEEDBACK_PIN);
	//debugf("button status %d",currentState);
	if (currentState != previousState && currentState == LOW) {
		debugf("Button pressed");
		digitalWrite(PIN,!digitalRead(PIN));
		debugf("LED pin status: %d", digitalRead(PIN));
		publishMessage();
	}
	previousState = currentState;
}

// Will be called when system initialization was completed
void systemInit()
{

	//screenDisplay("initialising...");
	startWebServer();
	interruptTimer.initializeMs(50,checkButton).start();
	pinMode(PIN,OUTPUT);
	pinMode(FEEDBACK_PIN, INPUT);

}



void connectOk()
{
	debugf("Connect to AP successful");
	//debugf("%s", WifiStation.getSSID().c_str());
	char str[100];
	os_sprintf(str,"Connected to \n%s",WifiStation.getSSID().c_str());
	screenDisplay(str);

	// disable softAP mode
	WifiAccessPoint.enable(false);

	// start MQTT
	procMqttInit.initializeMs(2000, startMqtt).start();
}

void connectFail() {
	char msg[100];
	os_sprintf(msg,"Please connect to \n%s \nWiFi network to \nconfigure.",SOFTAP_SSID);
	screenDisplay(msg);
	WifiStation.waitConnection(connectOk);
}

void init()
{

	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Enable debug output to serial
	Wire.pins(12,14);
	display.begin(SSD1306_SWITCHCAPVCC);
	AppSettings.load();

	WifiStation.enable(true);
	if (AppSettings.exist())
	{
		WifiStation.config(AppSettings.ssid, AppSettings.password);
		if (!AppSettings.dhcp && !AppSettings.ip.isNull())
			WifiStation.setIP(AppSettings.ip, AppSettings.netmask, AppSettings.gateway);
	}
	screenDisplay("Scanning for networks...");

	//screenDisplay("Connection...");
	WifiStation.startScan(networkScanCompleted);

	// Start AP for configuration
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config(SOFTAP_SSID, "", AUTH_OPEN);


	// Run WEB server on system ready
	System.onReady(systemInit);

	WifiStation.waitConnection(connectOk, 15, connectFail);
}


void screenDisplay(String str) {
	display.clearDisplay();
	//display.display();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);

	display.println(str);
	display.display();
	//delay(500);
}

