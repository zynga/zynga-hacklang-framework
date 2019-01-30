<?hh //strict

namespace Zynga\Framework\Environment\CodePath\V1;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Environment\CodePath\V1\Exceptions\PathNotFoundException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class CodePathTest extends TestCase {
  private string $currentRoot = '';
  private ?Map<string, string> $currentLibraryPathMap = null;

  public function setUp(): void {
    parent::setUp();
    $this->currentRoot = CodePath::getRoot();
    $this->currentLibraryPathMap = CodePath::getLibraryPathMap();
  }

  public function tearDown(): void {
    parent::tearDown();
    CodePath::setRoot($this->currentRoot);
    CodePath::setLibraryPathMap($this->currentLibraryPathMap);
  }

  public function testChangeRoot(): void {
    $expected = '/monkey';

    CodePath::setRoot($expected);
    $value = CodePath::getRoot();

    $this->assertEquals($expected, $value);
  }

  public function testUnknownLibraryThrowsExceptionWhenNoDataSet(): void {
    CodePath::resetPaths();

    $this->expectException(PathNotFoundException::class);
    CodePath::getLibraryPath('what up it\'s ya boy some lib');
  }

  public function testUknownLibraryThrowsExceptionWhenLibraryNotFound(): void {
    CodePath::resetPaths();

    CodePath::setLibraryPath('aylmao something', 'buzz');

    $this->expectException(PathNotFoundException::class);
    CodePath::getLibraryPath('what up it\'s ya boy some lib');
  }

  public function testSetLibraryPathIsReturned(): void {
    CodePath::resetPaths();

    $library = 'fizz';
    $path = 'buzz';
    CodePath::setLibraryPath($library, $path);

    $this->assertEquals(
      $path,
      CodePath::getLibraryPath($library),
      'Previously set library path should be returned on fetch',
    );
  }

}
