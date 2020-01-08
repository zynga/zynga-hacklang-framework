<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Mock;

use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Template as TemplateBase;
use Zynga\Framework\Database\V2\Config\Test\Mysql\Dev;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Base;

class TemplateReturningWrongDriver extends TemplateBase {

  public function factory(string $name): DriverInterface {
    error_log('JEO - attempting to send back bad driver');
    $config = new Dev();
    return new Base($config);
  }

}
