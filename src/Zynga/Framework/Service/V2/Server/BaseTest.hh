<?hh // strict

namespace Zynga\Framework\Service\V2\Server;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V2\Interfaces\ServerInterface;
use Zynga\Framework\Service\V2\Test\Server as TestServer;
use Zynga\Framework\Service\V2\Test\Valid;


class BaseTest extends TestCase {

  public function testServerClass() : void {
    $service = new Valid();
    $this->assertClassImplements(ServerInterface::class, $service->server());
  }

  public function testServerValue() : void {
    $service = new Valid();
    $this->assertNotEmpty($service->server()->PHP_SELF);
  }
}
