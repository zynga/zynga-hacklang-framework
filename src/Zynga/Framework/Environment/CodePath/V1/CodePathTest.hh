<?hh //strict

namespace Zynga\Framework\Environment\CodePath\V1;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class CodePathTest extends TestCase {
  private string $_currentRoot = '';

  public function captureCurrentRoot(): void {
    $this->_currentRoot = CodePath::getRoot();
  }

  public function restoreCurrentRoot(): void {
    CodePath::setRoot($this->_currentRoot);
  }

  /**
   * Are we able to set a new root and return it
   */
  public function testChangeRoot(): void {

    $this->captureCurrentRoot();

    $expected = '/monkey';

    CodePath::setRoot($expected);
    $value = CodePath::getRoot();

    $this->assertEquals($expected, $value);

    $this->restoreCurrentRoot();

  }
}
