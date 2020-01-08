<?hh // strict

namespace Zynga\Framework\Environment\StackTrace\V1;

use Zynga\Framework\Environment\StackTrace\V1\Shortner;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCaseBase;

class ShortnerTest extends TestCaseBase {

  public function testStackTrace(): void {

    $stacktrace =
      'class=Zynga\Framework\Environment\StackTrace\V1\Shortner::function=toString::line=15|class=Zynga\Framework\Environment\StackTrace\V1\ShortnerTest::function=testStackTrace';

    $this->assertStringStartsWith($stacktrace, Shortner::toString());
  }

  public function testTrimmedTrace(): void {

    $stacktrace =
      'class=Zynga\Framework\Environment\StackTrace\V1\Shortner::function=toString::line=23|class=Zynga\Framework\Environment\StackTrace\V1\ShortnerTest::function=testTrimmedTrace';

    $this->assertStringStartsWith($stacktrace, Shortner::toString(12));
  }

}
