var app = {

	initialize: function() {
		app.baseURL = 'http://login.etherios.com';
		app.firebase = new Firebase(config.firebaseURL);

	},

};

var Digi = {

	baseURL: 'http://login.etherios.com',

	/**
	 * check status of ConnectPort(s)
	 * @param  {string} deviceID mac address of ConnectPort
	 * @param  {string} user     username for Device Cloud account
	 * @param  {string} pw       password for Device Cloud account
	 * @return {xml}           
	 */
	checkStatus: function (deviceID, user, pw) {

		var headers = {
			'Authorization': 'Basic ' + user +':' + pw,
			'Content-Type': 'text/xml; charset="UTF-8"'
		}

		var payload = Digi.message.queryState(deviceID);

		Digi.post(Digi.baseURL+'/ws/sci', payload, headers);

	},

	/**
	 * get device information of ConnectPort(s)
	 * @param  {string} deviceID mac address of ConnectPort
	 * @param  {string} user     username for Device Cloud account
	 * @param  {string} pw       password for Device Cloud account
	 * @return {xml}
	 */
	getDeviceInfo: function (deviceID, user, pw) {
		
		var headers = {
			'Authorization': 'Basic ' + user +':' + pw,
			'Content-Type': 'text/xml; charset="UTF-8"'
		}

	},

	/**
	 * get device settings of ConnectPort
	 * @param  {string} deviceID mac address of ConnectPort
	 * @param  {string} user     username for Device Cloud account
	 * @param  {string} pw       password for Device Cloud account
	 * @return {xml}
	 */
	getSettings: function (deviceID, user, pw) {
		var headers = {
			'Authorization': 'Basic ' + user +':' + pw,
			'Content-Type': 'text/xml; charset="UTF-8"'
		}

		var payload = Digi.message.querySetting(deviceID);
	},

	getAllData: function () {

	},

	getOneDataStream: function (deviceID) {

	},

	/**
	 * HTTP POST method for device cloud with custom headers
	 * @param  {string} url     url for http request
	 * @param  {string} data    data to send to url
	 * @param  {json} headers   custom headers for authentication
	 * @return {[type]}         [description]
	 */
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

	/**
	 * various messages for digi device cloud commands
	 * @type {Object}
	 */
	message = {

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

	};
}