<?hh // strict 

namespace Zynga\Framework\Service\V2\Service\Group\Config;

use Zynga\Framework\Service\V2\Interfaces\Exposure\Filter;
use Zynga\Framework\Type\V1\StringBox;

class Pattern {
  public StringBox $codePath;
  public StringBox $filePath;
  public Vector<classname<Filter>> $mustImplementFilter;

  public function __construct() {
    $this->codePath = new StringBox();
    $this->filePath = new StringBox();
    $this->mustImplementFilter = Vector {};
  }

}
