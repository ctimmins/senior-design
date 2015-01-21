import requests

class Digi:
	def __init__(self, username, password):
		self.user = username
		self.pw = password
		#self.device_id = "00000000-00000000-00409DFF-FF78D746"
		self.baseURL = "https://login.etherios.com"

	def listDataStreams(self):
		return requests.get(self.baseURL + '/ws/v1/streams/inventory', auth=(self.user, self.pw))

	def getDeviceStatus(self, device_id):
		return requests.post(self.baseURL + '/ws/sci', auth=(self.user, self.pw), data=self.queryStateCmd(device_id))

	def getDeviceSettings(self, device_id):
		return requests.post(self.baseURL + '/ws/sci', auth=(self.user, self.pw), data=self.querySettingCmd(device_id))
	

	##################	
	## RCI commands ##
	##################
	def queryStateCmd(self, device_id):
		return """\
			<sci_request version="1.0"> 
				<send_message> 
				  <targets> 
				    <device id="%s"/>
				  </targets> 
				  <rci_request version="1.1">
				      <query_state/>
				  </rci_request>
				</send_message>
			</sci_request>
			""" % (device_id)

	def querySettingCmd(self, device_id):
		return """\
			<sci_request version="1.0"> 
				<send_message> 
				  <targets> 
				    <device id="%s"/>
				  </targets> 
				  <rci_request version="1.1">
				      <query_setting/>
				  </rci_request>
				</send_message>
			</sci_request>
			""" % (device_id)



		
			
			

	 

