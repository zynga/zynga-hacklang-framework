<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces\Authenticated;

use
  Zynga\Framework\Service\V1\Interfaces\RequestInterface as BaseRequestInterface
;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Poker\Type\Passcode\V1\Box as PasscodeBox;

interface RequestInterface extends BaseRequestInterface {
  public function snid(): SnidBox;
  public function uid(): UidBox;
  public function passcode(): PasscodeBox;
}
