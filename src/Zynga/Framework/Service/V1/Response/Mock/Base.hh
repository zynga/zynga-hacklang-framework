<?hh // strict

namespace Zynga\Framework\Service\V1\Response\Mock;

use Zynga\Framework\Service\V1\Response\Base as ResponseBase;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\StringBox;

class Base extends ResponseBase {

  private HttpResponseCodeBox $responseCode;

  public function __construct(bool $success, int $responseCode) {
    parent::__construct();
    $this->success = new BoolBox();
    $this->success->set($success);
    $this->responseCode = new HttpResponseCodeBox();
    $this->responseCode->set($responseCode);
  }

  public function success(): BoolBox {
    return $this->success;
  }
}
