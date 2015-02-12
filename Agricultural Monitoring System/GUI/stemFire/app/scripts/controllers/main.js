'use strict';

/**
 * @ngdoc function
 * @name stemFireApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the stemFireApp
 */
angular.module('stemFireApp')
  .controller('MainCtrl', function ($scope, fbutil) {

    $scope.nodeData = fbutil.syncObject('Test/2');
    
    var nodeRef = fbutil.ref('Test/2');
    nodeRef.orderByKey().on('child_added', function(snapshot){
    	console.log(snapshot.key());
    	var sensorRef = nodeRef.child(snapshot.key());
    	sensorRef.on('child_added', function(s_snapshot){
    		console.log('  sensor height: ' + s_snapshot.key());
    		var valRef = sensorRef.child(s_snapshot.key());
    		valRef.on('child_added', function(v_snapshot){
    			console.log('          ' + v_snapshot.key() + ': ' + v_snapshot.val());
    		});
    		console.log('');

    	});
    	console.log('-------------------');
    });
    
  });
