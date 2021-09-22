import boto3
import pandas as pd
from matplotlib import pyplot as plt

# Create a client for the IoT Analytics service
client = boto3.client('iotanalytics')

dataset = "sampleiotdevice_dataset"  #Replace with your dataset name
dataset_url = client.get_dataset_content(datasetName = dataset)['entries'][0]['dataURI']

df = pd.read_csv(dataset_url)
print(df.head())
df.timestamp = pd.to_datetime(df.timestamp)
df.set_index('timestamp', inplace=True, drop=True)
df.sort_values('timestamp', inplace=True)
print(df.head())
print(df.dtypes)

fig, ax = plt.subplots()
df.battery_soc.plot(legend=True)
df.battery_soh.plot(legend=True)
plt.grid(True)
plt.legend()
plt.show()

