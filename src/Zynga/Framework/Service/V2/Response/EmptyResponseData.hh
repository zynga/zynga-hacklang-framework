<?hh //strict

namespace Zynga\Framework\Service\V2\Response;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\Service\V2\Interfaces\ResponseDataInterface;

class EmptyResponseData extends StorableObjectBase
  implements ResponseDataInterface {}
