<?hh //strict

namespace Zynga\Framework\Service\V1\Router;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Service\V1\Test\Router\Mock;
use Zynga\Framework\Service\V1\Router\Request;
use Zynga\Framework\Service\V1\Router\Response;

use Zynga\Framework\Type\V1\StringBox;

class BaseTest extends TestCase {

  public function testMockConfig(): void {

    $obj = new Mock();

    $obj->setHandlerFromString(Mock::NOOP_HANDLER);

    $this->assertEquals(
      'Zynga\Framework\Service\V1\Test\Router\Services',
      $obj->getClassBase(),
    );

    // verify that we setup the request object correct
    $this->assertTrue($obj->request() instanceof Request);

    // verify that object caching works
    $this->assertTrue($obj->request() instanceof Request);

    // verify that we setup the response object correctly
    $this->assertTrue($obj->response() instanceof Response);

    // verify that object caching works
    $this->assertTrue($obj->response() instanceof Response);

  }

  public function testValidRoute(): void {

    $obj = new Mock();

    $request = $obj->request();

    $obj->setHandlerFromString(Mock::NOOP_HANDLER);

    if ($request instanceof Request) {
      $request->servicePath->set('/Mock/V1/Noop');
    }

    $this->assertTrue($obj->handle());

  }

  public function testInvalidRoute(): void {

    $obj = new Mock();

    $request = $obj->request();

    $obj->setHandlerFromString(Mock::NOOP_HANDLER);

    if ($request instanceof Request) {
      $request->servicePath->set('/Mock/V1/ServiceNotFound');
    }

    $this->assertFalse($obj->handle());

    $errorMessage = $obj->response()->message()->at(0);

    if ($errorMessage instanceof StringBox) {
      $this->assertEquals('SERVICE_NOT_FOUND', $errorMessage->get());
    }

  }

}
