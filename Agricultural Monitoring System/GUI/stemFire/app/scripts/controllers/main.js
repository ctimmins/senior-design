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
    	var obj = {
    		timeStamp: snapshot.key(),
    		nodeData: snapshot.exportVal()
    	};

    	$scope.snapshot.push(obj);    	
    });

    nodeRef.on('child_removed', function (snapshot) {
    	console.log('removing record');
    	nodeParsing.removeRecord($scope.snapshot, 'timeStamp', snapshot.key());
    });

 
    
  });
