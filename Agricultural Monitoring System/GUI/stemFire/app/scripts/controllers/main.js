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

    //initial latitude and longitude coordinates and node information
    var Latitude = 38.531869;
    var Longitude = -121.751864;
    $scope.markers = new Array();
    $scope.nodeInfo = new Array()

    //$scope.soilSensors = fbutil.syncArray('1/soil sensors', {limitToLast: 100});
    var s_ref = new Firebase("https://agmon.firebaseio.com/soil sensors")
    var ir_ref = new Firebase("https://agmon.firebaseio.com/IR data");
    var info_ref = new Firebase("https://agmon.firebaseio.com/node info");

    // Synchronized arrays 
    var soilSensors = $firebaseArray(s_ref);
    soilSensors.$watch(function(e) {
      //console.log('which_node: '+e.key);
    })
    var IRSensors = $firebaseArray(ir_ref);
    var sensorInfo = $firebaseArray(info_ref);
    sensorInfo.$loaded()
      .then(function(info) {
        Latitude = info.$getRecord($scope.selectedNode)["Latitude"] | Latitude;
        Longitude = info.$getRecord($scope.selectedNode)["Longitude"] | Longitude;
        sensorInfo.forEach(function(obj) {
          $scope.markers.push({
            lat: obj["Latitude"],
            lng: obj["Longitude"],
            message: obj["Name"],
            riseOnHover: true
          });
          $scope.nodeInfo.push(obj);
        });
      });


    window.sensorInfo = sensorInfo;
    
    // NODE SELECTION AND DATA
    $scope.nodes = {}
    $scope.selectedNode = 1;

    //Build data for graphs
    $scope.vwc = {};
    $scope.temperature = {};
    $scope.thisVwc = [];
    $scope.thisTemp = [];

    
    info_ref.orderByKey().on('child_added', function(node) {
      $scope.nodes[node.key()] = node.child('Name').val();
      //console.log('node: '+node.key());

      $scope.vwc[node.key()] = [];
      $scope.temperature[node.key()] = [];

      s_ref.child(node.key()).orderByKey().on('child_added', function(snapshot) {
        var timeStamp = snapshot.key();
        var nodeData = snapshot.exportVal();
        
        var t_obj = {};
        var v_obj = {};
        t_obj['timeStamp'] = timeStamp;
        v_obj['timeStamp'] = timeStamp;
        
        for (var key in nodeData) {
          v_obj[key] = parseFloat(nodeData[key]['vwc']);
          t_obj[key] = parseFloat(nodeData[key]['temp']);
        }

        $scope.vwc[node.key()].push(v_obj);
        $scope.temperature[node.key()].push(t_obj);
        if (node.key() == $scope.selectedNode) {
          $scope.thisVwc = $scope.vwc[node.key()];
          $scope.thisTemp = $scope.temperature[node.key()];
        }

      });

      //console.log('-------------------------');

    });

    $scope.nodeSelectChange = function(nodeID) {
      $scope.selectedNode = nodeID;
      $scope.thisVwc = $scope.vwc[$scope.selectedNode];
      $scope.thisTemp = $scope.temperature[$scope.selectedNode];
      Latitude = sensorInfo.$getRecord($scope.selectedNode)["Latitude"] | Latitude;
      Longitude = sensorInfo.$getRecord($scope.selectedNode)["Longitude"] | Longitude;
      //console.log($scope.thisVwc);
    };
    
    
    window.vwc = $scope.vwc;
    window.temperature = $scope.temperature;
    window.thisVwc = $scope.thisVwc;
    window.thisTemp = $scope.thisTemp;

    // build data for selected node
    // var soilData = {}
    // s_ref.orderByKey().on('child_added', function(snapshot) {
    //   soilData[snapshot.key()] = snapshot.val();
    //   // if (snapshot.key() == $scope.selectedNode) {
    //   //   $scope.nodeData = soilData[snapshot.key()]
    //   //   //console.log()
    //   // }
    // });
    
    // build data for vwc and soil temperature graph
    var v_obj = {};
    var t_obj = {};
    
    
    //Leaflet
    
    angular.extend($scope, {
      center: {
        lat: Latitude,
        lng: Longitude,
        zoom: 17,
      },
      defaults: {
        maxZoom: 17,
        minZoom: 17,
        draggable: false
      } 

    });

    $scope.$on('leafletDirectiveMarker.click', function(e, args) {
      var name = args.leafletEvent.target.options.message;
      sensorInfo.forEach(function(obj) {
        if (obj.Name == name) {
          $scope.nodeSelectChange(obj.$id);
        }
      });
    });

    

    // Angular Chart

    $scope.ChartType = 'donut';

    $scope.vwc_schema = {
      timeStamp: {
        type: 'datetime',
        format: '%Y-%m-%d %H:%M:%S',
        name: 'time'
      }
    }

    $scope.vwc_options = {
      data: {
        watchLimit: 5
      },
      rows: [
        {
          key: '0',
          axis: 'y',
          type: 'donut'
        },
        {
          key: '1',
          axis: 'y',
          type: 'donut'
        },
        {
          key: '2',
          axis: 'y',
          type: 'donut'
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
        label: 'Volumetric Water Content (%)',
        displayFormat: window.d3.format("0.2%")
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
      //console.log($scope.ChartType);
    };
    
  }]);
