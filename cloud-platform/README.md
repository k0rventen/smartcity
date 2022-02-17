# cloud platform

`docker-compose` based platform for storing & visualizing data from the smart city model.

## What's needed
You'll need a few things before deploying the project : 

- docker & docker-compose installed
- A MQTTs connector on the acklio LNS side configured

## How to install

Set up the docker-compose file with the required variables. They should be configured according to the mqtts connector from acklio:
```
  worker:
  ...
      - MQTT_ENDPOINT=
      - MQTT_PORT=
      - MQTT_USER=
      - MQTT_PASS=
      - MQTT_TOPIC=
```
 
Then, add your tls cert & key as `tls.crt` and `tls.key`, or create self-signed ones like so :
```
openssl req -x509 -nodes -newkey rsa:2048 -keyout tls.key -out tls.crt -days 365
```

Then, just `docker-compose up -d`. Grafana will be available at __https://host:3000__.
