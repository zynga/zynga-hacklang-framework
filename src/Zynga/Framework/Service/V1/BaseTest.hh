<?hh //strict

namespace Zynga\Framework\Service\V1;

use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Service\V1\Handler\Http as HttpHandler;
use Zynga\Framework\Service\V1\Test\BrokenHandle;
use Zynga\Framework\Service\V1\Test\Invalid;
use Zynga\Framework\Service\V1\Test\TestSafe;
use Zynga\Framework\Service\V1\Test\Valid;
use Zynga\Framework\Service\V1\Test\ValidRequired;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BaseTest extends TestCase {

  public function testValid(): void {
    $obj = new Valid();
    $this->assertTrue(is_object($obj->request()));
    $this->assertTrue(is_object($obj->response()));
    $this->assertTrue($obj->handle());
    $this->assertFalse($obj->setHandlerFromString('a-unsupported-method'));
  }

  public function test_setHandlerFromString_Noop(): void {
    $obj = new TestSafe();
    $this->assertTrue($obj->setHandlerFromString(TestSafe::NOOP_HANDLER));
  }

  public function testSafe(): void {
    $testField = 'id';
    $testValue = mt_rand(1, 10000);
    $obj = new TestSafe();
    $obj->setHandlerFromString('HTTP');
    $handler = $obj->getHandler();
    if ($handler instanceof HttpHandler) {
      $handler->setSendHttpCode(false);
      $handler->setSendHeaders(false);
    }

    $this->assertTrue(
      SuperGlobals::setRequestPairing($testField, $testValue),
    );
    $obj->handleRequest();
    $this->expectOutput('{"success":true,"code":200}');
    $this->assertTrue(SuperGlobals::unsetRequestPairing($testField));
  }

  public function testBrokenHandle(): void {
    $testField = 'id';
    $testValue = mt_rand(1, 10000);
    $obj = new BrokenHandle();
    $obj->setHandlerFromString('HTTP');
    $handler = $obj->getHandler();
    if ($handler instanceof HttpHandler) {
      $handler->setSendHttpCode(false);
      $handler->setSendHeaders(false);
    }

    $this->assertTrue(
      SuperGlobals::setRequestPairing($testField, $testValue),
    );
    $obj->handleRequest();
    $this->expectOutput(
      '{"success":false,"code":400,"message":["Zynga\\\\Framework\\\\Exception\\\\V1\\\\Exception: OH-NOZ"]}',
    );
    $this->assertTrue(SuperGlobals::unsetRequestPairing($testField));
  }

  public function testSoftBroken(): void {
    $testField = 'id';
    $testValue = mt_rand(1, 10000);
    $obj = new Invalid();
    $obj->setHandlerFromString('HTTP');
    $handler = $obj->getHandler();
    if ($handler instanceof HttpHandler) {
      $handler->setSendHttpCode(false);
      $handler->setSendHeaders(false);
    }

    $this->assertTrue(
      SuperGlobals::setRequestPairing($testField, $testValue),
    );
    $obj->handleRequest();
    $this->expectOutput('{"success":false,"code":400}');
    $this->assertTrue(SuperGlobals::unsetRequestPairing($testField));
  }

  public function test_http_requiredRequestParam(): void {
    SuperGlobals::clearRequest();
    $obj = new ValidRequired();
    $obj->setHandlerFromString('HTTP');
    $handler = $obj->getHandler();
    if ($handler instanceof HttpHandler) {
      $handler->setSendHttpCode(false);
      $handler->setSendHeaders(false);
    }

    $obj->handleRequest();
    $this->expectOutput(
      '{"success":false,"code":400,"message":["MissingKeyFromImportDataException: Failed to import one or more fields. Make sure that the data you\'re importing is of a supported type (either string or JSON). fields=[\"id\",\"someOtherValue\"]"]}'
    );
    SuperGlobals::clearRequest();
  }

  public function test_http_requiredResponseParam(): void {
    SuperGlobals::clearRequest();
    $obj = new ValidRequired();
    $obj->setHandlerFromString('HTTP');
    $handler = $obj->getHandler();
    if ($handler instanceof HttpHandler) {
      $handler->setSendHttpCode(false);
      $handler->setSendHeaders(false);
    }

    SuperGlobals::setRequestPairing('id', 12398755);
    SuperGlobals::setRequestPairing('someOtherValue', 634563456);
    $obj->handleRequest();
    $this->expectOutput(
      '{"success":false,"code":400,"message":["MissingDataFromExportDataException: fields=[\"id\",\"yetAnotherValue\"]"]}',
    );
    SuperGlobals::clearRequest();
  }
}
