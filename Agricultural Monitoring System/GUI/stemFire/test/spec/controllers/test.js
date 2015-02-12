'use strict';

describe('Controller: TestctrlCtrl', function () {

  // load the controller's module
  beforeEach(module('stemFireApp'));

  var TestctrlCtrl,
    scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    TestctrlCtrl = $controller('TestctrlCtrl', {
      $scope: scope
    });
  }));

  it('should attach a list of awesomeThings to the scope', function () {
    expect(scope.awesomeThings.length).toBe(3);
  });
});
