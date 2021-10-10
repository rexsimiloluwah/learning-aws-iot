const AWS = require('aws-sdk');
AWS.config.update({
    'region': 'us-west-2'
})

// Lambda function deployed via API Gateway

const dynamodb = new AWS.DynamoDB.DocumentClient();
const sensorTableName = 'esp8266SensorData';

exports.handler = async(event) => {
    let response;
    console.log('Received Event: ', event);
    let page = event.queryStringParameters.page;
    let limit = event.queryStringParameters.limit
    switch(true){
        case event.httpMethod == 'GET' && event.path==='/data':
            response = await getAllSensorReadings(page,limit);
            console.log(response);
            break;
    }
    
    return response
};

/**
 * Description: Build AWS Lambda response 
 * @param {object} body 
 * @param {number} statusCode 
*/
const buildResponse = (statusCode, body) => {
    return {
        statusCode: statusCode,
        body: JSON.stringify(body),
        headers: {
            'Access-Control-Allow-Origin' : '*',
            'Access-Control-Allow-Headers':'Content-Type,X-Amz-Date,Authorization,X-Api-Key,X-Amz-Security-Token',
            'Access-Control-Allow-Credentials' : true,
            'Content-Type': 'application/json'
        }
    }
}

/**
 * 
 * @param {Number} page 
 * @param {Number} limit 
 * @returns 
 */
const getAllSensorReadings = async (page =1, limit=40) => {
    let pageNumber = parseInt(page)
    let readingsPerPage = parseInt(limit)
    const params = {
        TableName: sensorTableName,
    }
    console.log(params)
    let allSensorReadings=[];
    let items;
    try{
        do{
            items = await dynamodb.scan(params).promise();
            console.log(items);
            if(items.Count > 0){
                items.Items.forEach((item) => allSensorReadings.push(item));
                params.ExclusiveStartKey = items.LastEvaluatedKey;
            }
            
        } while(typeof items.LastEvaluatedKey !== 'undefined')
        
        if(allSensorReadings.length){
            const total = allSensorReadings.length;
            const numPages = Math.ceil(total/readingsPerPage);
            const start = (pageNumber-1)*readingsPerPage;
            const stop = start + readingsPerPage;
            allSensorReadings.reverse();
            const paginatedData = allSensorReadings.slice(start,stop)
            
            return buildResponse(200, {
                status: true,
                message: `Successfully fetched ${paginatedData.length} records.`,
                page: pageNumber,
                totalPages: numPages,
                data: paginatedData
            })
        }else{
            return buildResponse(404, {
                status: false,
                message: `No sensor readings found.`,
                data: allSensorReadings
            })
        }
        
    }
    
    catch(error){
        console.error(`An error occurred: - ${error}`);
        return buildResponse(404, {
            status: false,
            message: error
        })
    }
}
