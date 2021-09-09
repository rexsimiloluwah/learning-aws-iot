const path = require('path');
const awsIoTSDK = require('aws-iot-device-sdk');

// Configuration 
const CONFIG_DIR = '../config'
const PORT = 8883
const DEVICE_NAME = 'sample-device'
const HOST = process.env.AWS_IOT_ENDPOINT
const ROOT_CA_PATH = path.join(CONFIG_DIR,'root-CA.crt')
const DEVICE_PRIVATE_KEY_PATH = path.join(CONFIG_DIR,'sample-device','ff1b4a6e6e-private.pem.key')
const DEVICE_CERT_PATH = path.join(CONFIG_DIR,'sample-device','ff1b4a6e6e-certificate.pem.crt')

// Topics 
PUB_TOPIC = 'sample/device/out/data';
SUB_TOPIC = 'sample/device/in/data';

// Initialize the connection
const device = awsIoTSDK.device({
    keyPath: DEVICE_PRIVATE_KEY_PATH,
   certPath: DEVICE_CERT_PATH,
     caPath: ROOT_CA_PATH,
   clientId: DEVICE_NAME,
       host: HOST,
 });

/**
 * @description Callback function when the device is connected
 */
device.on('connect', (err,data)=>{
    console.log(`Successfully established connection to: ${HOST}`);
    // Start the infinite publish loop
    infiniteLoopPublish();
})

/**
 * @description Callback function when a new message is received from a subscribed topic
 */
device.on('message', (topic, message)=>{
    console.log(`[MESSAGE RECEIVED] on topic ${topic}: ${message}`);
})

/**
 * @description Function to generate a random float between two numbers 
 * @param {number} minValue 
 * @param {number} maxValue
 */
const generateRandomFloatNumber = (minValue,maxValue) => {
    return parseFloat(Math.min(minValue + (Math.random() * (maxValue - minValue)),maxValue));
}

/**
 * @description Function to run an infinite loop for publishing data every 20 seconds
 */
const infiniteLoopPublish = (interval=20) => {
    console.log(`Publishing data for ${DEVICE_NAME}`);
    payload = generateFakePayload(DEVICE_NAME);
    console.log(payload);
    device.publish(PUB_TOPIC, JSON.stringify(payload));

    // Recursive call after a certain interval (20 seconds default)
    setTimeout(infiniteLoopPublish, 20*1000);
}

/**
 * @description Generate fake payload 
 * @param {string} deviceName
 */
const generateFakePayload = (deviceName) => {
    const device_states = ['offline','online','inverter_not_detected'];
    let payload = {
        'device_id': deviceName,
        'battery_soc': Math.floor(Math.random() * 100),
        'battery_soh': Math.floor(Math.random() * 100),
        'device_status': device_states[Math.floor(Math.random() * device_states.length) | 0],
        'generated_energy': generateRandomFloatNumber(500, 5000),
        'peak_power_generated': generateRandomFloatNumber(500, 5000),
        'timestamp': new Date().toUTCString().replace(/\..+/, '')
    }

    return payload
}