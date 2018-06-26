<?hh // strict

namespace Zynga\Framework\Logging\V1\Config\Noop;

use Zynga\Framework\Logging\V1\Config\Base as ConfigBase;
use Zynga\Framework\Logging\V1\Level;

class Dev extends ConfigBase {

  public function init(): bool {
    $this->setLogLevel(Level::DEBUG);
    $this->setLogLevel(Level::INFO);
    $this->setLogLevel(Level::WARNING);
    $this->setLogLevel(Level::ERROR);
    $this->setLogLevel(Level::CRITICAL);
    return true;
  }

  public function getDriver(): string {
    return 'Noop';
  }

}
