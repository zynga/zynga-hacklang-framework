<?hh //strict

namespace Zynga\Framework\Service\V1\Test\Handler;

use Zynga\Framework\Service\V1\Test\Handler\MockEmptyHandler;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class MockEmptyHandlerTest extends TestCase {
  public function testParseRequest(): void {
    $mockEmptyHandler = new MockEmptyHandler();
    $this->assertTrue($mockEmptyHandler->parseRequest());
  }

  public function testHandleResponse(): void {
    $mockEmptyHandler = new MockEmptyHandler();
    $this->assertTrue($mockEmptyHandler->handleResponse());
  }
}
