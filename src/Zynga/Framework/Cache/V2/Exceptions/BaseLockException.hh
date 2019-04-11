<?hh //strict

namespace Zynga\Framework\Cache\V2\Exceptions;

use Zynga\Framework\Exception\V1\Exception;

abstract class BaseLockException extends Exception {
  public function __construct(string $method, string $msg, mixed $data) {
    parent::__construct();
    $this->message =
      'method='.__METHOD__.' msg='.$msg.' data='.json_encode($data);
  }
}
