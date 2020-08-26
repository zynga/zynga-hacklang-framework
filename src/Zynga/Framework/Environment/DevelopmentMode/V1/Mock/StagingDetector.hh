<?hh // strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1\Mock;

use
  Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces\DetectorInterface
;

class StagingDetector implements DetectorInterface {
  public function isDevelopment(): bool {
    return false;
  }
  public function isStaging(): bool {
    return true;
  }
  public function isProduction(): bool {
    return false;
  }
}