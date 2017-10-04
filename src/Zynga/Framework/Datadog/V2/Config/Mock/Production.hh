<?hh // strict

namespace Zynga\Framework\Datadog\V2\Config\Mock;

use Zynga\Framework\Datadog\V2\Config\Mock\Base;

/**
 * Dev implementation of the mock datadog config
 **/
class Production extends Base {
  public function init(): bool {
    return parent::init();
  }
}
