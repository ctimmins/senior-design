var app = {

	initialize: function() {
		app.baseURL = 'https://login.etherios.com';
		app.firebase = new Firebase(config.firebaseURL);
		console.log('this is it:');
		console.log(Digi.listDataStreams('ctimmins', 'T3am#winning'));
	},

};

/**
 * Digi Device Cloud and ConnectPort Utility functions
 */
var Digi = {

	baseURL: 'https://login.etherios.com',

	/**
	 * check status of ConnectPort(s)
	 * @param  {string} deviceID mac address of ConnectPort
	 * @param  {string} user     username for Device Cloud account
	 * @param  {string} pw       password for Device Cloud account
	 * @return {xml}           
	 */
	checkStatus: function (deviceID, user, pw) {

		var headers = {
			'Authorization': 'Basic ' + btoa(user +':' + pw),
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
			'Authorization': 'Basic ' + btoa(user +':' + pw),
			'Content-Type': 'text/xml; charset="UTF-8"'
		}

		var payload = Digi.message.querySetting(deviceID);

		Digi.post(Digi.baseURL+'/ws/sci', payload, headers);
	},

	listDataStreams: function (user, pw) {
		var headers = {
			'Authorization': 'Basic ' + btoa(user +':' + pw),
			'Data-Type': 'json'
		}

		var res = Digi.get(Digi.baseURL+'/ws/v1/streams/inventory', headers);

		return res;
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
			},
			type: "POST",
			url: url,
			processData: false,
			data: data
		});
	},

	/**
	 * HTTP GET method for device cloud with custom headers
	 * @param  {string} url     url for GET request
	 * @param  {string} headers custom headers for authentication
	 * @return {[type]}         [description]
	 */
	get: function(url, headers) {
		$.ajax({
			beforeSend: function (xhr) {
				$.each(headers, function (key, val) {
					xhr.setRequestHeader(key, val);
				});
			},
			type: "GET",
			url: url
		})
		.done(function(data) {
			return data;
		});
	},

	/**
	 * various messages for digi device cloud commands
	 * @type {Object}
	 */
	message: {

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
}