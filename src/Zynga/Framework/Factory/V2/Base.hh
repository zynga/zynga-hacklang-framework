<?hh // strict

namespace Zynga\Framework\Factory\V2;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Factory\V2\Template as FactoryTemplate;
use Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverException;
use Zynga\Framework\Factory\V2\Interfaces\FactoryInterface;
use Zynga\Framework\Factory\V2\Interfaces\FactoryTemplateInterface;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;

/**
 * Base level factory class for the generic factory framework we have in
 * hacklang.
 *
 * docs/FactoryFramework.md covers the topic area.
 *
 */
abstract class Base implements FactoryInterface {

  private static Map<string, FactoryTemplateInterface> $_template = Map {};
  private static Map<string, string> $_mockOverrides = Map {};

  abstract public static function getClassRoot(): string;

  public static function addClassRoot(string $root): bool {

    $template = self::getFactoryTemplate();

    return $template->addClassRoot($root);

  }

  public static function clearFactoryTemplates(): bool {
    self::$_template->clear();
    return true;
  }

  public static function setFactoryTemplate(
    string $classRoot,
    FactoryTemplateInterface $fti,
  ): bool {

    $templateMap = self::$_template;

    $templateMap->set($classRoot, $fti);

    return true;

  }

  public static function getFactoryTemplate(): FactoryTemplateInterface {

    $classRoot = static::getClassRoot();

    $templateMap = self::$_template;

    // if we have a cached copy pass that upwards
    if ($templateMap->containsKey($classRoot)) {
      return $templateMap[$classRoot];
    }

    // if not cached create a factory for this context
    $template = new FactoryTemplate(static::getClassRoot());

    self::setFactoryTemplate($classRoot, $template);

    return $templateMap[$classRoot];

  }

  public static function factory<TDriver as DriverInterface>(
    classname<TDriver> $driverName,
    string $name,
  ): TDriver {

    try {

      $template = self::getFactoryTemplate();

      if ($template->getUseMockDrivers() === true) {

        // --
        // First we run the driver as is to prevent people from using
        // non-existant configs and invalid configs. Then we discard the driver
        // and override it with the mock one.
        // --
        $driver = $template->factory($name);

        if (self::$_mockOverrides->containsKey($name)) {
          $name = self::$_mockOverrides[$name];
        } else if (strpos($name, 'Mock\\') !== false ||
                   strpos($name, 'Mock_') !== false) {
          // No-op since Factory is pointing to a Mock Namespace
        } else {
          $name = 'Mock';
        }
      }

      $driver = $template->factory($name);

      // --
      // JEO: And a word from our sponsor, so what we are doing here is refining
      // the generic type that is stored within the template to the actual
      // driver that user is wanting to have.
      // --
      /* HH_FIXME[4162]: Instanceof on a generic classname is now an error.
       * Consider using different logic to avoid use of classname<Trepo>.
       */
      if ($driver instanceof $driverName) {
        return $driver;
      }

      $classImplements = '';

      if (is_object($driver)) {

        $raw = class_implements($driver);
        if ($raw !== false && is_array($raw)) {
          $classImplements = implode(', ', $raw);
        }
      }

      throw new FailedToLoadDriverException(
        'Failed to load driver for name='.
        $name.
        ' classType='.
        get_class($driver).
        ' classImplements='.
        $classImplements.
        ' lookingFor='.
        $driverName,
      );

    } catch (Exception $e) {
      throw $e;
    }

  }

  public static function clear(): bool {

    $template = self::getFactoryTemplate();

    // --
    // While mocking is on you cannot clear the driver set. It interferes with
    // people loading data sets into their mock drivers.
    // --
    if (self::getUseMockDrivers() === true) {
      return true;
    }

    return $template->clear();

  }

  public static function getDriverCount(): int {

    $template = self::getFactoryTemplate();

    return $template->getDriverCount();

  }

  public static function getUseMockDrivers(): bool {
    $template = self::getFactoryTemplate();
    return $template->getUseMockDrivers();
  }

  public static function enableMockDrivers(): bool {
    $template = self::getFactoryTemplate();
    return $template->enableMockDrivers();
  }

  public static function disableMockDrivers(): bool {
    $template = self::getFactoryTemplate();
    self::clearOverridenMockDrivers();
    return $template->disableMockDrivers();
  }

  public static function overrideMockDriver(
    string $actualClass,
    string $mockClass,
  ): void {
    self::enableMockDrivers();
    self::$_mockOverrides[$actualClass] = $mockClass;
  }

  public static function clearOverridenMockDrivers(): void {
    self::$_mockOverrides = Map {};
  }
}
