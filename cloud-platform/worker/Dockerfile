FROM python:3-alpine

RUN pip3 install paho-mqtt influxdb

WORKDIR /app
COPY app.py .

CMD ["python3","-u","app.py"]