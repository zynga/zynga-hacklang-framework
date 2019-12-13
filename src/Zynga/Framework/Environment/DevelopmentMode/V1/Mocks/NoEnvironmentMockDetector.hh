<?hh // strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1\Mocks\

use
  Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces\DetectorInterface
;

class NoEnvironmentMockDetector implements DetectorInterface {

  public function isDevelopment(
    string $isDevelopmentFile = '/opt/zynga/is_development',
  ): bool {
    return false;
  }

  public function isStaging(
    string $isStagingFile = '/opt/zynga/is_staging',
  ): bool {
    return false;
  }

  public function isProduction(
    string $isProductionFile = '/opt/zynga/is_production',
  ): bool {
    return false;
  }

}
