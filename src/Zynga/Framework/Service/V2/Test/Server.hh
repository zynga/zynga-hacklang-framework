<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Server\Base as ServerBase;
use Zynga\Framework\Type\V1\StringBox;

class Server extends ServerBase {
  public StringBox $PHP_SELF;

  public function __construct() {
    parent::__construct();
    $this->PHP_SELF = new StringBox();
    $this->PHP_SELF->setIsRequired(true);
  }
}
