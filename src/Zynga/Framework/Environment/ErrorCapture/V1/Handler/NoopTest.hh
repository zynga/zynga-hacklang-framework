<?hh // strict

namespace Zynga\Framework\ErrorCapture\V1\Handler;

use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Noop;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class NoopTest extends TestCase {
  public function test_impl(): void {
    // should trap the meessage.
    $handler = new Noop();
    $handler->start();
    trigger_error('I am a walrus');
    $handler->stop();
    $this->assertTrue(true);
  }
}
