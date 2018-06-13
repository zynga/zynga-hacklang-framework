<?hh // strict 

namespace Zynga\Service\V1\Service\Group\Config;

use Zynga\Framework\Type\V1\StringBox;

abstract class Config {

  public StringBox $title;
  public StringBox $description;
  public Vector<Pattern> $patterns;

  public function __construct() {
    $this->title        = new StringBox();
    $this->description  = new StringBox();
    $this->patterns     = Vector {};
  }

}
