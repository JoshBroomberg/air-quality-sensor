#!/usr/bin/env python3
import serial
import json
import boto3

session = boto3.Session(profile_name='firehose')
temperatureClient = session.client('firehose', region_name="us-west-2")
        
raw_data_buffer = []
parsed_record_buffer = []

def process_data(data):
    global raw_data_buffer
    
    if data == "data start":
        raw_data_buffer = []
        return

    if data == "data end":
        handle_block()
        return
    
    raw_data_buffer.append(data)

def parse_record_from_raw():
    record_dict= {}
    for entry in raw_data_buffer:
        key_val = entry.split(": ")
        record_dict[key_val[0]] = float(key_val[1])
    
    return record_dict

def handle_block():
    global parsed_record_buffer

    record_dict = parse_record_from_raw()
    print(record_dict)

    record = {
        'Data': json.dumps(record_dict) + "\n"
    }

    parsed_record_buffer.append(record)
    
    if len(parsed_record_buffer) > 15:
        response = temperatureClient.put_record_batch(
            DeliveryStreamName='AirQuality-Source',
            Records=parsed_record_buffer
        )
        print(response)
        parsed_record_buffer = []

# Reading
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
ser.flush()
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        process_data(line)