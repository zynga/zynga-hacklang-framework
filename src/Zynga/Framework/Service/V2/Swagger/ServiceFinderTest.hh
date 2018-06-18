<?hh //strict

namespace Zynga\Framework\Service\V2\Swagger;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V2\Swagger\ServiceFinder;
use Zynga\Framework\Service\V2\Service\Group\Config\Pattern;

class ServiceFinderTest extends TestCase {

  public function testConstruct(): void {
    $pattern = new Pattern();
    $pattern->codePath->set('Zynga\Poker\Services\Consumer');
    $pattern->filePath->set(CodePath::getRoot().'/include/Zynga/Poker/Services/Consumer');
    $serviceFinder = new ServiceFinder($pattern);
    $this->assertTrue($serviceFinder instanceof ServiceFinder);
  }

  public function testFind(): void {
    $pattern = new Pattern();
    $pattern->codePath->set('Zynga\Poker\Services\Consumer');
    $pattern->filePath->set(CodePath::getRoot().'/include/Zynga/Poker/Services/Consumer');
    $serviceFinder = new ServiceFinder($pattern);
    $this->assertTrue($serviceFinder->find());
    $services = $serviceFinder->getServices();
    $this->assertGreaterThan(0, $services->count());
  }

  public function testREADMEfileIsNotAService(): void {
    $pattern = new Pattern();
    $serviceFinder = new ServiceFinder($pattern);
    $this->assertFalse($serviceFinder->isFileNameService('README.md'));
    $this->assertFalse($serviceFinder->isFileNameService('README.hh'));
  }

  public function testInterfaceIsNotAService(): void {
    $pattern = new Pattern();
    $serviceFinder = new ServiceFinder($pattern);
    $this->assertFalse($serviceFinder->isFileNameService('SomeInterface.hh'));
    $this->assertFalse($serviceFinder->isFileNameService('Interface.hh'));
  }
}
