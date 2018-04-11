<?hh // strict

namespace Zynga\Framework\Type\V1\FileType;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Type\V1\FileType\BoxFactory;
use Zynga\Framework\Type\V1\FileType\Box as FileTypeBox;
use Zynga\Framework\Type\V1\FileType\Exception\UnknownFileTypeException;

class BoxTest extends ZyngaTestCase {

  public function testInvalidSetThrowsUnknownFileTypeException(): void {
    $fileType = new FileTypeBox();
    $this->expectException(UnknownFileTypeException::class);
    $fileType->set('pumpernickel');
  }

}
