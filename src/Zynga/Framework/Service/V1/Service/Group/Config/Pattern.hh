<?hh // strict 

namespace Zynga\Framework\Service\V1\ServiceGroup\Config;

use Zynga\Framework\Type\V1\StringBox;

class Pattern {
  public StringBox $codePath;
  public StringBox $filePath;

  public function __construct() {
    $this->codePath = new StringBox();
    $this->filePath = new StringBox();
  }

}
