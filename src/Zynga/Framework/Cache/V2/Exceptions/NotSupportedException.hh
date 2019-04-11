<?hh //strict

namespace Zynga\Framework\Cache\V2\Exceptions;

use Zynga\Framework\Exception\V1\Exception;

class NotSupportedException extends Exception {
  public function __construct(string $method, string $msg) {
    parent::__construct();
    $this->message = 'method='.__METHOD__.' msg='.$msg;
  }
}
