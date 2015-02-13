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

    // Public API here
    return {
      removeRecord: function (arr, attr, value) {
        var i = arr.length;
        while (i--) {
          if (arr[i] && arr[i].hasOwnProperty(attr) && (arguments.length > 2 && arr[i][attr] === value)) {
            arr.splice(i,1);
          }
        }        
        console.log('record removed');
        return arr;
      },

      addRecord: function () {

      },

      findEntryByTime: function (arr, time) {
        var entry = $.grep(arr, function(e) {
          return e.timeStamp === time;
        });

        if (entry.length === 0) {
          return null;
        }
        else if (entry.length === 1) {
          return entry[0];
        }
        // else multiple items were found
        else {
          return entry;
        }
      }
    };
  });
