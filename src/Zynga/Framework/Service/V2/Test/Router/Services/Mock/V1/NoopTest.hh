<?hh //strict

namespace Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Noop;

use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Request;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Response;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Server;

class NoopTest extends TestCase {

  public function test_valid(): void {
    $obj = new Noop();
    $obj->setHandlerFromString(Noop::NOOP_HANDLER);
    $this->assertTrue($obj->handle());
  }

  public function test_request(): void {
    $obj = new Noop();
    $this->assertEquals(Request::class, get_class($obj->request()));
    $this->assertEquals(Request::class, get_class($obj->request()));
  }

  public function test_response(): void {
    $obj = new Noop();
    $this->assertEquals(Response::class, get_class($obj->response()));
    $this->assertEquals(Response::class, get_class($obj->response()));
  }

  public function test_server(): void {
    $obj = new Noop();
    $this->assertEquals(Server::class, get_class($obj->server()));
    $this->assertEquals(Server::class, get_class($obj->server()));
  }
}
