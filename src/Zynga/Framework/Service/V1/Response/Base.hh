<?hh // strict

namespace Zynga\Framework\Service\V1\Response;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use
  Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector
;

use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;
use Zynga\Framework\Service\V1\Interfaces\ResponseDataInterface;

use Zynga\Environment\V1\SuperGlobals;
use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\StringBox;

abstract class Base extends StorableObjectBase implements ResponseInterface {
  public BoolBox $success;
  public HttpResponseCodeBox $code;
  public StorableVector<StringBox> $message;
  public ResponseDataInterface $responseData;

  public function __construct() {
    parent::__construct();

    $this->success = new BoolBox();
    $this->success->setIsRequired(true);

    $this->code = new HttpResponseCodeBox();
    $this->code->setIsRequired(true);

    $this->message = new StorableVector(StringBox::class);

    $this->responseData = new EmptyResponseData();
  }

  public function success(): BoolBox {
    return $this->success;
  }

  public function code(): HttpResponseCodeBox {
    return $this->code;
  }

  public function message(): StorableVector<StringBox> {
    return $this->message;
  }

  public function responseData(): ResponseDataInterface {
    return $this->responseData;
  }

  public function setResponseData(ResponseDataInterface $responseData): void {
    $this->responseData = $responseData;
  }

  public function addMessage(string $message): bool {
    $val = new StringBox();
    $val->set($message);
    $this->message->add($val);
    return true;
  }
}
