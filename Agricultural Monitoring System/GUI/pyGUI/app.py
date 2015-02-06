#Configuration file
from config import config
cf = config()
username = cf.username
password = cf.password
device_id = cf.device_id
baseURL = cf.baseURL

#Defined commands
from DeviceCommands import commands
cm = commands()

#Http requests
import requests
#r = requests.get('https://login.etherios.com/ws/v1/streams/inventory', auth=(username, password))

def listDataStreams():
	return requests.get(baseURL + '/ws/v1/streams/inventory', auth=(username, password))

def getDeviceStatus():
	return requests.post(baseURL + '/ws/sci', auth=(username, password), data=cm.queryState(device_id))

def getDeviceSettings():
	return requests.post(baseURL + '/ws/sci', auth=(username, password), data=cm.querySetting(device_id))
