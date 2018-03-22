<?hh //strict

namespace Zynga\Framework\StorableObject\V1\Exporter;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNestedMap;

use Zynga\Framework\StorableObject\V1\Test\Mock\Nofields;
use Zynga\Framework\StorableObject\V1\Test\Mock\InvalidUnsupportedType;

use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestInt32;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestInt64;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestUInt32;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestUInt64;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestSInt32;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestSInt64;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtoTest\ProtoTestBool;

use Zynga\Framework\Environment\CodePath\V1\CodePath;

class ProtobufTest extends TestCase {

  const int MIN_INT32 = -2147483648;
  const int MIN_INT64 = PHP_INT_MIN;
  const int MIN_UINT32 = 0;
  const int MIN_UINT64 = 0;
  const int MIN_SINT32 = -2147483648;
  const int MIN_SINT64 = PHP_INT_MIN;
  const bool MIN_BOOL = false;

  const int MAX_INT32 = 2147483647;
  const int MAX_INT64 = PHP_INT_MAX;
  const int MAX_UINT32 = 4294967295;
  const int MAX_UINT64 = PHP_INT_MAX;
  const int MAX_SINT32 = 2147483647;
  const int MAX_SINT64 = PHP_INT_MAX;
  const bool MAX_BOOL = true;

  public function getTestFilePath(string $shortPath): string {
    $filePath = dirname(__FILE__);
    $filePath = dirname($filePath);     // strip: Exporter
    $filePath = dirname($filePath);     // strip: V1
    $filePath = dirname($filePath);     // strip: StorableObject
    $filePath = dirname($filePath);     // strip: Framework
    $filePath = dirname($filePath);     // strip: Zynga
    $filePath = dirname($filePath);     // strip: src
    $filePath .= '/tests/protobuf-java-compat/'. $shortPath;
    return $filePath;
  }

  public function getMinValuesBinary(string $binFile): string {
    $filePath = $this->getTestFilePath($binFile);
    $this->assertFileExists($filePath);
    return file_get_contents($filePath);
  }

  public function writingTest(
    StorableObjectInterface $obj,
    string $referenceFile,
  ): void {
    $testBin = $obj->export()->asBinary();

    $testHex = bin2hex($testBin);

    $referenceBin = $this->getMinValuesBinary($referenceFile);
    $referenceHex = bin2hex($referenceBin);

    $this->assertEquals($referenceHex, $testHex);
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function testProfobufNoFields(): void {
    $obj = new Nofields();
    $obj->export()->asBinary();
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function testProtobufInvalidUnsupportedType(): void {
    $obj = new InvalidUnsupportedType();
    $obj->export()->asBinary();
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException
   */
  public function testProtobuf_String(): void {
    // JEO: Currently these are unsupported with protobuf encoder.
    $obj = new Valid();
    $obj->export()->asBinary();
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException
   */
  public function testProtobuf_NestedMap(): void {
    // JEO: Currently these are unsupported with our protobuf encoder.
    $obj = new ValidNestedMap();
    $obj->export()->asBinary();
  }

  public function testProtobufWriting_Int32(): void {

    $obj = new ProtoTestInt32();
    $obj->example_int32->set(self::MIN_INT32);
    $this->writingTest($obj, 'min_int32.bin');

    $obj = new ProtoTestInt32();
    $obj->example_int32->set(self::MAX_INT32);
    $this->writingTest($obj, 'max_int32.bin');

  }

  public function testProtobufWriting_Int64(): void {

    $obj = new ProtoTestInt64();
    $obj->example_int64->set(self::MIN_INT64);
    $this->writingTest($obj, 'min_int64.bin');

    $obj = new ProtoTestInt64();
    $obj->example_int64->set(self::MAX_INT64);
    $this->writingTest($obj, 'max_int64.bin');

  }

  public function testProtobufWriting_uint32(): void {

    $obj = new ProtoTestUInt32();
    $obj->example_uint32->set(self::MIN_UINT32);
    $this->writingTest($obj, 'min_uint32.bin');

    $obj = new ProtoTestUInt32();
    $obj->example_uint32->set(self::MAX_UINT32);
    $this->writingTest($obj, 'max_uint32.bin');

  }

  public function testProtobufWriting_uint64(): void {

    $obj = new ProtoTestUInt64();
    $obj->example_uint64->set(self::MIN_UINT64);
    $this->writingTest($obj, 'min_uint64.bin');

    $obj = new ProtoTestUInt64();
    $obj->example_uint64->set(self::MAX_UINT64);
    $this->writingTest($obj, 'max_uint64.bin');

  }

  public function testProtobufWriting_sint32(): void {

    $obj = new ProtoTestSInt32();
    $obj->example_sint32->set(self::MIN_SINT32);
    $this->writingTest($obj, 'min_sint32.bin');

    $obj = new ProtoTestSInt32();
    $obj->example_sint32->set(self::MAX_SINT32);
    $this->writingTest($obj, 'max_sint32.bin');

  }

  public function testProtobufWriting_sint64(): void {

    $obj = new ProtoTestSInt64();
    $obj->example_sint64->set(self::MIN_SINT64);
    $this->writingTest($obj, 'min_sint64.bin');

    $obj = new ProtoTestSInt64();
    $obj->example_sint64->set(self::MAX_SINT64);
    $this->writingTest($obj, 'max_sint64.bin');

  }

  public function testProtobufWriting_bool(): void {

    $obj = new ProtoTestBool();
    $obj->example_bool->set(self::MIN_BOOL);
    $this->writingTest($obj, 'min_bool.bin');

    $obj = new ProtoTestBool();
    $obj->example_bool->set(self::MAX_BOOL);
    $this->writingTest($obj, 'max_bool.bin');

  }

  /*
   public function testProtobufWriting(): void {
   // int32, int64, uint32, uint64, sint32, sint64, bool, enum
   // Set all of the values to their min:
   $minObj = new ProtobufValid();
   $minObj->example_uint32->set(self::MIN_UINT32);
   $minObj->example_uint64->set(self::MIN_UINT64);
   $minObj->example_sint32->set(self::MIN_INT32);
   $minObj->example_sint64->set(self::MIN_INT64);
   $minObj->example_bool->set(false);
   var_dump($minObj->export()->asMap()->toArray());
   $minBin = $minObj->export()->asBinary();
   $minHex = bin2hex($minBin);
   var_dump(bin2hex($minBin));
   $minReferenceBinary = $this->getMinValuesBinary();
   $minReferenceHex = bin2hex($minReferenceBinary);
   var_dump($minReferenceHex);
   $this->assertEquals(
   $minReferenceHex,
   $minHex
   );
   // now read in this binary and attempt to push it into a new object
   //$thawMinObj = new ProtobufValid();
   //$thawMinObj->import()->fromBinary($minBin);
   }
   */

}
