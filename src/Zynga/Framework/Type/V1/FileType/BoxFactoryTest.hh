<?hh // strict

namespace Zynga\Framework\Type\V1\FileType;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Type\V1\FileType\BoxFactory;
use Zynga\Framework\Type\V1\FileType\Enum\FileType as FileTypeEnum;

class BoxFactoryTest extends ZyngaTestCase {

  public function testJsonReturnsValidType(): void {
    $this->assertEquals((string)FileTypeEnum::JSON, BoxFactory::json()->get());
  }

  public function testCsvReturnsValidType(): void {
    $this->assertEquals((string)FileTypeEnum::CSV, BoxFactory::csv()->get());
  }

  public function testIsValidValueReturnsFalseCorrectly(): void {
    $this->assertFalse(BoxFactory::isValidValue('pumpernickel'));
  }

  public function testIsValidValueReturnsTrueCorrectly(): void {
    $this->assertTrue(BoxFactory::isValidValue((string)FileTypeEnum::JSON));
    $this->assertTrue(BoxFactory::isValidValue((string)FileTypeEnum::CSV));
  }

}
