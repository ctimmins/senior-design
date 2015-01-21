import json
from flask import render_template
from flask import jsonify
from app import app
from app.digi import Digi 

myDigi = Digi('ctimmins', 'T3am#winning')

@app.route('/')
def root():
    res = myDigi.listDataStreams().json()
    return render_template('index.html',
    						user={'name':'Chad'},
    						list=res["list"])
    
