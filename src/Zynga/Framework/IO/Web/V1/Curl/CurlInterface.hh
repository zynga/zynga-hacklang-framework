<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;

interface CurlInterface {
  public function setOptionsArray(array<int, mixed> $options): bool;
  public function execute(): mixed;
  public function getInfo(int $optName): mixed;
  public function close(): bool;
}
