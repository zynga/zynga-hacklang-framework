<?hh // strict

namespace Zynga\Framework\Service\V2\Test\Handler;

use Zynga\Framework\Service\V2\Handler\Http as HandlerBase;
use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;
use Zynga\Framework\Exception\V1\Exception;

class MockHttpBrokenGetServiceHandler extends HandlerBase {
  public function getService(): ServiceInterface {
    throw new Exception('i-am-a-walrus');
  }
}
