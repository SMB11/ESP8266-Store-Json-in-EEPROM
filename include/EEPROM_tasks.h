
void EEPROM_init()
{
    EEPROM.begin(512);
}
int convertToMinutes(const char *time)
{
    int hour1 = time[0] - '0';
    int hour2 = time[1] - '0';
    int min1 = time[3] - '0';
    int min2 = time[4] - '0';
    int timeInMinutes = ((hour1 * 10 + hour2) * 60 + (min1 * 10 + min2));
    return timeInMinutes;
}

void getTasks()
{
    EEPROM_readAnything(0, tasks);
    delay(100);
    //  EEPROM_readAnything(0, newTasks);
    //  if (/* condition */)
    //  {
    //      /* code */
    //  }
}

void parseTasks(byte *payload)
{
    StaticJsonDocument<768> doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    for (JsonObject item : doc.as<JsonArray>())
    {

        int task_number = item["task_number"];       // 1, 2, 3, 4
        const char *start_time = item["start_time"]; // "07:00", "09:00", "16:00", "18:00"
        const char *end_time = item["end_time"];     // "09:00", "16:00", "18:00", "07:00"
        float start_temp = item["start_temp"];       // 19, 21, 21, 19
        float end_temp = item["end_temp"];           // 21, 21, 19, 21
        int start_humidity = item["start_humidity"]; // 80, 80, 80, 80
        int end_humidity = item["end_humidity"];     // 80, 80, 80, 80
        int ventilation_temp = item["ventilationTemp"];

        tasks[task_number - 1].task_number = task_number;
        tasks[task_number - 1].start_time = convertToMinutes(start_time);
        tasks[task_number - 1].end_time = convertToMinutes(end_time);
        tasks[task_number - 1].start_temp = start_temp;
        tasks[task_number - 1].end_temp = end_temp;
        tasks[task_number - 1].start_humidity = start_humidity;
        tasks[task_number - 1].end_humidity = end_humidity;
        tasks[task_number - 1].ventilation_temp = ventilation_temp;
    }
    Serial.println(sizeof(tasks));
    EEPROM_writeAnything(0, tasks);
    EEPROM.commit();
    Serial.println("EEPROM Updated");
}
/////////////////////////////////////////////////////////////////////////////////////

void getMechanisms()
{
    EEPROM_readAnything(129, m);
    delay(100);
}

void parseMechanisms(byte *payload)
{
    StaticJsonDocument<768> doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

     m.winterMode = doc["winterMode"];                              // false
    m.wPip = doc["wPip"];                                         // 1.1
    m.wPii = doc["wPii"];                                           // 11
    m.wPid = doc["wPid"];                                           // 12
    m.humiPip = doc["humiPip"];                                     // 12
    m.humidityStartControlValue = doc["humidityStartControlValue"]; // 80



    Serial.println(sizeof(m));
    EEPROM_writeAnything(129, m);
    EEPROM.commit();
    Serial.println("EEPROM Updated");

}
