<?hh // strict

namespace Zynga\Framework\Environment\StackTrace\V1;

use Zynga\Framework\StorableObject\V1\Base;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;

class Frame extends Base {
  public UInt64Box $frameId;
  public StringBox $function;
  public UInt64Box $line;
  public StringBox $file;
  public StringBox $class;
  public StringBox $type;

  public function __construct() {

    parent::__construct();

    $this->frameId = new UInt64Box();
    $this->function = new StringBox();
    $this->line = new UInt64Box();
    $this->file = new StringBox();
    $this->class = new StringBox();
    $this->type = new StringBox();

  }

}
