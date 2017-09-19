<?hh // strict

namespace Zynga\Framework\Database\V2\Config;

use Zynga\Framework\Database\V2\Config\Base;

use Zynga\Framework\Database\V2\Exceptions\ClusterInitFailureException;
use Zynga\Framework\Database\V2\Exceptions\ClusterInitNoServersException;

use Zynga\Framework\Exception\V1\Exception;

abstract class ClusterBase extends Base {

  private Vector<string> $_servers = Vector {};

  /**
   * Initializes the configuration, and verifies that all the functions have been called.
   * @return bool
   * @codeCoverageIgnore
   * JEO: Again we find the code coverage analysis doesn't accurately reflect what is executed.
   */
  final public function init(): bool {

    // reset our servers list to empty.
    $this->_servers->clear();

    try {
      if ($this->clusterInit() !== true) {
        throw new ClusterInitFailureException(
          'Implementation returned false',
        );
      }
    } catch (Exception $e) {
      throw $e;
    }

    if ($this->getServerCount() == 0) {
      throw new ClusterInitNoServersException(
        'You failed to add any servers to the list, call addServer',
      );
    }

    // We have to call parent::init last as it verifies that the base level settings are done.
    return parent::init();

  }

  final public function clearServers(): bool {
    $this->_servers->clear();
    return true;
  }

  final public function addServer(string $name): bool {
    $this->_servers->add($name);
    return true;
  }

  final public function getRandomServer(): string {

    $serverCount = $this->getServerCount();

    if ($serverCount === 0) {
      return '';
    }

    if ($serverCount === 1) {
      return $this->_servers[0];
    }

    // strip one off our count, as we're honoring offset 0 based arrays
    $serverCount = $serverCount - 1;

    // init the random generator
    mt_srand();

    // get a offset for us to poke to
    $offset = mt_rand(0, $serverCount);

    // pick the lucky server
    return $this->_servers[$offset];
  }

  final public function getServerCount(): int {
    return $this->_servers->count();
  }

  /**
   * User definable and overloadable hook for initializing your cluster.
   * @return bool
   */
  abstract public function clusterInit(): bool;

}
