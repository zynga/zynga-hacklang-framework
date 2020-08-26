<?hh // strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1\Mock;

use
  Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces\DetectorInterface
;

class ProductionDetector implements DetectorInterface {
  public function isDevelopment(): bool {
    return false;
  }
  public function isStaging(): bool {
    return false;
  }
  public function isProduction(): bool {
    return true;
  }
}