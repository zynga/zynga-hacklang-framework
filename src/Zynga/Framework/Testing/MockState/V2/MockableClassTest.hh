<?hh // strict

namespace Zynga\Framework\Testing\MockState\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCaseBase;
use Zynga\Framework\Testing\MockState\V2\MockableClass;
use Zynga\Framework\Testing\MockState\V2\Mocks\FactoryExample;

class MockableClassTest extends TestCaseBase {

  public function testDetection(): void {

    $mockedClass = new MockableClass(FactoryExample::class);

    $this->assertEquals(
      'unknown',
      $mockedClass->detectFunctionName(Vector {'IamNotHere'}),
    );

  }

}
