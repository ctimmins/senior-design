'use strict';

/**
 * @ngdoc service
 * @name stemFireApp.nodeParsing
 * @description
 * # nodeParsing
 * Factory in the stemFireApp.
 */
angular.module('stemFireApp')
  .factory('nodeParsing', function () {
    // Service logic
    // ...

    var meaningOfLife = 42;

    // Public API here
    return {
      someMethod: function () {
        return meaningOfLife;
      }
    };
  });
