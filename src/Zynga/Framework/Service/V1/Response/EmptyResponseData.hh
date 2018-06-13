<?hh //strict

namespace Zynga\Framework\Service\V1\Response;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\Service\V1\Interfaces\ResponseDataInterface;

class EmptyResponseData extends StorableObjectBase
  implements ResponseDataInterface {}
