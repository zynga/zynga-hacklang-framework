<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\FloatBox;

class ValidSingleRequired extends StorableObject {
  const string EXAMPLE_STRING_KEY = "example_string";
  const string EXAMPLE_UINT64_KEY = "example_uint64";
  const string EXAMPLE_FLOAT_KEY = "example_float";

  public StringBox $example_string;
  public UInt64Box $example_uint64;
  public FloatBox $example_float;

  public function __construct() {

    parent::__construct();

    $this->example_string = new StringBox();
    $this->example_uint64 = new UInt64Box();
    $this->example_float = new FloatBox();

    // We are intending to mimic a primary key field for this object please do not add additional required fields here.
    $this->example_uint64->setIsRequired(true);

  }

}
