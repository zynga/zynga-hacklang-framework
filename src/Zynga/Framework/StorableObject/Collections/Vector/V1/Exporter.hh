<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use
  Zynga\Framework\StorableObject\Collections\Exporters\V1\Base as CollectionExporter
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class Exporter<Tv as StorableObjectInterface> extends CollectionExporter<Tv> {}
