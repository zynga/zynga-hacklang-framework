<?hh // strict

namespace Zynga\Framework\Service\V2\Authenticated;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V2\Interfaces\Authenticated\RequestInterface;
use Zynga\Framework\Service\V2\Interfaces\Authenticated\ResponseInterface;
use Zynga\Framework\Service\V2\Interfaces\Authenticated\ServiceInterface;
use Zynga\Framework\Service\V2\Service\Base as ServiceBase;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;

abstract class Base extends ServiceBase implements ServiceInterface {
  const string FAILED_AUTH_MESSAGE = 'Invalid login credentials.';

  public function __construct() {

    parent::__construct();

  }

  // abstract public function request(): RequestInterface;

  // abstract public function response(): ResponseInterface;

  final public function handle(): bool {
    try {

      if ($this->attemptAuth() !== true) {
        $this->handleAuthFailure();
        return false;
      }

      return $this->authenticatedHandle();

    } catch (Exception $e) {
      throw $e;
    }

  }

  abstract public function attemptAuth(): bool;

  abstract public function handleAuthFailure(): bool;

  abstract public function authenticatedHandle(): bool;

}
