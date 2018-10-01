<?hh // strict

namespace Zynga\Framework\Logging\V1\Config\GlobalConfig;

use Zynga\Framework\Logging\V1\Adapter\ErrorLog;
use Zynga\Framework\Logging\V1\Config\Base as ConfigBase;
use Zynga\Framework\Logging\V1\Level;

class Production extends ConfigBase {

  public function init(): bool {
    $errorLog = new ErrorLog();
    $this->addAdapter($errorLog);
    $this->unsetLogLevel(Level::DEBUG);
    $this->unsetLogLevel(Level::INFO);
    $this->setLogLevel(Level::WARNING);
    $this->setLogLevel(Level::ERROR);
    $this->setLogLevel(Level::CRITICAL);
    return true;
  }

  public function getDriver(): string {
    return 'Tee';
  }

}
