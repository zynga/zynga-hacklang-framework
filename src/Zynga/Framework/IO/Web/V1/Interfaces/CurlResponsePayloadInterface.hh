<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Interfaces;

interface CurlResponsePayloadInterface {
  public function getSuccess(): bool;
  public function getPayload(): array<string, mixed>;
}
