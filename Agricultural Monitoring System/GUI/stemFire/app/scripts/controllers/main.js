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

    $scope.chartType = 'area-spline';

    $scope.vwc = [
      {
        'day': '2015-2-21 00:00:00',
        '0': 39,
        '1': 21,
        '2': 26,
        '3': 12,
        '4': 17,
        '5': 11
      },
      {
        'day': '2015-2-21 00:05:00',
        '0': 36,
        '1': 22,
        '2': 24,
        '3': 10,
        '4': 18,
        '5': 13
      },
      {
        'day': '2015-2-21 00:10:00',
        '0': 34,
        '1': 28,
        '2': 21,
        '3': 9,
        '4': 17,
        '5': 13
      },
      {
        'day': '2015-2-21 00:15:00',
        '0': 30,
        '1': 27,
        '2': 19,
        '3': 8,
        '4': 19,
        '5': 14
      },
      {
        'day': '2015-2-21 00:20:00',
        '0': 26,
        '1': 22,
        '2': 18,
        '3': 8,
        '4': 18,
        '5': 16
      },
      {
        'day': '2015-2-21 00:25:00',
        '0': 23,
        '1': 23,
        '2': 17,
        '3': 7,
        '4': 19,
        '5': 15
      },
      {
        'day': '2015-2-21 00:30:00',
        '0': 39,
        '1': 21,
        '2': 26,
        '3': 12,
        '4': 17,
        '5': 11
      },
      {
        'day': '2015-2-21 00:35:00',
        '0': 36,
        '1': 22,
        '2': 24,
        '3': 10,
        '4': 18,
        '5': 13
      },
      {
        'day': '2015-2-21 00:40:00',
        '0': 34,
        '1': 28,
        '2': 21,
        '3': 9,
        '4': 17,
        '5': 13
      },
      {
        'day': '2015-2-21 00:45:00',
        '0': 30,
        '1': 27,
        '2': 19,
        '3': 8,
        '4': 19,
        '5': 14
      },
      {
        'day': '2015-2-21 00:50:00',
        '0': 26,
        '1': 22,
        '2': 18,
        '3': 8,
        '4': 18,
        '5': 16
      },
      {
        'day': '2015-2-21 00:55:00',
        '0': 23,
        '1': 23,
        '2': 17,
        '3': 7,
        '4': 19,
        '5': 15
      },
      {
        'day': '2015-2-21 01:00:00',
        '0': 39,
        '1': 21,
        '2': 26,
        '3': 12,
        '4': 17,
        '5': 11
      },
      {
        'day': '2015-2-21 01:05:00',
        '0': 36,
        '1': 22,
        '2': 24,
        '3': 10,
        '4': 18,
        '5': 13
      },
      {
        'day': '2015-2-21 01:10:00',
        '0': 34,
        '1': 28,
        '2': 21,
        '3': 9,
        '4': 17,
        '5': 13
      },
      {
        'day': '2015-2-21 01:15:00',
        '0': 30,
        '1': 27,
        '2': 19,
        '3': 8,
        '4': 19,
        '5': 14
      },
      {
        'day': '2015-2-21 01:20:00',
        '0': 26,
        '1': 22,
        '2': 18,
        '3': 8,
        '4': 18,
        '5': 16
      },
      {
        'day': '2015-2-21 01:25:00',
        '0': 23,
        '1': 23,
        '2': 17,
        '3': 7,
        '4': 19,
        '5': 15
      }
    ];

    $scope.schema = {
      day: {
        type: 'datetime',
        format: '%Y-%m-%d %H:%M:%S',
        name: 'Date'
      },
      0: {
        type: 'numeric',
        name: 'Moisture L0'
      },
      1: {
        type: 'numeric',
        name: 'Temp L1'
      },
      2: {
        type: 'numeric',
        name: 'Moisture L2'
      },
      3: {
        type: 'numeric',
        name: 'Moisture L3'
      },
      4: {
        type: 'numeric',
        name: 'Temp L4'
      },
      5: {
        type: 'numeric',
        name: 'Moisture L5'
      },
    };

    $scope.options = {
      rows: [
        {
          key: '0',
          axis: 'y',
          type: $scope.chartType
        }, 
        {
          key: '1',
          axis: 'y2'
        },
        {
          key: '2',
          axis: 'y'
        },
        {
          key: '3',
          axis: 'y'
        },
        {
          key: '4',
          axis: 'y2'
        },
        {
          key: '5',
          axis: 'y'
        },
      ],

      legend: {
        'selector': false
      },

      xAxis: {
        key: 'day',
        selector: false,
        displayFormat: '%H:%M:%S'
      },

      yAxis: {
        label: 'Volumetric Water Content (%)'
      },

      y2Axis: {
        label: 'Temperature (C)'
      },

      type: $scope.chartType,

    };

    $scope.updateChartType = function(sel){
      $scope.options.rows.forEach(function(row) {
        row.type = sel;
      });
    };
    
  });
