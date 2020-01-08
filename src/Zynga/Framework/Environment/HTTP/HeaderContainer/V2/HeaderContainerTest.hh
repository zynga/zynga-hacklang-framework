<?hh //strict

namespace Zynga\Framework\Environment\HTTP\HeaderContainer\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\Environment\HTTP\HeaderContainer\V2\HeaderContainer as HttpHeaderContainer
;
use
  Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Exceptions\UnknownHeaderException
;
use
  Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Test\Mock\HeaderContainer as MockHeaderContainer
;

class HeaderContainerTest extends TestCase {

  public function testMock(): void {
    $testHeader = 'test-header';
    $testValue = 'some-value';

    $obj = new MockHeaderContainer();
    $obj->setDoSend(false);
    $obj->setSendRv(true);

    $this->assertTrue($obj->setHeader($testHeader, $testValue));
    $this->assertEquals($testValue, $obj->getHeader($testHeader));

    $this->assertTrue($obj->noCaching());
    $this->assertTrue($obj->contentIsJSON());
    $this->assertTrue($obj->send());

  }

  public function testValidSimpleHeader(): void {

    $testHeader = 'test-header';
    $testValue = 'some-value';

    $obj = new HttpHeaderContainer();
    $this->assertTrue($obj->setHeader($testHeader, $testValue));
    $this->assertEquals($testValue, $obj->getHeader($testHeader));

  }

  public function testNoCachingHeaders(): void {
    $obj = new HttpHeaderContainer();
    $this->assertTrue($obj->noCaching());
    $this->assertEquals(
      'no-store, no-cache, must-revalidate',
      $obj->getHeader('Cache-Control'),
    );
    $this->assertEquals('no-cache', $obj->getHeader('Pragma'));
    $this->assertEquals(
      'Mon, 26 Jul 1997 05:00:00 GMT',
      $obj->getHeader('Expires'),
    );
  }

  public function testIsJSON(): void {
    $obj = new HttpHeaderContainer();
    $this->assertTrue($obj->contentIsJSON());
    $this->assertEquals('application/json', $obj->getHeader('Content-type'));
  }

  public function testUnknownHeader(): void {
    $obj = new HttpHeaderContainer();
    $this->expectException(UnknownHeaderException::class);
    $obj->getHeader('not-here');
  }

  public function testSendOutputLoop(): void {
    $obj = new MockHeaderContainer();
    $obj->setDoHeadersSent(false);
    $obj->setHeadersSentRv(false);
    $obj->contentIsJSON();
    $this->assertTrue($obj->send());
  }

  public function testHeadersSent(): void {
    $obj = new MockHeaderContainer();
    $this->assertTrue($obj->headersSent());
  }

}
