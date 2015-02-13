'use strict';

/**
 * @ngdoc function
 * @name stemFireApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the stemFireApp
 */
angular.module('stemFireApp')
  .controller('MainCtrl', function ($scope, fbutil, nodeParsing) {

    $scope.nodeData = fbutil.syncArray('Test/2', {limitToLast: 10});
    
    $scope.snapshot = [];

    window.snapTest = $scope.snapshot;

    var nodeRef = $scope.nodeData.$inst().$ref();

    nodeRef.orderByKey().on('child_added', function (snapshot) {
    	var obj = {};
    	obj.timeStamp = snapshot.key();
    	obj.nodeData = snapshot.exportVal(); 
    	$scope.snapshot.push(obj);    	
    });

    nodeRef.on('child_removed', function (snapshot) {
    	console.log('removing record');
    	nodeParsing.removeRecord($scope.snapshot, 'timeStamp', snapshot.key());
    	// var index = $.grep($scope.snapshot, function(e) {
    	// 	return e.timeStamp === snapshot.key();
    	// });

    	// if (index.length == 0) {
    	// 	console.log("not found");
    	// }
    	// else if (index.length == 1) {
    	// 	console.log (index[0].timeStamp);
    	// }
    	console.log($scope.snapshot);
    });

 
    
  });
