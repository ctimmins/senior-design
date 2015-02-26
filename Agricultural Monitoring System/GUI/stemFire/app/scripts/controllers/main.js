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

    $scope.nodeData = fbutil.syncArray('1/soil sensors', {limitToLast: 10});
    
    $scope.snapshot = [];
    $scope.vwc = [];
    $scope.temperature = [];

    window.snapTest = $scope.snapshot;

    var nodeRef = $scope.nodeData.$inst().$ref();

    nodeRef.orderByKey().on('child_added', function (snapshot) {
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
    
    nodeRef.on('child_removed', function (snapshot) {
    	console.log('removing record');
    	nodeParsing.removeRecord($scope.snapshot, 'timeStamp', snapshot.key());
    });


    // Angular Chart

    $scope.ChartType = 'bar';

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
          axis: 'y'
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

      type: $scope.ChartType,

    };

    function onChartClick() {

    }

    $scope.updateChartType = function(sel){
      $scope.vwc_options.rows.forEach(function(row) {
        row.type = sel;
      });
    };
    
  }]);
