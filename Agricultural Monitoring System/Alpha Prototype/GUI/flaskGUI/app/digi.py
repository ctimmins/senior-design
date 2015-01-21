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
	
	def setXbeeSettings(self, addr, cmd, val):
		return requests.post(self.baseURL + '/ws/sci', auth=(self.user, self.pw), data = self.setXbeeCmd(addr, cmd, val))

	def getXbeeSettings(self, addr, cmd):
		return requests.post(self.baseURL + '/ws/sci', auth=(self.user, self.pw), data = self.setXbeeCmd(addr, cmd))


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

	def setXbeeCmd(self, addr, cmd, val):
		return """\
			<sci_request version="1.0">
			  <data_service>
			    <targets>
			      <device id="%s"/>
			    </targets>
			    <requests>
			      <device_request target_name="xig">
			        &lt;at hw_address="%s" command="%s" value="%s" /&gt;
			      </device_request>
			    </requests>
			  </data_service>
			</sci_request>
			""" % (device_id, addr, cmd, val)

	def getXbeeCmd(self, addr, cmd):
		return """\
			<sci_request version="1.0">
			  <data_service>
			    <targets>
			      <device id="%s"/>
			    </targets>
			    <requests>
			      <device_request target_name="xig">
			        &lt;at hw_address="%s" command="%s"/&gt;
			      </device_request>
			    </requests>
			  </data_service>
			</sci_request>
			""" % (device_id, addr, cmd, val)


			