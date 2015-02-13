'use strict';

describe('Service: nodeParsing', function () {

  // load the service's module
  beforeEach(module('stemFireApp'));

  // instantiate service
  var nodeParsing;
  beforeEach(inject(function (_nodeParsing_) {
    nodeParsing = _nodeParsing_;
  }));

  it('should do something', function () {
    expect(!!nodeParsing).toBe(true);
  });

});
