

// Online C++ Compiler - Build, Compile and Run your C++ programs online in your favorite browser

#include<iostream>
#include<string>
#include <mosquitto.h>

#define MQTT_HOST "local host"
#define MQTT_PORT 1883
#define MQTT_TOPIC "airport security"

using namespace std;

struct SensorData {
    string sensorId;
    float value;
};

class MQTTClient {
    private:
        mosquitto* mosq;
    public:
          MQTTClient() {
              mosquitto_lib_init();
              mosq = mosquitto_new(nullptr, true, nullptr);
              if(!mosq){
                  cerr << "Failed to initialize MQTT client." << endl;
               exit(1);
                }
                
                mosquitto_message_callback_set(mosq, &MQTTClient::messageCallback);
                mosquitto_user_data_set(mosq, this);
              
}


    ~MQTTClient() {
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }
    
    void connect(const string& clientId) {
        if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 0) != MOSQ_ERR_SUCCESS) {
            cerr << "Failed to connect to the MQTT broker." << endl;
            exit(1);
        }
        
         if (mosquitto_subscribe(mosq, nullptr, MQTT_TOPIC, 0) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to subscribe to the MQTT topic." << std::endl;
            exit(1);
         }
         
         msquitto_loop_start(mosq);
         cout<<"connected to MQTT broker."<<endl;
}
        
 void publish(const string& message){
     mosquitto_publish(mosq, nullptr, MQTT_TOPIC, message.length(), message.c_str(), 0, false);
    

 }
 
    static void messageCallback(mosquitto*, void* userData, const mosquitto_message* message) {
        MQTTClient* client = static_cast<MQTTClient*>(userData);
        string topic = message->topic;
        string payload = static_cast<const char*>(message->payload);

        // Handle incoming messages here
        cout << "Received message - Topic: " << topic << " Payload: " << payload << endl;
    }
};

int main()
{

    MQTTClient mqttClient;
    mqttClient.connect("airport_security_system");
    
     // Simulate sensor data
     vector<SensorData> sensorDataList = {
        {"temperature", 25.5},
        {"humidity", 50.0},
        {"pressure", 1013.25}
    };
    
     // Continuously publish sensor data
     while (true) {
         for (const auto& sensorData : sensorDataList) {
             // Convert sensor data to JSON or other desired format
             string sensorDataJson = "{\"sensorId\":\"" + sensorData.sensorId + "\", \"value\":" + to_string(sensorData.value) + "}";
             
              // Publish sensor data to the MQTT broker
              mqttClient.publish(sensorDataJson);
              
              // Wait for some time between sensor readings
              this_thread::sleep_for(std::chrono::seconds(5));
         }
        
    return 0;
}