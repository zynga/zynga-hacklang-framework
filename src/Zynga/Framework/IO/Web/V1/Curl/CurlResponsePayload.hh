<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;

use Zynga\Framework\IO\Web\V1\Interfaces\CurlResponsePayloadInterface;

class CurlResponsePayload implements CurlResponsePayloadInterface {
  private bool $_success;
  private array<string, mixed> $_payload;

  public function __construct(bool $success, array<string, mixed> $payload) {
    $this->_success = $success;
    $this->_payload = $payload;
  }

  public function getSuccess(): bool {
    return $this->_success;
  }

  public function getPayload(): array<string, mixed> {
    return $this->_payload;
  }

}
