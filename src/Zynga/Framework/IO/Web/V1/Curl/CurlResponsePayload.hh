<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;

class CurlResponsePayload {
  public bool $success = false;
  public array<string, mixed> $payload = array();
  
  public function __construct(bool $success, array<string, mixed> $payload) {
    $this->success = $success;
    $this->payload = $payload;
  }
}
