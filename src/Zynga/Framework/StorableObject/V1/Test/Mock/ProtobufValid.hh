<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

// --
// varints: int32, int64, uint32, uint64, sint32, sint64, bool, enum
// 64-bit: fixed64, sfixed64, double
// length-deliminated: string, bytes, embeded messages, packed and repeated fields
// start group: deprecated
// end group: deprecated
// 32-bit: fixed32, sfixed32, float
// --

use Zynga\Framework\Type\V1\Int32Box;
use Zynga\Framework\Type\V1\Int64Box;
use Zynga\Framework\Type\V1\UInt32Box;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\SInt32Box;
use Zynga\Framework\Type\V1\SInt64Box;
use Zynga\Framework\Type\V1\BoolBox;
// TODO: enum

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Type\V1\FloatBox;

class ProtobufValid extends StorableObject {

  public Int32Box $example_int32;
  public Int64Box $example_int64;
  public UInt32Box $example_uint32;
  public UInt64Box $example_uint64;
  public SInt32Box $example_sint32;
  public SInt64Box $example_sint64;
  public BoolBox $example_bool;
  // TODO: enum

  //public StringBox $example_string;

  //public FloatBox $example_float;

  public function __construct() {

    parent::__construct();

    // --
    // Stand up each of the varint examples:
    // --
    $this->example_int32 = new Int32Box();
    $this->example_int64 = new Int64Box();
    $this->example_uint32 = new UInt32Box();
    $this->example_uint64 = new UInt64Box();
    $this->example_sint32 = new SInt32Box();
    $this->example_sint64 = new SInt64Box();
    $this->example_bool = new BoolBox();
    // TODO: enum

    //$this->example_string = new StringBox();

    //$this->example_float = new FloatBox();

  }

}
