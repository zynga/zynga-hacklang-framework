<?hh //strict

namespace Zynga\Framework\Environment\CodePath\V1;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class CodePathTest extends TestCase {
  private string $currentRoot = '';
  private string $currentExternalLibraryRoot = '';

  public function setUp(): void {
    parent::setUp();
    $this->currentRoot = CodePath::getRoot();
    $this->currentExternalLibraryRoot = CodePath::getExternalLibraryRoot();
  }

  public function tearDown(): void {
    CodePath::setRoot($this->currentRoot);
    CodePath::setExternalLibraryRoot($this->currentExternalLibraryRoot);
    parent::tearDown();
  }

  public function testRootPathIsSet(): void {
    $expected = '/monkey';

    CodePath::setRoot($expected);
    $value = CodePath::getRoot();

    $this->assertEquals($expected, $value);
  }

  public function testExternalLibraryPathIsSet(): void {
    $expected = '/its-ya-boy-lib';

    CodePath::setExternalLibraryRoot($expected);
    $value = CodePath::getExternalLibraryRoot();

    $this->assertEquals($expected, $value);
  }
}
