<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger\SwaggerType;

use \ReflectionClass;
use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Swagger\V2\Exceptions\UnknownZyngaTypeException;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\Int32Box;
use Zynga\Framework\Type\V1\Int64Box;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt16Box;
use Zynga\Framework\Type\V1\UInt32Box;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\UInt8Box;
use Zynga\Framework\Service\V2\Interfaces\ResponseDataInterface;

class Base extends StorableObjectBase {

  const string BOOLEAN_TYPE = 'boolean';
  const string INTEGER_TYPE = 'integer';
  const string FLOAT_TYPE = 'number';
  const string NUMBER_TYPE = 'number';
  const string STRING_TYPE = 'string';
  const string ARRAY_TYPE = 'array';
  const string INT32_FORMAT = 'int32';
  const string INT64_FORMAT = 'int64';
  const string DOUBLE_FORMAT = 'double';
  const string FLOAT_FORMAT = 'float';

  public StringBox $type;
  public StringBox $format;
  public int $minimum;
  public int $maximum;

  public function __construct() {
    parent::__construct();

    $this->type = new StringBox();
    $this->format = new StringBox();
    $this->minimum = 0;
    $this->maximum = 0;
  }

  public function convertZyngaTypeToSwaggerType(string $type): bool {
    if ($this->convertZyngaIntegerTypeToSwaggerType($type) === true) {
      return true;
    }

    if ($this->convertZyngaBoolTypeToSwaggerType($type) === true) {
      return true;
    }

    if ($this->convertZyngaStringTypeToSwaggerType($type) === true) {
      return true;
    }

    if ($this->convertZyngaFloatTypesToSwaggerType($type) === true) {
      return true;
    }

    if ($this->convertZyngaArrayTypeToSwaggerType($type) === true) {
      return true;
    }

    if ($this->classExtends($type, ResponseDataInterface::class) === true) {
      return true;
    }

    throw new UnknownZyngaTypeException('type='.$type);
  }

  public function convertZyngaFloatTypesToSwaggerType(string $type): bool {
    if ($this->classKnownToExtendBox($type, FloatBox::class)) {
      $this->type->set(self::FLOAT_TYPE);
      $this->format->set(self::FLOAT_FORMAT);
      return true;
    }

    return false;
  }

  public function convertZyngaIntegerTypeToSwaggerType(string $type): bool {
    if ($this->classKnownToExtendBox($type, UInt64Box::class)) {
      $this->type->set(self::NUMBER_TYPE);
      $this->format->set(self::DOUBLE_FORMAT);
      $this->minimum = 0;
      $this->maximum = PHP_INT_MAX;
      return true;
    }

    if ($this->classKnownToExtendBox($type, Int64Box::class)) {
      $this->type->set(self::INTEGER_TYPE);
      $this->format->set(self::INT64_FORMAT);
      return true;
    }

    if ($this->classKnownToExtendBox($type, UInt32Box::class)) {
      $this->type->set(self::NUMBER_TYPE);
      $this->format->set(self::DOUBLE_FORMAT);
      $this->minimum = 0;
      $this->maximum = 4294967295;
      return true;
    }

    if ($this->classKnownToExtendBox($type, Int32Box::class)) {
      $this->type->set(self::INTEGER_TYPE);
      $this->format->set(self::INT64_FORMAT);
      $this->minimum = -2147483648;
      $this->maximum = 2147483647;
      return true;
    }

    if ($this->classKnownToExtendBox($type, UInt16Box::class)) {
      $this->type->set(self::NUMBER_TYPE);
      $this->format->set(self::DOUBLE_FORMAT);
      $this->minimum = 0;
      $this->maximum = 65535;
      return true;
    }

    if ($this->classKnownToExtendBox($type, UInt8Box::class)) {
      $this->type->set(self::NUMBER_TYPE);
      $this->format->set(self::DOUBLE_FORMAT);
      $this->minimum = 0;
      $this->maximum = 255;
      return true;
    }

    return false;
  }

  public function convertZyngaBoolTypeToSwaggerType(string $type): bool {
    if ($this->classKnownToExtendBox($type, BoolBox::class)) {
      $this->type->set(self::BOOLEAN_TYPE);
      return true;
    }

    return false;
  }

  public function convertZyngaStringTypeToSwaggerType(string $type): bool {
    if ($this->classKnownToExtendBox($type, StringBox::class)) {
      $this->type->set(self::STRING_TYPE);
      return true;
    }

    return false;
  }

  public function convertZyngaArrayTypeToSwaggerType(string $type): bool {
    if (preg_match('/StorableVector/', $type)) {
      $this->type->set(self::ARRAY_TYPE);
      return true;
    }

    if (preg_match('/Collections.Vector.V\d+.Base</', $type)) {
      $this->type->set(self::ARRAY_TYPE);
      return true;
    }

    if (preg_match('/Collections.Map.V\d+.Base</', $type)) {
      $this->type->set(self::ARRAY_TYPE);
      return true;
    }

    if (preg_match('/StorableMap/', $type)) {
      $this->type->set(self::ARRAY_TYPE);
      return true;
    }

    return false;
  }

  private function classExtends(string $class, string $extends): bool {
    return is_subclass_of($class, $extends) || $class === $extends;
  }

  private function classKnownToExtendBox(string $class, string $box): bool {
    return $this->classExtends($class, $box);
  }
}
