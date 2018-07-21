<?hh // strict

namespace Zynga\Framework\Service\V2\Swagger;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;
use Zynga\Framework\Type\V1\StringBox;
use \ReflectionClass;
use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

class ServiceFile {
  public StringBox $shortClassName;
  public StringBox $filePath;
  public StringBox $codePath;
  public StringBox $serviceUri;
  public StringBox $tagName;

  public function __construct() {
    $this->shortClassName = new StringBox();
    $this->filePath = new StringBox();
    $this->codePath = new StringBox();
    $this->serviceUri = new StringBox();
    $this->tagName = new StringBox();
  }

  public function doesImplementClass(string $neededClass): bool {

    // --
    // Do some reflection magic to see if this class is indeed a service implementation.
    // --
    $className = $this->getClassName();

    if (class_exists($className) === true) {
      // Yay we have a class to work with.
      $serviceReflection = ReflectionClasses::getReflection($className);

      if (!$serviceReflection instanceof ReflectionClass) {
        throw new Exception('testUnableToBeReflected name='.$className);
      }

      $implementedInterfaces = $serviceReflection->getInterfaceNames();

      if (in_array($neededClass, $implementedInterfaces) === true) {
        return true;
      }

    }

    return false;
  }

  public function doesContainService(): bool {
    return $this->doesImplementClass(ServiceInterface::class);
  }

  public function getClassName(): string {
    return
      sprintf('%s\%s', $this->codePath->get(), $this->shortClassName->get());
  }

  public function setFilePath(string $classPathBase, string $filePath): bool {
    $this->filePath->set($filePath);
    $this->convertFileNameToShortClassName($classPathBase);
    $this->convertShortClassNameToUri();
    $this->convertTagNameFromShortClassName();
    return true;
  }

  public function setCodePath(string $codePathBase): bool {
    $this->codePath->set($codePathBase);
    return true;
  }

  public function convertShortClassNameToUri(): bool {
    $uri = '/';
    $uri .= str_replace('\\', '/', $this->shortClassName->get());
    $uri .= '/';
    return $this->serviceUri->set($uri);
  }

  public function convertTagNameFromShortClassName(): bool {
    $tagName = '';

    $pregs = array();

    if (preg_match('/^(.*)\\\\V\d*/', $this->shortClassName->get(), $pregs)) {
      $tagName = str_replace('\\', '_', $pregs[1]);
    }

    return $this->tagName->set($tagName);
  }

  public function convertFileNameToShortClassName(
    string $classPathBase,
  ): bool {
    $className = str_replace($classPathBase, '', $this->filePath->get());
    $className = ltrim($className, '/');
    $className = str_replace('/', '\\', $className);
    $className = rtrim($className, '.hh');
    return $this->shortClassName->set($className);
  }

}
