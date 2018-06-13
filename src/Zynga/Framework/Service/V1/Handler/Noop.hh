<?hh // strict

namespace Zynga\Framework\Service\V1\Handler;

use Zynga\Framework\Service\V1\Handler\Base as BaseHandler;

class Noop extends BaseHandler {

  public function __construct() {
    parent::__construct();
  }

  <<__Override>>
  public function parseRequest(): bool {
    return true;
  }

  <<__Override>>
  public function handleResponse(): bool {
    return true;
  }

}
