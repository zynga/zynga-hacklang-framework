<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class UrlBox extends StringBox {

  public function replaceInUrl(string $find, string $replace): bool {
    $result = str_replace($find, $replace, $this->_value);

    try {
      $this->set($result);
    } catch (FailedToImportFromStringException $exception) {
      return false;
    }

    return true;
  }

  <<__Override>>
  protected function importFromBool(bool $value): bool {
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
    if ($this->isValidUrl($value)) {
      return parent::importFromString($value);
    }

    throw new FailedToImportFromStringException('unsupported url='.$value);
  }

  private function isValidUrl(string $url): bool {
    // Weed out any non uri formatted urls.
    if (strpos($url, '://') === false) {
      return false;
    }

    // --
    // The hack version of parse_url actually works better than the regex
    // version we ran before.
    // --
    $data = parse_url($url);

    if ($data !== null &&
        is_array($data) &&
        array_key_exists('scheme', $data) &&
        array_key_exists('host', $data)) {
      return true;
    }

    return false;
  }

}

