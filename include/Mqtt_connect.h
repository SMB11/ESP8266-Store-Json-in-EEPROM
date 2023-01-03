WiFiClient espClient;
PubSubClient client(espClient);

// #include <task.h>
void setup_wifi()
{
    delay(10);
    Serial.print("\nConnecting to ");
    Serial.println(ssid);

    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid,password);

    // WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("\nWiFi connected\nIP address: ");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    lastMQTTReconnectAttempt = 0;
}

bool reconnect()
{

    Serial.print("Attempting MQTT connection...");
    String clientId = "MainClimate";

    if (client.connect(clientId.c_str()))
    {
        const char *type = "mqtt";
        const char *status = "reconnected";
        Serial.println(status);
        // pubslishError(type, status);

        client.subscribe(tasksUpdateTopic); // subscribe the topics here
        client.subscribe(mechanismsUpdateTopic); // subscribe the topics here
        client.subscribe(tasksCheckTopic); // subscribe the topics here
        client.subscribe(mechanismsCheckTopic); // subscribe the topics here

        // client.subscribe(getVariablesTopic);
        Serial.print("Subscribed to ");
        Serial.println(tasksUpdateTopic);

        // client.subscribe(setTime_topic); // subscribe the topics here
    }

    return client.connected();
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String incommingMessage = "";
    for (int i = 0; i < length; i++)
        incommingMessage += (char)payload[i];

    Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);

    //--- check the incomming message
    if (strcmp(topic, tasksUpdateTopic) == 0)
    {
        // Serial.println("Message received watering_tasks ");
        parseTasks(payload);
        client.publish("mainClimateTasks_received", "1");
    }
     if (strcmp(topic, mechanismsUpdateTopic) == 0)
    {
        // Serial.println("Message received watering_tasks ");
        parseMechanisms(payload);
        client.publish("mainClimateTasks_received", "1");
    }
      if (strcmp(topic, mechanismsCheckTopic) == 0)
    {
        getMechanisms();
      Serial.println(m.humidityStartControlValue);
  Serial.println(m.winterMode);
  Serial.println(m.humiPip);
  Serial.println(m.wPid);
  Serial.println(m.wPii);
  Serial.println(m.wPip);
    }
     if (strcmp(topic, tasksCheckTopic) == 0)
    {
        getTasks();
         for (int i = 0; i < (int)(sizeof(tasks) / sizeof(tasks[0])); i++)
        {
            Serial.println(tasks[i].task_number);
            Serial.println(tasks[i].start_time);
            Serial.println(tasks[i].end_time);
            Serial.println(tasks[i].start_temp);
            Serial.println(tasks[i].end_temp);
            Serial.println(tasks[i].start_humidity);
            Serial.println(tasks[i].end_humidity);
            Serial.println(tasks[i].ventilation_temp);
        }

  }


}


void mqtt_loop()
{
    // client.loop();
    // if (!client.connected())

    //     reconnect(); // TODO: change reconnect function
    if (!client.connected())
    {
        long now = millis();
        if (now - lastMQTTReconnectAttempt > mqttUpdateInterval)
        {
            lastMQTTReconnectAttempt = now;
            // Attempt to reconnect
            if (reconnect())
            {
                lastMQTTReconnectAttempt = 0;
            }
        }
    }
    else
    {
        // Client connected

        client.loop();
    }
}

void mqtt_init()
{
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    client.setBufferSize(1000);
}
