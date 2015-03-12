'use strict';

/**
 * @ngdoc service
 * @name stemFireApp.weather
 * @description
 * # weather
 * Factory in the stemFireApp.
 */
angular.module('stemFireApp')
  .service('weatherService', ['$q', '$http', function ($q, $http) {
    
    var API_ROOT = 'https://query.yahooapis.com/v1/public/yql?q=SELECT%20*%20FROM%20weather.forecast%20WHERE%20location%3D%22';
    var API_END = '%22&format=json&diagnostics=true';
    // Public API here
    return {
    	byZip: function (zip) {
    		var deferred = $q.defer();
    		var url = API_ROOT + zip + API_END;
    		console.log(url);
    		//call the yahoo weather api
    		$http.get(url)
    			.success(function(data) {
    				deferred.resolve(data.query.results.channel);
    			})
    			.error(function(err) {
    				console.log("Error getting yahoo weather");
    				deferred.reject(err);
    			});

			return deferred.promise;
    	},

    	byLocation: function(lat, lng) {

    	}
    };
  }]);
// https://query.yahooapis.com/v1/public/yql?q=SELECT%20*%20FROM%20weather.forecast%20location%3D%2295616%22&format=json&diagnostics=true
// https://query.yahooapis.com/v1/public/yql?q=SELECT%20*%20FROM%20weather.forecast%20WHERE%20location%3D%2295616%22&format=json&diagnostics=true&callback