let AWS = require('aws-sdk');
// Fetch AWS Endpoint from environment variables
const ENDPOINT = process.env.AWS_IOT_ENDPOINT

// Initialize the iot-data SDK class
let iotData = new AWS.IotData({
    endpoint: ENDPOINT
})

// This basically republishes the received payload on a topic to another topic
exports.handler = async (event) => {
    console.log('Received event: -', JSON.stringify(event));
   let params = {
       topic : 'sample/device/out/republished-data',
       payload: JSON.stringify(event),
       qos: 1
   }
   
   return iotData.publish(params, (err, data) => {
       if(err){
           console.error(err);
       }else{
           console.log('Successfully republished payload.');
       }
   }).promise()
};
