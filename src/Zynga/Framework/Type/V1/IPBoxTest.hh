<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\IPBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class IPBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new IPBox();
  }

  public function testIsStringValid(): void {
    $ipBox = new IPBox();
    $this->assertFalse($ipBox->isStringValid(''));
    $this->assertTrue($ipBox->isStringValid('10.10.0.1'));
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};
    $values->add(new ValidValue('172.16.254.1', '172.16.254.1'));
    $values->add(new ValidValue('172.16.0.0', '172.16.0.0'));
    $values->add(new ValidValue(3341838852, '199.48.106.4'));
    $values->add(
      new ValidValue(
        '2001:0db8:85a3:0000:0000:8a2e:0370:7334',
        '2001:0db8:85a3:0000:0000:8a2e:0370:7334',
      ),
    );
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = 'false';
    $values[] = 'true';
    $values[] = -3.5;
    $values[] = -4.00123;
    $values[] = '';
    return $values;
  }

  public function testIsIPV4ReturnsTrueOnIPV4(): void {
    $ip = new IPBox();
    $ip->set('172.16.254.1');
    $this->assertTrue($ip->isIPV4());
  }

  public function testIsIPV4ReturnsFalseOnIPV6(): void {
    $ip = new IPBox();
    $ip->set('2001:0db8:85a3:0000:0000:8a2e:0370:7334');
    $this->assertFalse($ip->isIPV4());
  }

  public function testIsIPV6ReturnsFalseOnIPV4(): void {
    $ip = new IPBox();
    $ip->set('172.16.254.1');
    $this->assertFalse($ip->isIPV6());
  }

  public function testIsIPV6ReturnsTrueOnIPV6(): void {
    $ip = new IPBox();
    $ip->set('2001:0db8:85a3:0000:0000:8a2e:0370:7334');
    $this->assertTrue($ip->isIPV6());
  }

  public function testIsIPV6ReturnsFalseWhenNotSet(): void {
    $ip = new IPBox();
    $this->assertFalse($ip->isIPV6());
  }

  public function testIsIPV4ReturnsFalseWhenNotSet(): void {
    $ip = new IPBox();
    $this->assertFalse($ip->isIPV4());
  }
}
