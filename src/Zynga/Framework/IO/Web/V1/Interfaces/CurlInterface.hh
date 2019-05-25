<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Interfaces;

use Zynga\Framework\IO\Web\V1\Interfaces\CurlResponsePayloadInterface;

interface CurlInterface {
  public function setOptionsArray(array<int, mixed> $options): bool;
  public function execute(): CurlResponsePayloadInterface;
  public function getInfo(int $optName): mixed;
  public function close(): bool;
}
