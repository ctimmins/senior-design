from flask import render_template
from flask import jsonify
from flask import request
from firebase.firebase import FirebaseApplication, FirebaseAuthentication
from app import app
from app.digi import Digi
from app.userConfig import config 
from datetime import datetime

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

@app.route('/update')
def update():
	
	current_time = str(datetime.now().time())

	data = {
		"Temperature":  request.args.get('temp', 'no_temp'),
		"VWC":          request.args.get('vwc', 'no_vwc'),
		"Time":         current_time
	}

	loc = request.args.get('loc', 'no_location')
	sensor = request.args.get('lev', 'no_sensor_depth')
	url = '/Chad/%s/%s' % (loc, sensor) 

	res = firebase.post(url, data)
	return "post success: %s" % current_time
