const AWS = require('aws-sdk')
const AWSIoTData = require('aws-iot-device-sdk');
const thingName = 'esp8266-mcu-device'
// const thingShadowName = 'esp8266-mcu-shadow'

/* Query Selectors */
let [tempCardBody,humidityCardBody]= document.querySelectorAll(".card .body");
let progressCircle = document.querySelector(".svg-circle__inner");
let lightIntensityValue = document.querySelector(".progress-circle__text");
let toggleSwitch = document.querySelector(".switch input");
let lampState = document.querySelector(".lamp1-state");
let lastPublish = document.querySelector(".last-publish");
let loader = document.querySelector("#loader");
let monitorSection = document.querySelector(".monitor");

/* Constants */
const PROGRESS_CIRCLE_CIRCUMFERENCE= 440;

// Initialize AWS Configuration
AWS.config.region = AWSConfiguration.region;
AWS.config.credentials = new AWS.CognitoIdentityCredentials({
    IdentityPoolId: AWSConfiguration.poolId
})

// Fetch sensor data from DynamoDB using the created AWS Lambda API
const fetchSensorDataFromDynamo = async(page=1, limit=1) => {
    const config = {
        headers: {
        },
    }
    const apiEndpoint = `https://weoi0wjusi.execute-api.us-west-2.amazonaws.com/default/data?page=${page.toString()}&limit=${limit.toString()}`;
    const response = await fetch(apiEndpoint, config);
    const {data} = await response.json();
    return data
}

// Authentication using cognito identity pools
let cognitoIdentity = new AWS.CognitoIdentity();
AWS.config.credentials.get((err,data) => {
    if(!err){
        console.log(`Retrieved identity from Cognito: ${AWS.config.credentials.identityId}`);
        let cognitoParams = {
            IdentityId: AWS.config.credentials.identityId
        }
        cognitoIdentity.getCredentialsForIdentity(cognitoParams, (err,data) => {
            if(!err){
                const device = AWSIoTData.device({
                    region: AWS.config.region,  
                    host: AWSConfiguration.endpoint,
                    clientId: thingName + '-' + (Math.floor((Math.random() * 100000) + 1)),
                    protocol: 'wss',
                    // Set Access Key, Secret Key and session token based on credentials from Cognito
                    accessKeyId: data.Credentials.AccessKeyId,
                    secretKey: data.Credentials.SecretKey,
                    sessionToken: data.Credentials.SessionToken
                })
                // Create the AWS IoT shadows and device object.
                // const thingShadows = AWSIoTData.thingShadow({
                //     region: AWS.config.region,  
                //     host: AWSConfiguration.endpoint,
                //     clientId: thingShadowName + '-' + (Math.floor((Math.random() * 100000) + 1)),
                //     protocol: 'wss',
                //     // Set Access Key, Secret Key and session token based on credentials from Cognito
                //     accessKeyId: data.Credentials.AccessKeyId,
                //     secretKey: data.Credentials.SecretKey,
                //     sessionToken: data.Credentials.SessionToken
                // });
                
                // thingShadows.on('connect', () => {
                //     console.log('Thing shadow successfully connected');
                //     thingShadows.register('esp8266-mcu',{},function() {
                //         console.log('Registered device shadow.');
                //       })
                // })

                // thingShadows.on('status', ()=>{
                //     console.log('New status')
                // })

                console.log('Attempting to connect device')
                device.on('connect', () => {
                    console.log('Device connected successfully.');
                    device.subscribe('esp8266-mcu/sensors/data');
                    device.publish("$aws/things/esp8266-mcu/shadow/update", JSON.stringify({"state":{}}))
                })
                
                device.on('message', (topic, payload) => {
                    console.log(`Received message on topic: ${topic}`);
                    console.log(`Payload: ${payload.toString()}`);
                    if(topic == 'esp8266-mcu/sensors/data'){
                        console.log(new Date().toUTCString())
                        const {deviceName,temperature,humidity,ldrVoltage} = JSON.parse(payload);
                        const sensorValues = {
                            device_name: deviceName,
                            temp: temperature,
                            humidity: humidity,
                            light_intensity: Math.floor((ldrVoltage / 5)*100)
                        }

                        updateSensorValues(sensorValues);
                        lastPublish.children[0].innerText = new Date().toUTCString();
                    }
                })

                device.on('offline', () => {
                    console.log('Device is Offline');
                })

                device.on('reconnect', () => {
                    console.log('Device is reconnecting');
                })

                toggleSwitch.addEventListener('change', () => {
                    lampState.innerText = toggleSwitch.checked ? "OFF" : "ON";
                    localStorage.setItem('lampState', toggleSwitch.checked);
                    let stateObject = {
                        state: {
                            desired: {
                                lamp: "OFF"
                            }
                        }
                    }

                    if(toggleSwitch.checked){
                        stateObject.state.desired.lamp="ON"
                    }

                    device.publish(
                        "$aws/things/esp8266-mcu/shadow/update",
                        JSON.stringify(stateObject)
                    )
                    console.log(stateObject);
                    toggleSwitch.checked? console.log("Successfully turned on the LAMP.") : console.log("Successfully turned off the LAMP.");
                })

                
            }
        })
    }
})

// Update Sensor Card values
const updateSensorValues = (sensorValues) => {
    console.log(toggleSwitch.checked);
    tempCardBody.getElementsByClassName("value")[0].innerHTML = `<span>${sensorValues['temp']} <sup>&#x2103;</sup></span>`
    humidityCardBody.getElementsByClassName("value")[0].innerHTML = `<span>${sensorValues['humidity']} <sup>%</sup></span>`
    progressCircle.style.strokeDashoffset = parseInt(((100-sensorValues['light_intensity'])/100)*PROGRESS_CIRCLE_CIRCUMFERENCE).toString();
    lightIntensityValue.innerHTML = `<span>${sensorValues['light_intensity']}%</span>`
}


window.addEventListener('DOMContentLoaded', async() => {
    const data = await fetchSensorDataFromDynamo();
    const {temperature,humidity,ldrVoltage}= data[0]['sensorData'];
    console.log(data[0])
    const lastSensorData = {
        temp: temperature,
        humidity: humidity,
        light_intensity: Math.round(ldrVoltage/5 * 100)
    };

    toggleSwitch.checked = localStorage.getItem('lampState')
    updateSensorValues(lastSensorData);
    loader.style.display = "none"
    monitorSection.style.display = "block"
    lastPublish.children[0].innerText = new Date(parseInt(data[0].timestamp)).toUTCString();
})


