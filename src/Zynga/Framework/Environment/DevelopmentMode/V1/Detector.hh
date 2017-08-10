<?hh // strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1;

use Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces\DetectorInterface;

// There are php equivalents for all these functions in poker-server/shared/casino-platform/include/class/zyCasino/App.php
class Detector implements DetectorInterface {

  public function isDevelopment(
    string $isDevelopmentFile = '/opt/zynga/is_development',
  ): bool {
    if (is_file($isDevelopmentFile)) {
      return true;
    }
    return false;
  }

  public function isStaging(
    string $isStagingFile = '/opt/zynga/is_staging',
  ): bool {
    if (is_file($isStagingFile)) {
      return true;
    }
    return false;
  }

  public function isProduction(
    string $isProductionFile = '/opt/zynga/is_production',
  ): bool {
    if (is_file($isProductionFile)) {
      return true;
    }
    return false;
  }

}
