<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Enum\IPVersion;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;
use Zynga\Framework\Type\V1\StringBox;

/**
 * Encapsulation of the concept of an IP Address
 */
class IPBox extends StringBox {

  private ?IPVersion $ipVersion;

  public function isIPV4(): bool {
    return $this->ipVersion === IPVersion::V4;
  }

  public function isIPV6(): bool {
    return $this->ipVersion === IPVersion::V6;
  }

  <<__Override>>
  protected function importFromString(string $value): bool {
    if ($this->isValidIpV4Address($value)) {
      $this->ipVersion = IPVersion::V4;
    } else if ($this->isValidIpV6Address($value)) {
      $this->ipVersion = IPVersion::V6;
    } else {
      throw new FailedToImportFromStringException();
    }

    return parent::importFromString($value);
  }

  private function isValidIpV4Address(string $value): bool {
    return $this->isValidIpAddress($value, FILTER_FLAG_IPV4);
  }

  private function isValidIpV6Address(string $value): bool {
    return $this->isValidIpAddress($value, FILTER_FLAG_IPV6);
  }

  private function isValidIpAddress(string $value, int $ipFilterFlag): bool {
    return filter_var($value, FILTER_VALIDATE_IP, $ipFilterFlag) !== false;
  }

}
