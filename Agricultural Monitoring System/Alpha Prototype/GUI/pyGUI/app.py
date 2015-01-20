import config
Config = config.config()
username = Config.username
password = Config.password
device_id = Config.device_id

import requests
r = requests.get('https://login.etherios.com/ws/v1/streams/inventory', auth=(username, password))
print (r.text)


# import httplib
# import base64 
# # create HTTP basic authentication string, this consists of 
# # "username:password" base64 encoded  
# auth = base64.encodestring("%s:%s" % (username, password))[:-1]
# # message to send to server
# message = """<sci_request version="1.0"> 
# <send_message> 
#   <targets> 
#     <device id="%s"/>
#   </targets> 
#   <rci_request version="1.1">
#       <query_state/>
#   </rci_request>
# </send_message>
# </sci_request>
# """%(device_id)
# webservice = httplib.HTTP("login.etherios.com ", 80)
# # to what URL to send the request with a given HTTP method
# webservice.putrequest("POST", "/ws/sci")
 
# # add the authorization string into the HTTP header
# webservice.putheader("Authorization", "Basic %s" % auth)
# webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
# webservice.putheader("Content-length", "%d" % len(message))
# webservice.endheaders()
# webservice.send(message)
# # get the response
# statuscode, statusmessage, header = webservice.getreply()
# response_body = webservice.getfile().read()
# # print the output to standard out
# print (statuscode, statusmessage)
# print response_body
