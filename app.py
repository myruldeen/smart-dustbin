import paho.mqtt.client as mqtt
from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret'
socketio = SocketIO(app)

# The callback for whem the client recieves a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code"+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and 
    # reconnect then sunscriptions will be renewed
    client.subscribe("/esp8266/ultra")

	# The callback for when a PUBLISH message is recieved from the ESP8266.
def on_message(client, userdata, message):

    print("GARBAGE UPDATE'" + str(message.payload) +"%' Filled'")
    
    if message.topic == "/esp8266/ultra":
        print("SMART DUSTBIN DEMO")
        
mqttc=mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect("localhost",1883,60)
mqttc.loop_start()

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=8181, debug=True)
