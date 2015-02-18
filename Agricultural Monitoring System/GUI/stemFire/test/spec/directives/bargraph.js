'use strict';

describe('Directive: barGraph', function () {

  // load the directive's module
  beforeEach(module('stemFireApp'));

  var element,
    scope;

  beforeEach(inject(function ($rootScope) {
    scope = $rootScope.$new();
  }));

  it('should make hidden element visible', inject(function ($compile) {
    element = angular.element('<bar-graph></bar-graph>');
    element = $compile(element)(scope);
    expect(element.text()).toBe('this is the barGraph directive');
  }));
});
