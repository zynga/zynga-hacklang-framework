<?hh // strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;

use Zynga\Framework\Swagger\V2\Swagger;

class Response extends Swagger implements ResponseInterface {
  private BoolBox $_success;
  private HttpResponseCodeBox $_code;
  private StorableVector<StringBox> $_message;

  public function __construct() {

    parent::__construct();

    $this->_success = new BoolBox();
    $this->_code = new HttpResponseCodeBox();
    $this->_message = new StorableVector(StringBox::class);

  }

  // --
  // JEO: These are not used for this type of response, so they get noop boxes
  // --
  public function success(): BoolBox {
    return $this->_success;
  }

  public function code(): HttpResponseCodeBox {
    return $this->_code;
  }

  public function message(): StorableVector<StringBox> {
    return $this->_message;
  }

  public function addMessage(string $message): bool {
    return true;
  }

}
