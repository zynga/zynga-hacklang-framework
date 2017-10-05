<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Exporter;

// --
// Rewritten for hack, inspirational source at:
// https://github.com/datto/Protobuf-PHP/blob/master/library/DrSlump/Protobuf/Codec/Binary/Writer.php
//
// Where we differ, I decided to cleave out some of the static variable usage
// also removed the binary memory buffer wrapper.
//
// JEO: This is temporary until i write one that is pure us.
//
// --
class ProtobufBinary {

  private resource $_buffer;

  private mixed $_gmp_0x00;
  private mixed $_gmp_0x7f;
  private mixed $_gmp_0x80;

  public function __construct() {
    $this->_buffer = fopen('php://memory', 'wb');
    $this->_gmp_0x00 = gmp_init(0x00);
    $this->_gmp_0x7f = gmp_init(0x7f);
    $this->_gmp_0x80 = gmp_init(0x80);
  }

  public function __destroy(): void {
    // Close off the buffer handle.
    // Turns out destructors fuck up coverage.
    // @codeCoverageIgnoreStart
    if (is_resource($this->_buffer)) {
      fclose($this->_buffer);
    }
    // @codeCoverageIgnoreEnd
  }

  public function getBuffer(): string {
    fseek($this->_buffer, 0, SEEK_SET);
    return stream_get_contents($this->_buffer);
  }

  /*
   public function debugMessage(string $message): bool {
   echo '[DEBUG] - ' . $message . "\n";
   return true;
   }
   */

  public function writeStringToBuffer(string $bytes, int $length): bool {

    // --
    // JEO: Unsure that we actually need this.
    // --
    // if no length was provided calculate one.
    // if ( $length === null ) {
    //  $length = strlen($bytes);
    //}

    // $this->debugMessage("bufferContents=" . bin2hex($this->getBuffer()) . " - before write");

    // write the bits down into the memory buffer.
    fseek($this->_buffer, 0, SEEK_END);
    $written = fwrite($this->_buffer, $bytes, $length);

    //$this->debugMessage("bufferContents=" . bin2hex($this->getBuffer()) . " - after write");

    // make sure we are writing the length that we asked to.
    if ($written === $length) {
      return true;
    }

    // @codeCoverageIgnoreStart
    return false;
    // @codeCoverageIgnoreEnd

  }

  public function addByteToBuffer(int $value): bool {
    return $this->writeStringToBuffer(chr($value), 1); // add 1 byte only
  }

  public function addVarint(int $value): bool {

    // Small values do not need to be encoded
    if ($value >= 0 && $value < 0x80) {
      // $this->debugMessage("addByteToBuffer=$value");
      $this->addByteToBuffer($value);
      return true;
    }

    // Build an array of bytes with the encoded values
    $values = Vector {};
    if ($value > 0) {
      while ($value > 0) {
        $values[] = 0x80 | ($value & 0x7f);
        $value = $value >> 7;
      }
    } else {
      $values = $this->createGmpVarint($value);
    }

    // Remove the MSB flag from the last byte
    $values[$values->count() - 1] &= 0x7f;

    // Convert the byte sized ints to actual bytes in a string
    $valueArray = $values->toArray();
    $bytes =
      call_user_func_array('pack', array_merge(array('C*'), $valueArray));
    ;

    $this->writeStringToBuffer($bytes, strlen($bytes));

    return true;

  }

  public function createGmpVarint(int $value): Vector<int> {

    $value =
      PHP_INT_SIZE < 8
        ? gmp_and($value, '0x0ffffffffffffffff')
        : sprintf('%u', $value);

    $values = Vector {};

    while ((int) gmp_cmp($value, $this->_gmp_0x00) > 0) {
      $values[] = gmp_intval(gmp_and($value, $this->_gmp_0x7f)) | 0x80;
      $value = gmp_div_q($value, $this->_gmp_0x80);
    }

    return $values;
  }

  public function addZigzag(int $value, int $base): bool {
    // TODO: base can only be 32|64
    $value = ($value << 1) ^ ($value >> $base - 1);
    return $this->addVarint($value);
  }

}
