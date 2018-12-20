<?hh // strict

namespace Zynga\Framework\Service\V2\Server;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V2\Interfaces\ServerGlobalsInterface;
use Zynga\Framework\Service\V2\Test\ServerGlobals as TestServer;
use Zynga\Framework\Service\V2\Test\Valid;


class BaseTest extends TestCase {

  public function testServiceReturnsServerObject() : void {
    $service = new Valid();
    $this->assertClassImplements(ServerGlobalsInterface::class, $service->serverGlobals());
  }

  public function testServerHasPopulatedValue() : void {
    $service = new Valid();
    $this->assertNotEmpty($service->serverGlobals()->PHP_SELF);
  }
}
