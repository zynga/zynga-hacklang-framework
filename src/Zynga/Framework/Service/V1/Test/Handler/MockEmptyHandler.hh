<?hh // strict

namespace Zynga\Framework\Service\V1\Test\Handler;

use Zynga\Framework\Service\V1\Handler\Base as HandlerBase;

class MockEmptyHandler extends HandlerBase {
  public function parseRequest(): bool {
    return true;
  }

  public function handleResponse(): bool {
    return true;
  }
}
