# The following lines require manual changes 
username = "YourUsername" # enter your username
password = "YourPassword" # enter your password
device_id = "Target Device Id" # enter device id of target 
# Nothing below this line should need to be changed
# ------------------------------------------------- 
import httplib
import base64 
# create HTTP basic authentication string, this consists of 
# "username:password" base64 encoded  
auth = base64.encodestring("%s:%s" % (username, password))[:-1]
# message to send to server
message = """<sci_request version="1.0"> 
<send_message> 
  <targets> 
    <device id="%s"/>
  </targets> 
  <rci_request version="1.1">
      <query_state/>
  </rci_request>
</send_message>
</sci_request>
"""%(device_id)
webservice = httplib.HTTP("login.etherios.com ", 80)
# to what URL to send the request with a given HTTP method
webservice.putrequest("POST", "/ws/sci")
 
# add the authorization string into the HTTP header
webservice.putheader("Authorization", "Basic %s" % auth)
webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
webservice.putheader("Content-length", "%d" % len(message))
webservice.endheaders()
webservice.send(message)
# get the response
statuscode, statusmessage, header = webservice.getreply()
response_body = webservice.getfile().read()
# print the output to standard out
print (statuscode, statusmessage)
print response_body