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

    $scope.nodeData = fbutil.syncArray('Test/2', {limitToLast: 10});
    
    $scope.snapshot = [];

    var nodeRef = $scope.nodeData.$inst().$ref();

    nodeRef.orderByKey().on('child_added', function(snapshot) {
    	var obj = {};
    	obj[snapshot.key()] = snapshot.exportVal(); 
    	$scope.snapshot.push(obj);
    	console.log(snapshot.key());
    	snapshot.forEach(function(child) {
    		console.log(child.key() + ': ' + JSON.stringify(child.exportVal()));
    	});

    	console.log('');
    	console.log($scope.snapshot);
    	console.log('-------------------')
    	

    });

    nodeRef.on('child_removed', function(snapshot) {
    	var index = $scope.snapshot.indexOf(snapshot.key());
    	if (index > -1) {
    		$scope.snapshot.splice(index, 1);
    	}
    });

 
    
  });
