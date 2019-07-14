<?hh //strict

namespace Zynga\Framework\Environment\Command\Finder\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Environment\Command\Finder\V1\Finder as CommandFinder;

class FinderTest extends TestCase {

  public function testGetPaths(): void {
    $paths = CommandFinder::getPaths();
    $this->assertTrue(in_array('/usr/local/bin', $paths));
    $this->assertTrue(in_array('/usr/bin', $paths));
    $this->assertTrue(in_array('/bin', $paths));
  }

  public function testFind(): void {

    // ls should be available on all unix hosts.
    $lsPath = CommandFinder::get('ls');
    $this->assertTrue(is_executable($lsPath));

    $lsPath = CommandFinder::find('ls');
    $this->assertTrue(is_executable($lsPath));

  }

  <<
  expectedException(
    "Zynga\Framework\Environment\Command\Finder\V1\Exceptions\CommandNotFoundException",
  )
  >>
  public function testNotFound(): void {
    CommandFinder::get('not-a-real-command');
  }

}
