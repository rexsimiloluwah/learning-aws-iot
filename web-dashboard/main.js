var AWS = require('aws-sdk')
var AWSIoTData = require('aws-iot-device-sdk');

console.log(window)

const thingName = 'sample-device'

/* Query Selectors */
let [tempCardBody,humidityCardBody]= document.querySelectorAll(".card .body");
let progressCircle = document.querySelector(".svg-circle__inner");
let lightIntensityValue = document.querySelector(".progress-circle__text");
let toggleSwitch = document.querySelector(".switch input");
let lampState = document.querySelector(".lamp1-state");

// Initialize AWS Configuration
AWS.config.region = AWSConfiguration.region;
AWS.config.credentials = new AWS.CognitoIdentityCredentials({
    IdentityPoolId: AWSConfiguration.poolId
})

// Authentication using cognito identity pools
let cognitoIdentity = new AWS.CognitoIdentity();
let cognitoIdentityData;
AWS.config.credentials.get((err,data) => {
    if(!err){
        console.log(`Retrieved identity from Cognito: ${AWS.config.credentials.identityId}`);
        let cognitoParams = {
            IdentityId: AWS.config.credentials.identityId
        }
        cognitoIdentity.getCredentialsForIdentity(cognitoParams, (err,data) => {
            if(!err){
                let cognitoConfig = {
                    region: AWS.config.region,  
                    host: AWSConfiguration.endpoint,
                    clientId: thingName + '-' + (Math.floor((Math.random() * 100000) + 1)),
                    protocol: 'wss',
                    maximumReconnectTimeMs: 8000,
                    // Set Access Key, Secret Key and session token based on credentials from Cognito
                    accessKeyId: data.Credentials.AccessKeyId,
                    secretKey: data.Credentials.SecretKey,
                    sessionToken: data.Credentials.SessionToken
                }
                // Create the AWS IoT shadows and device object.
                const device = AWSIoTData.device(cognitoConfig);
                const thingsShadow = AWSIoTData.thingShadow(cognitoConfig);

                device.on('connect', () => {
                    console.log('Device connected successfully.');
                    device.subscribe('sample/device/out/data');
                    device.publish(
                        'sample/device/test/data',
                        JSON.stringify({'test':'working'})
                    )
                })

                device.on('message', (topic, payload) => {
                    console.log(`Received message on topic: ${topic}`);
                    console.log(`Payload: ${payload.toString()}`);
                })
                
            }
        })
    }
})

/* Constants */
const PROGRESS_CIRCLE_CIRCUMFERENCE= 440;

toggleSwitch.addEventListener('change', () => {
    lampState.innerText = toggleSwitch.checked ? "OFF" : "ON"
})

const updateSensorValues = () => {
    const sensorValues = {
        'temp': parseFloat((Math.random()*50).toFixed(2)),
        'humidity': parseInt(Math.random()*100),
        'light_intensity': parseInt(Math.random()*100)
    }

    console.log(toggleSwitch.checked);
    tempCardBody.getElementsByClassName("value")[0].innerHTML = `<span>${sensorValues['temp']} <sup>&#x2103;</sup></span>`
    humidityCardBody.getElementsByClassName("value")[0].innerHTML = `<span>${sensorValues['humidity']} <sup>%</sup></span>`
    progressCircle.style.strokeDashoffset = parseInt(((100-sensorValues['light_intensity'])/100)*PROGRESS_CIRCLE_CIRCUMFERENCE).toString();
    lightIntensityValue.innerHTML = `<span>${sensorValues['light_intensity']}%</span>`
}


// setInterval(updateSensorValues, 60000);