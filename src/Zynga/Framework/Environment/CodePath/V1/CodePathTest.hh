<?hh //strict

namespace Zynga\Framework\Environment\CodePath\V1;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class CodePathTest extends TestCase {
  private string $currentRoot = '';
  private string $currentExternalLibraryPath = '';

  public function setUp(): void {
    parent::setUp();
    $this->currentRoot = CodePath::getRoot();
    $this->currentExternalLibraryPath = CodePath::getExternalLibraryPath();
  }

  public function tearDown(): void {
    parent::tearDown();
    CodePath::setRoot($this->currentRoot);
    CodePath::setExternalLibraryPath($this->currentExternalLibraryPath);
  }

  public function testRootPathIsSet(): void {
    $expected = '/monkey';

    CodePath::setRoot($expected);
    $value = CodePath::getRoot();

    $this->assertEquals($expected, $value);
  }

  public function testExternalLibraryPathIsSet(): void {
    $expected = '/its-ya-boy-lib';

    CodePath::setExternalLibraryPath($expected);
    $value = CodePath::getExternalLibraryPath();

    $this->assertEquals($expected, $value);
  }
}
