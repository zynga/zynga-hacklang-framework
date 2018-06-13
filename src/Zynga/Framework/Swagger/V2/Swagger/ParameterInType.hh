<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Swagger\V2\Exceptions\UnSupportedTypeException;

class ParameterInType extends StringBox {
  public function importFromString(string $string): bool {
    $acceptableInValues = 'query|header|path|formData|body';
    if (preg_match('/^('.$acceptableInValues.')$/', $string)) {
      return parent::importFromString($string);
    }
    throw new UnSupportedTypeException(
      'in='.$string.' acceptable='.$acceptableInValues,
    );
  }
}
