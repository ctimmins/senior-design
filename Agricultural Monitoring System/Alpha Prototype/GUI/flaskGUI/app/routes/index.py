from flask import render_template
from flask import jsonify
from flask import request
from firebase.firebase import FirebaseApplication, FirebaseAuthentication
from app import app
from app.digi import Digi
from app.userConfig import config 

myConfig = config()
myDigi = Digi(myConfig.username, myConfig.password)
auth = FirebaseAuthentication(myConfig.firebaseSecret,
							  myConfig.firebaseEmail,
							  True,
							  True)
firebase = FirebaseApplication(myConfig.firebaseURL, auth)

@app.route('/')
def root():
    res = myDigi.listAllStreams().json()
    return render_template('index.html',
    						user={'name':'Chad'},
    						list=res["list"])

@app.route('/jam')
def jam():
	node = request.args.get('node', 'no_node')
	sensor = request.args.get('sensor', 'no_sensor')
	name = request.args.get('name', 'no_name')
	age = request.args.get('age', '0')
	#res = firebase.get('/Chad', None)
	res = firebase.post('/Chad/'+node+'/'+sensor, {"Sensor": sensor, "name": name})
	return "post success: %s" % node
