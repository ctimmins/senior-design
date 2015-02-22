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

    // Angular Chart
    $scope.dataset = [
      {
        'day': '2013-01-08T00:00:00',
        'sales': 300,
        'income': 200,
        'customers': 30,
        'units': 130,
        'dayString': 'Montag'
      },
      {
        'day': '2013-01-03T00:00:00',
        'sales': 200,
        'income': 130,
        'customers': 20,
        'units': 120,
        'dayString': 'Dienstag'
      },
      {
        'day': '2013-01-04T00:00:00',
        'sales': 160,
        'income': 90,
        'customers': 50,
        'units': 150,
        'dayString': 'Mittwoch'
      },
      {
        'day': '2013-01-05T00:00:00',
        'sales': 400,
        'income': 240,
        'customers': 40,
        'units': 140,
        'dayString': 'Donnerstag'
      },
      {
        'day': '2013-01-06T00:00:00',
        'sales': 250,
        'income': 130,
        'customers': 60,
        'units': 160,
        'dayString': 'Freitag'
      },
      {
        'day': '2013-01-07T00:00:00',
        'sales': 250,
        'income': 220,
        'customers': 50,
        'units': 150,
        'dayString': 'Samstag'
      }
    ];

    $scope.schema = {
      day: {
        type: 'datetime',
        name: 'Date'
      },
      income: {
        type: 'numeric',
        name: 'income'
      }
    };

    $scope.options = {
      rows: [
        {
          key: 'income',
          type: 'bar',
        }, 
        {
          key: 'sales'
        },
        {
          key: 'units'
        }
      ],

      legend: {
        'selector': true
      },

      xAxis: {
        key: 'day',
        displayFormat: '%Y-%m-%d'
      },

      type: 'spline',

    };
 
    
  });
