<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces;

use Zynga\Framework\Exception\V1\Exception;

interface HandlerInterface {

  public function setService(ServiceInterface $service): bool;

  public function getService(): ?ServiceInterface;

  public function parseRequest(): bool;

  public function handleGenericFailure(): bool;

  public function handleGenericSuccess(): bool;

  public function createShortExceptionName(Exception $e): string;

  public function handleFailureException(Exception $e): bool;

  public function handleResponse(): bool;

}
