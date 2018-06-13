<?hh // strict

namespace Zynga\Framework\Service\V1\Authenticated;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V1\Interfaces\Authenticated\RequestInterface;
use Zynga\Framework\Service\V1\Interfaces\Authenticated\ResponseInterface;
use Zynga\Framework\Service\V1\Interfaces\Authenticated\ServiceInterface;
use Zynga\Framework\Service\V1\Service\Base as ServiceBase;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Poker\User\V1\Authenticated\User;
use Zynga\Poker\User\V1\Authentication\Passcode;
use Zynga\Legacy\Log\V1\Log;

abstract class Base extends ServiceBase implements ServiceInterface {
  const string FAILED_AUTH_MESSAGE = 'Invalid login credentials.';
  public User $user;

  public function __construct() {

    parent::__construct();

    $this->user = new User();

  }

  abstract public function request(): RequestInterface;

  abstract public function response(): ResponseInterface;

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

  final public function attemptAuth(): bool {
    try {

      $request = $this->request();

      return Passcode::verifyPasscode(
        $request->snid(),
        $request->uid(),
        $request->passcode(),
        $this->user,
      );

    } catch (Exception $e) {
      // --
      // In this case we swallow the exception, and return false. We do this
      // in order to prevent the whole stack going down in the case of a shard
      // failure.
      // --
      return false;
    }
  }

  final public function handleAuthFailure(): bool {
    $this->response()->success()->set(false);
    $this->response()->code()->set(HttpResponseCodeBox::HTTP_AUTH_FAILURE);
    $this->response()->addMessage(self::FAILED_AUTH_MESSAGE);
    return true;
  }

  abstract public function authenticatedHandle(): bool;

}
