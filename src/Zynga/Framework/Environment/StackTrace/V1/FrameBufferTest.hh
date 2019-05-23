<?hh // strict

namespace Zynga\Framework\Environment\StackTrace\V1;

use Zynga\Framework\Environment\StackTrace\V1\FrameBuffer;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCaseBase;

class FrameBufferTest extends TestCaseBase {

  public function testBuffer(): void {
    $fb = new FrameBuffer();

    $this->assertTrue($fb->addFromFrame(array('foo' => 'bar'), 'foo'));
    $this->assertEquals('foo=bar', $fb->get());
    $this->assertTrue($fb->addFromFrame(array('foo2' => 'bar2'), 'mid'));
    $this->assertEquals('foo=bar', $fb->get());
  }
  
}
