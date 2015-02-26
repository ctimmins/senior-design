'use strict';

describe('Controller: LeavesCtrl', function () {

  // load the controller's module
  beforeEach(module('stemFireApp'));

  var LeavesCtrl,
    scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    LeavesCtrl = $controller('LeavesCtrl', {
      $scope: scope
    });
  }));

  it('should attach a list of awesomeThings to the scope', function () {
    expect(scope.awesomeThings.length).toBe(3);
  });
});
