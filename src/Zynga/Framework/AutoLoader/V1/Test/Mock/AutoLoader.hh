<?hh // strict

namespace Zynga\Framework\AutoLoader\V1\Test\Mock;

use Zynga\Framework\AutoLoader\V1\Base;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

class AutoLoader extends Base {

  public function getFileSystemRoot(): string {
    // --
    // Find our root
    // --
    $autoloaderRoot = dirname(__FILE__); // This file
    $autoloaderRoot = dirname($autoloaderRoot); // strip: Mock
    $autoloaderRoot = dirname($autoloaderRoot); // strip: Test
    $autoloaderRoot = dirname($autoloaderRoot); // strip: V1
    $autoloaderRoot = dirname($autoloaderRoot); // strip: AutoLoader
    $autoloaderRoot = dirname($autoloaderRoot); // strip: Framework
    $autoloaderRoot = dirname($autoloaderRoot); // strip: Zynga
    $autoloaderRoot = dirname($autoloaderRoot); // strip: src
    $autoloaderRoot .= '/tests/autoloader';
    return $autoloaderRoot;
  }

}
