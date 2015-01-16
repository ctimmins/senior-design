var app = {

	initialize: function() {
		app.baseURL = 'http://login.etherios.com';
		app.firebase = new Firebase(config.firebaseURL);

	},

	checkStatus: function (deviceID, user, pw) {

		var headers = {
			'Authorization': 'Basic ' + user +':' + pw,
			'Content-Type': 'text/xml; charset="UTF-8"'
		}

		var payload = digiMessage.queryState(deviceID);

		app.post(baseURL+'/ws/sci', payload, headers);

	},

	getDeviceInfo: function (deviceID, user, pw) {
		
		var headers = {
			'Authorization': 'Basic ' + user +':' + pw,
			'Content-Type': 'text/xml; charset="UTF-8"'
		}

	},

	getSettings: function () {

	},

	getAllData: function () {

	},

	getOneData: function (deviceID) {

	},

	post: function(url, data, headers) {
		$.ajax({
			beforeSend: function (xhr) {
				$.each(headers, function (key, val) {
					xhr.setRequestHeader(key, val);
				});
				xhr.setRequestHeader('Content-Length', data.length);
			}
			type: "POST",
			url: url,
			processData: false,
			data: data
		});
	}

};

/**
 * returns the xml string for http request needed for various device cloud operations
 * @type {Object}
 */
var digiMessage = {

	queryState: function (target) {
		var message = 
					'<sci_request version = "1.0"' +
					  	'<send_message>' +
						  	'<targets>' +
						  		'<device id="'+ target +'"/>' +
					  		'</targets>' +
					  		'<rci_request version="1.1"' +
					  			'<query_state/>' +
				  			'</rci_request>' +
			  			'</send_message>' +
		  			'</sci_request>';
		
		return message;
	},

	querySetting: function (target) {
		var message = 
					'<sci_request version = "1.0"' +
					  	'<send_message>' +
						  	'<targets>' +
						  		'<device id="'+ target +'"/>' +
					  		'</targets>' +
					  		'<rci_request version="1.1"' +
					  			'<query_setting/>' +
				  			'</rci_request>' +
			  			'</send_message>' +
		  			'</sci_request>';
		
		return message;
	},

}