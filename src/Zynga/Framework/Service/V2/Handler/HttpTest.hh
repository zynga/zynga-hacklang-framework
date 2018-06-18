<?hh //strict

namespace Zynga\Framework\Service\V2\Handler;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Environment\HTTP\HeaderContainer\V1\Interfaces\HeaderContainerInterface;
use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;
use Zynga\Framework\Service\V2\Response\Failure as ResponseFailure;
use Zynga\Framework\Service\V2\Test\Handler\MockHttpBrokenGetServiceHandler;
use Zynga\Framework\Service\V2\Test\Handler\MockHttpHandler;
use Zynga\Framework\Service\V2\Test\Valid as ValidService;
use Zynga\Framework\Service\V2\Test\ValidNoFields as ValidNoFieldsService;
use Zynga\Framework\Service\V2\Test\ValidRequired as ValidRequiredService;
use Zynga\Framework\StorableObject\V1\Exceptions\MissingDataFromExportDataException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\StringBox;

class HttpTest extends TestCase {
  public function test_handleGenericFailureNoService(): void {
    $obj = new MockHttpHandler();
    $this->assertFalse($obj->handleGenericFailure());
  }

  public function test_handleGenericSuccessNoService(): void {
    $obj = new MockHttpHandler();
    $this->assertFalse($obj->handleGenericSuccess());
  }

  public function test_handleFailureException(): void {
    $obj = new MockHttpHandler();
    $e = new Exception();
    $this->assertFalse($obj->handleFailureException($e));
    $this->assertEquals(
      'Zynga\Framework\Exception\V1\Exception',
      $obj->createShortExceptionName($e),
    );
    $this->assertTrue(
      $obj->getHttpHeaderContainer() instanceof HeaderContainerInterface,
    );
  }

  public function test_flagsInitalState(): void {
    $obj = new MockHttpHandler();
    // Check for default state for the object.
    $this->assertTrue($obj->getSendHeaders());
    $this->assertTrue($obj->getSendHttpCode());
    $this->assertTrue($obj->getSendJson());
  }

  public function test_flagChanges(): void {
    $obj = new MockHttpHandler();
    // Turn off the flags for the object
    $this->assertTrue($obj->setSendHeaders(false));
    $this->assertTrue($obj->setSendHttpCode(false));
    $this->assertTrue($obj->setSendJson(false));
    // See if the updated state exists
    $this->assertFalse($obj->getSendHeaders());
    $this->assertFalse($obj->getSendHttpCode());
    $this->assertFalse($obj->getSendJson());
  }

  public function test_createJsonForResponse_withNoHandlerSetup(): void {
    $obj = new MockHttpHandler();
    // since this handler isn't wired to a service it should handle it with a error message
    $this->assertEquals(
      '{"message":"invalid-service-for-export"}',
      $obj->createJsonForResponse(),
    );
  }

  public function test_createJsonForResponse_Ok(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $this->assertTrue($obj->setService($svc));
    $svc->response()->code()->set(200);
    $svc->response()->success()->set(true);
    $this->assertEquals(
      '{"success":true,"code":200}',
      $obj->createJsonForResponse(),
    );
  }

  public function test_createJsonForResponse_notOk(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $this->assertTrue($obj->setService($svc));
    $svc->response()->code()->set(400);
    $this->assertEquals(
      '{"success":false,"code":400}',
      $obj->createJsonForResponse(),
    );
  }

  public function test_createJsonForResponse_ResponseMissingData(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidRequiredService();
    $this->assertTrue($obj->setService($svc));
    $svc->response()->code()->set(200);
    $svc->response()->success()->set(true);
    $this->assertEquals(
      '{"success":false,"code":400,"message":["MissingDataFromExportDataException: fields=[\"id\",\"yetAnotherValue\"]"]}',
      $obj->createJsonForResponse(),
    );
  }

  public function test_parseRequest_withNoHandlerSetup(): void {
    $obj = new MockHttpHandler();
    $this->assertFalse($obj->parseRequest());
  }

  public function setupHttpBodyData(string $class, string $function): void {
    $file = CodePath::getRoot();
    $file .= '/tests/data/';
    $file .= str_replace('\\', '/', $class);
    $file .= '/';
    $file .= $function;
    $file .= '.json';
    SuperGlobals::setBodyFilePath($file);
  }

  public function resetHttpBodyData(): void {
    SuperGlobals::setBodyFilePath(SuperGlobals::DEFAULT_HTTP_BODY_FILE);
  }

  public function test_parseRequest(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $this->setupHttpBodyData(get_class($this), __FUNCTION__);
    $this->assertTrue($obj->setService($svc));
    $this->assertTrue($obj->parseRequest());
    $this->resetHttpBodyData();
  }

  public function test_parseRequestNoFields(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidNoFieldsService();
    $this->assertTrue($obj->setService($svc));
    $this->assertTrue($obj->parseRequest());
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_parseRequestExceptionTrap(): void {
    $obj = new MockHttpBrokenGetServiceHandler();
    $svc = new ValidNoFieldsService();
    $this->assertTrue($obj->setService($svc));
    $obj->parseRequest();
  }

  public function test_withService(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $this->assertTrue($obj->setService($svc));
    $this->assertTrue($obj->getService() instanceof ServiceInterface);
  }

  public function test_handleGenericFailure(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertTrue($obj->handleGenericFailure());
    $this->assertFalse($svc->response()->success()->get());
  }

  public function test_failureException(): void {
    $obj = new MockHttpHandler();
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

  public function test_handleGenericSuccess(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertTrue($obj->handleGenericSuccess());
    $this->assertTrue($svc->response()->success()->get());
  }

  public function test_createHttpCodeResponseFailure(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $mess = new StringBox();
    $mess->set('some-error-message');
    $svc->response()->message()->add($mess);
    $failure = $obj->createHttpCodeResponseFailure($svc);
    $this->assertTrue($failure instanceof ResponseFailure);
    $this->assertEquals(400, $failure->code()->get());
  }

  public function test_createMissingDataFailure(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $e = new MissingDataFromExportDataException('missing=a');
    $failure = $obj->createMissingDataFailure($svc, $e);
    $this->assertTrue($failure instanceof ResponseFailure);
    $this->assertEquals(400, $failure->code()->get());
  }

  public function test_handleResponse_noService(): void {
    $obj = new MockHttpHandler();
    $this->assertFalse($obj->handleResponse());
  }

  public function test_handleResponse(): void {
    $obj = new MockHttpHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertTrue($obj->handleResponse());
    $this->expectOutput('{"success":false,"code":400}');
  }

  public function test_handleResponse_ErrorTrap(): void {
    $obj = new MockHttpBrokenGetServiceHandler();
    $svc = new ValidService();
    $obj->setService($svc);
    $this->assertFalse($obj->handleResponse());
    $this->expectOutput('{"message":"uncaught-exception"}');
  }
}
