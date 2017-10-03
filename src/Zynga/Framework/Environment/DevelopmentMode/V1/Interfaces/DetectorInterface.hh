<?hh // strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces;

interface DetectorInterface {
  public function isDevelopment(): bool;
  public function isStaging(): bool;
  public function isProduction(): bool;
}
