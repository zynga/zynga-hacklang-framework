<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class UrlBox extends StringBox {

  public function importFromBool(bool $value): bool {
    try {
      return $this->importFromString(strval($value));
    } catch (Exception $e) {
      throw $e;
    }
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    try {
      return $this->importFromString(strval($value));
    } catch (Exception $e) {
      throw $e;
    }
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    try {
      return $this->importFromString(strval($value));
    } catch (Exception $e) {
      throw $e;
    }
  }

  <<__Override>>
  protected function importFromString(string $value): bool {

    try {

      // Weed out any non uri formatted urls.
      if (strpos($value, '://') === false) {
        throw new FailedToImportFromStringException(
          'unsupported url='.$value,
        );
      }

      // --
      // The hack version of parse_url actually works better than the regex
      // version we ran before.
      // --
      $data = parse_url($value);

      if ($data !== null &&
          is_array($data) &&
          array_key_exists('scheme', $data) &&
          array_key_exists('host', $data)) {
        return parent::importFromString($value);
      }

      throw new FailedToImportFromStringException('unsupported url='.$value);

    } catch (Exception $e) {
      throw $e;
    }

  }

}
