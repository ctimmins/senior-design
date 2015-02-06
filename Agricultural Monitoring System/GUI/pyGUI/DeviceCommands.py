class commands:
	
	def queryState(self, device_id):
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

	def querySetting(self, device_id):
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



	
		
		

