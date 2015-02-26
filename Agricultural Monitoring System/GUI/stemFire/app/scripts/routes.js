'use strict';
/**
 * @ngdoc overview
 * @name stemFireApp:routes
 * @description
 * # routes.js
 *
 * Configure routes for use with Angular, and apply authentication security
 */
angular.module('stemFireApp')

  // configure views; the authRequired parameter is used for specifying pages
  // which should only be available while logged in
  .config(['$routeProvider', function($routeProvider) {
    $routeProvider
      .when('/', {
        templateUrl: 'views/main.html',
        controller: 'MainCtrl'
      })

      .when('/chat', {
        templateUrl: 'views/chat.html',
        controller: 'ChatCtrl'
      })

      .when('/leaves', {
        templateUrl: 'views/leaves.html',
        contoller: 'LeavesCtrl'
      })

      .otherwise({redirectTo: '/'});
  }]);