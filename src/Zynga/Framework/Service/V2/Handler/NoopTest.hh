<?hh //strict

namespace Zynga\Framework\Service\V2\Handler;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Service\V2\Test\Handler\MockNoopEmptyHandler;
use Zynga\Framework\Service\V2\Test\Valid as ValidService;
use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Exception\V1\Exception;

class NoopTest extends TestCase {

  public function testValid(): void {
    $obj = new MockNoopEmptyHandler();
    $this->assertFalse($obj->handleGenericFailure());
    $this->assertFalse($obj->handleGenericSuccess());
    $e = new Exception();
    $this->assertFalse($obj->handleFailureException($e));
    $this->assertEquals(
      'Zynga\Framework\Exception\V1\Exception',
      $obj->createShortExceptionName($e),
    );
  }

  public function testValid_Service(): void {
    $obj = new MockNoopEmptyHandler();
    $svc = new ValidService();
    $this->assertTrue($obj->setService($svc));
    $this->assertTrue($obj->getService() instanceof ServiceInterface);
  }

  public function testValid_HandleGenericFailure(): void {
    $obj = new MockNoopEmptyHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertTrue($obj->handleGenericFailure());
    $this->assertFalse($svc->response()->success()->get());
  }

  public function testValid_FailureException(): void {
    $obj = new MockNoopEmptyHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $e = new Exception('some-error-message');
    $this->assertTrue($obj->handleFailureException($e));
    $message = $svc->response()->message()->at(0);
    if ($message instanceof StringBox) {
      $this->assertEquals(
        'Zynga\Framework\Exception\V1\Exception: some-error-message',
        $message->get(),
      );
    }
    $this->assertFalse($svc->response()->success()->get());
  }

  public function testValid_HandleGenericSuccess(): void {
    $obj = new MockNoopEmptyHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertTrue($obj->handleGenericSuccess());
    $this->assertTrue($svc->response()->success()->get());
  }

  public function testValid_HandleRequest(): void {
    $obj = new MockNoopEmptyHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertTrue($obj->parseRequest());
    $this->assertTrue($obj->handleResponse());
  }

}
