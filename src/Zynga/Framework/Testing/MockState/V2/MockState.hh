<?hh // strict

namespace Zynga\Framework\Testing\MockState\V2;

use Zynga\Framework\Testing\MockState\V2\MockableClass;


/**
 * Class to allow developers to have one spot to turn on / off the mocking
 * environment for the whole platform.
 */
class MockState {

  private static Vector<string> $_mockableClassConfig = Vector {
    'Zynga\Framework\Database\V2\Factory',
    'Zynga\Framework\Fluentd\V1\Factory',
    'Zynga\Framework\Log\Stat\V1\Factory',
    'Zynga\Framework\Datadog\V2\Factory',
    'Zynga\Framework\Queue\V2\Factory',
    'Zynga\Framework\Cache\V2\Factory',
    'Zynga\Framework\Database\V2\UserSharded\Factory',
    'Zynga\Legacy\V1\SexyShards',
    'Zynga\Legacy\V1\UnifiedSexyShards'
  };

  public static Vector<MockableClass> $_mockables = Vector {};

  public static function addMockableClass(string $className): bool {
    self::$_mockableClassConfig->add($className);
    return true;
  }

  public static function clearMockables(): bool {
    self::$_mockables->clear();
    return true;
  }

  public static function enableMocks(): bool {

    self::detectMockableClasses();

    foreach ( self::$_mockables as $mockable ) {
      $mockable->enable();
    }

    return true;
  }

  public static function disableMocks(): bool {

    self::detectMockableClasses();

    foreach ( self::$_mockables as $mockable ) {
      $mockable->disable();
    }

    return true;

  }

  public static function getMockableCount(): int {
    return self::$_mockables->count();
  }

  public static function detectMockableClasses(): bool {

    if ( self::getMockableCount() > 0 ) {
      return true;
    }

    foreach ( self::$_mockableClassConfig as $mockableClass ) {

      $mockCandidate = new MockableClass($mockableClass);

      if ( $mockCandidate->detect() === true ) {
        self::$_mockables->add($mockCandidate);
      }

    }

    return true;

  }

}
