<?hh //strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V1\Swagger\ServiceFinder;

class ServiceFinderTest extends TestCase {

  public function testConstruct(): void {
    $codePath = 'Zynga\Poker\Services\Consumer';
    $consumerPath = CodePath::getRoot().'/include/Zynga/Poker/Services/Consumer';
    $serviceFinder = new ServiceFinder($codePath, $consumerPath);
    $this->assertTrue($serviceFinder instanceof ServiceFinder);
  }

  public function testFind(): void {
    $codePath = 'Zynga\Poker\Services\Consumer';
    $consumerPath = CodePath::getRoot().'/include/Zynga/Poker/Services/Consumer';
    $serviceFinder = new ServiceFinder($codePath, $consumerPath);
    $this->assertTrue($serviceFinder->find());
    $services = $serviceFinder->getServices();
    $this->assertGreaterThan(0, $services->count());
  }

  public function testREADMEfileIsNotAService(): void {
    $serviceFinder = new ServiceFinder('', '');
    $this->assertFalse($serviceFinder->isFileNameService('README.md'));
    $this->assertFalse($serviceFinder->isFileNameService('README.hh'));
  }

  public function testInterfaceIsNotAService(): void {
    $serviceFinder = new ServiceFinder('', '');
    $this->assertFalse($serviceFinder->isFileNameService('SomeInterface.hh'));
    $this->assertFalse($serviceFinder->isFileNameService('Interface.hh'));
  }
}
