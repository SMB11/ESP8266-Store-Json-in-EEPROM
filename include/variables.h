struct Task
{
    int task_number;
    int start_time;
    int end_time;
    float start_temp;
    float end_temp;
    int start_humidity;
    int end_humidity;
    int ventilation_temp;
};
Task tasks[4];


struct Mechanisms{
    bool winterMode;
    float wPip;
    float wPii;
    float wPid;
    float humiPip;
    float humidityStartControlValue;
};
Mechanisms m;

// Task newTasks[4];
Task current_task;

  const char *ssid = "TechZone";
 const char *password = "Aaaa1111";

 const char *mqtt_server = "192.168.0.103"; //  your broker url
// const char *mqtt_server = "192.168.8.100"; //  your broker url
// const char *mqtt_username = "NakiLLC";
// const char *mqtt_password = "Narek2015";
const int mqtt_port = 1883;


const char *tasksUpdateTopic = "MainClimate_tasksUpdate";
const char *mechanismsUpdateTopic = "MainClimate_mechanismsUpdate";
const char *mechanismsCheckTopic = "check_M";
const char *tasksCheckTopic = "check_T";
unsigned long lastMQTTReconnectAttempt;
int mqttUpdateInterval= 5000;