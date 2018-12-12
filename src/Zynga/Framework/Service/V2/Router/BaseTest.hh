<?hh //strict

namespace Zynga\Framework\Service\V2\Router;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Service\V2\Test\Router\Mock;
use Zynga\Framework\Service\V2\Router\Request;
use Zynga\Framework\Service\V2\Router\Response;

use Zynga\Framework\Service\V2\Interfaces\ServerInterface;
use Zynga\Framework\Service\V2\Interfaces\Exposure\Filter as BaseFilter;
use Zynga\Framework\Service\V2\Service\Group\Config\Base as ServiceConfigBase;
use Zynga\Framework\Service\V2\Service\Group\Config\Pattern as ConfigPattern;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

use Zynga\Framework\Type\V1\StringBox;

class MockConfig extends ServiceConfigBase {

  public function __construct() {
    parent::__construct();
    $this->initConfiguration();
  }

  private function initConfiguration(): void {
    $legacyConfig = new ConfigPattern();
    $legacyConfig->codePath->set('Zynga\Framework\Service\V2\Test');
    $legacyConfig->filePath->set(CodePath::getRoot().'/include/Zynga/Framework/Service/V2/Test');
    $legacyConfig->mustImplementFilter->add(BaseFilter::class);
    $this->patterns->add($legacyConfig);
  }

}

class BaseTest extends TestCase {

  public function testMockConfig(): void {

    $config = new MockConfig();
    $obj = new Mock($config);

    $obj->setHandlerFromString(Mock::NOOP_HANDLER);

    $this->assertEquals(
      'Zynga\Framework\Service\V2\Test\Router\Services',
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

    $config = new MockConfig();
    $obj = new Mock($config);

    $request = $obj->request();

    $obj->setHandlerFromString(Mock::NOOP_HANDLER);

    if ($request instanceof Request) {
      $request->servicePath->set('/Valid');
    }

    $this->assertTrue($obj->handle());

  }

  public function testInvalidRoute(): void {

    $config = new MockConfig();
    $obj = new Mock($config);

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

  public function testRouterServerType(): void {
    $config = new MockConfig();
    $obj = new Mock($config);
    $this->assertClassImplements(ServerInterface::class, $obj->server());
  }

}
