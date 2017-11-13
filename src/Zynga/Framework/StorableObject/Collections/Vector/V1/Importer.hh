<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use
  Zynga\Framework\StorableObject\Collections\Importers\V1\Base as CollectionImporter
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class Importer<Tv as StorableObjectInterface> extends CollectionImporter<Tv> {}
