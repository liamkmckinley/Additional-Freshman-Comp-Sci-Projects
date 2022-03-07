import requests
import json

def lambda_handler(event, context):
    url = "https://api.smrzr.io/summarize?ratio=0.15"

    payload = event['queryStringParameters']['textInput']
    headers = {
      'content-type': 'raw/text',
      'accept': '*/*',
      'sec-fetch-mode': 'cors',
      'sec-fetch-dest': 'empty',
      'accept-language': 'en-GB,en-US;q=0.9,en;q=0.8'
    }

    response = requests.request("POST", url, headers=headers, data = payload)

    print("Response from api {}".format(response.text))
    json_data = json.loads(response.text)
    if 'summary' in json_data.keys():
        print("Successfully fetched response: {}".format(json_data['summary']))
        return {
            'statusCode': 200,
            'body': str({
                'status': '1',
                'summary': str(json_data['summary'])
            }),
            "isBase64Encoded": False
        }
    else:
        print("Some error in fetching summary")
        return {
            "statusCode": 200,
            'body': str({
                'status': '0',
                'error': 'Some error occurred'
            }),
            "isBase64Encoded": False
        }