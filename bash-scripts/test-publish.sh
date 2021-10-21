# Author : Similoluwa Okunowo 
# Description : Just a dummy bash script that publishes pseudo-data to AWS IoT core for demonstration

#!/bin/bash 

REGION='us-west-2'
MQTT_TOPIC='iot/topic/sample'
PUBLISH_INTERVAL=5   # Publish interval in seconds
NUM_ITERATIONS=5  # To configure number of times for publishing

for ((i = 0; i < $NUM_ITERATIONS; i++));
    do
        TIMESTAMP=`date --iso-8601=seconds`
        BATTERY_SOH=$(( 0 + $RANDOM % 100 ))  # Generates a random number between 0 and 100, you know $RANDOM generates a random unsigned integer in the range 0 - (2^15 - 1)
        BATTERY_SOC=$(( 0 + $RANDOM % 100 ))
        PEAK_POWER_GENERATED=$(( 0 + $RANDOM % 5000 ))  # Between 0 and 5000 W
        PUBLISH_COUNT=$(($i+1))

        echo "Publishing message $PUBLISH_COUNT/$NUM_ITERATIONS to topic $MQTT_TOPIC ..."
        aws iot-data publish --topic "$MQTT_TOPIC" --cli-binary-format raw-in-base64-out --payload "{\"device_id\":\"sample-device\", \"battery_soh\":\"$BATTERY_SOH\", \"battery_soc\":\"$BATTERY_SOC\", \"peak_power_generated\":\"$PEAK_POWER_GENERATED\", \"publish_count\":\"$PUBLISH_COUNT\", \"timestamp\":\"$TIMESTAMP\"}" --region "$REGION"
        # Sleep for an interval of 5s 
        sleep $PUBLISH_INTERVAL
    done;
