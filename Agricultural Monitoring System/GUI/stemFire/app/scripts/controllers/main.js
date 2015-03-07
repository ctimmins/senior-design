'use strict';

/**
 * @ngdoc function
 * @name stemFireApp.controller:MainCtrl
 * @description
 * # MainCtrl
 * Controller of the stemFireApp
 */
angular.module('stemFireApp')
  .controller('MainCtrl', ['$scope', '$firebaseArray', 'nodeParsing', function ($scope, $firebaseArray, nodeParsing) {

    //$scope.soilSensors = fbutil.syncArray('1/soil sensors', {limitToLast: 100});
    var s_ref = new Firebase("https://agmon.firebaseio.com/soil sensors")
    var ir_ref = new Firebase("https://agmon.firebaseio.com/IR data");
    var info_ref = new Firebase("https://agmon.firebaseio.com/node info");

    // Synchronized arrays 
    var soilSensors = $firebaseArray(s_ref);
    soilSensors.$watch(function(e) {
      console.log('key: '+e.key);
    })
    var IRSensors = $firebaseArray(ir_ref);
    var sensorInfo = $firebaseArray(info_ref);
    
    
    // NODE SELECTION AND DATA
    $scope.nodes = {}
    $scope.selectedNode = 1;
    var syncSoilNodes = {}
    // var nodeRef = s_ref.child($scope.selectedNode)
    // var nodeArray = $firebaseArray(nodeRef);
    // nodeArray.$watch(function(e) {
    //   console.log('key: '+e.key);
    // });
    info_ref.orderByKey().on('child_added', function(snapshot) {
      $scope.nodes[snapshot.key()] = snapshot.child('Name').val();
      syncSoilNodes[snapshot.key()] = $firebaseArray(s_ref.child(snapshot.key()));
      syncSoilNodes[snapshot.key()].$watch(function(e) {
        console.log('prev: ' + e.prevChild);
        console.log('child: ' + e.key);
      })
    });

    window.syncSoil = syncSoilNodes;

    $scope.nodeSelectChange = function(nodeID) {
      $scope.selectedNode = nodeID;
      nodeRef = s_ref.child($scope.selectedNode)
      $scope.nodeData = soilSensors.$getRecord($scope.selectedNode)
    };
    
    //Build data for graphs



    window.sdata = $scope.nodeData;
    $scope.vwc = [];
    $scope.temperature = [];

    // build data for selected node
    // var soilData = {}
    // s_ref.orderByKey().on('child_added', function(snapshot) {
    //   soilData[snapshot.key()] = snapshot.val();
    //   // if (snapshot.key() == $scope.selectedNode) {
    //   //   $scope.nodeData = soilData[snapshot.key()]
    //   //   console.log()
    //   // }
    // });
    
    // build data for vwc and soil temperature graph
    var v_obj = {};
    var t_obj = {};
    
    


    

    // $scope.soilSensors.$watch(function(obj) {
    //   console.log('event: ' + obj.event);
    //   console.log('key: ' + obj.key);
    // });
/*
    s_ref.orderByKey().on('child_added', function (snapshot) {

      var timeStamp = snapshot.key()
      var nodeData = snapshot.exportVal()
      
      // build data for vwc and soil temperature graph
      var v_obj = {};
      var t_obj = {};

      v_obj['timeStamp'] = timeStamp;
      t_obj['timeStamp'] = timeStamp;

      for (var key in nodeData) {
        v_obj[key] = parseFloat(nodeData[key]['vwc']);
        t_obj[key] = parseFloat(nodeData[key]['temp']);
      }

      $scope.vwc.push(v_obj);
      $scope.temperature.push(t_obj);
   	
    });
    
    window.vwc = $scope.vwc;
    window.temperature = $scope.temperature;

    // soilRef.on('child_removed', function (snapshot) {
    // 	console.log('removing record');
    // 	nodeParsing.removeRecord($scope.snapshot, 'timeStamp', snapshot.key());
    // });

*/
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

    $scope.temp_options = {
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
        label: 'Temperature (C)'
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
