<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces;

use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\StringBox;

interface ResponseInterface extends StorableObjectInterface {
  public function success(): BoolBox;
  public function code(): HttpResponseCodeBox;
  public function message(): StorableVector<StringBox>;
  public function addMessage(string $message): bool;
}
