<?hh // strict

namespace Zynga\Framework\Service\V2\Test\Handler;

use Zynga\Framework\Service\V2\Handler\Base as HandlerBase;

class MockEmptyHandler extends HandlerBase {
  public function parseRequest(): bool {
    return true;
  }

  public function handleResponse(): bool {
    return true;
  }
}
