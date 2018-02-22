<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\UrlBox;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UrlBoxTest extends TestCase {
  public function setUp(): void {
    parent::setUp();
  }

  public function getTypeObject(): TypeInterface {
    return new UrlBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};

    $cnetUrl = 'http://www.zynga.com';
    $values->add(new ValidValue($cnetUrl, $cnetUrl));

    $pokerUrl = 'https://poker.zynga.com';
    $values->add(new ValidValue($pokerUrl, $pokerUrl));

    $pokerWithPortUrl = 'https://poker.zynga.com:8874';
    $values->add(new ValidValue($pokerWithPortUrl, $pokerWithPortUrl));

    $profilePicUrl =
      'https://scontent.xx.fbcdn.net/v/t1.0-1/s200x200/12074947_10152975537761923_4996569292942257821_n.jpg?oh=b5459da74187f52fd9068bc0b503d7c2&oe=59D6E9E0';
    $values->add(new ValidValue($profilePicUrl, $profilePicUrl));

    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    $values[] = 'javascript:';
    $values[] = '0';
    $values[] = '1';
    $values[] = 'true';
    $values[] = 'false';
    $values[] = '';
    $values[] = true;
    $values[] = false;
    $values[] = 0.0;
    $values[] = 1.0;
    $values[] = 0;
    $values[] = 1;
    return $values;
  }

  public function testReplaceInUrlReturnsFalseWhenInvalid(): void {
    $url = 'https://www.zynga.com';
    $urlBox = new UrlBox();
    $urlBox->set($url);
    $this->assertFalse($urlBox->replaceInUrl('https', '????????'));
  }

  public function testReplaceInUrlReturnsTrueWhenValid(): void {
    $url = 'https://www.zynga.com';
    $urlBox = new UrlBox();
    $urlBox->set($url);
    $this->assertTrue($urlBox->replaceInUrl('com', 'org'));
  }
}
