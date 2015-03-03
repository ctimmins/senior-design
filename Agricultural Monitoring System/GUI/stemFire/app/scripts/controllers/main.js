'use strict';

/**
 * @ngdoc function
 * @name stemFireApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the stemFireApp
 */
angular.module('stemFireApp')
  .controller('MainCtrl', ['$scope', 'fbutil', 'nodeParsing', function ($scope, fbutil, nodeParsing) {

    $scope.soilSensors = fbutil.syncArray('1/soil sensors', {limitToLast: 100});
    
    $scope.snapshot = [];
    $scope.vwc = [];
    $scope.temperature = [];

    var soilRef = $scope.soilSensors.$inst().$ref();

    soilRef.orderByKey().on('child_added', function (snapshot) {
    	var obj = {
    		timeStamp: snapshot.key(),
    		nodeData: snapshot.exportVal()
    	};
      
      // build data for vwc graph
      var v_obj = {};
      v_obj['timeStamp'] = obj.timeStamp;
      for (var key in obj.nodeData) {
        v_obj[key] = parseFloat(obj.nodeData[key]['vwc']);
      }
      $scope.vwc.push(v_obj);

      //build data for temp graph
      var t_obj = {};
      t_obj['timeStamp'] = obj.timeStamp;
    	for (var key in obj.nodeData) {
        t_obj[key] = parseFloat(obj.nodeData[key]['temp']);
      }
      $scope.temperature.push(t_obj);

      $scope.snapshot.push(obj);    	
    });
    
    // soilRef.on('child_removed', function (snapshot) {
    // 	console.log('removing record');
    // 	nodeParsing.removeRecord($scope.snapshot, 'timeStamp', snapshot.key());
    // });

    window.snapshot = $scope.snapshot;
    window.vwc = $scope.vwc;

    // Angular Chart

    $scope.ChartType = 'area-spline';

    $scope.vwc_schema = {
      timeStamp: {
        type: 'datetime',
        format: '%Y-%m-%d %H:%M:%S',
        name: 'time'
      }
    }

    $scope.vwc_options = {
      rows: [
        {
          key: '0',
          axis: 'y',
          type: 'area-spline'
        }
      ],

      legend: {
        'selector': false
      },

      xAxis: {
        key: 'timeStamp',
        selector: false,
        displayFormat: '%H:%M:%S'
      },

      yAxis: {
        label: 'Volumetric Water Content (%)'
      },

    };

    function onChartClick() {

    }

    $scope.updateChartType = function(sel){
      $scope.vwc_options.rows.forEach(function(row) {
        row.type = sel;
      });
      console.log($scope.ChartType);
    };
    
  }]);
