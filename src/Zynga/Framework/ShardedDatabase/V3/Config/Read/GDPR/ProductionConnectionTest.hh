<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Read\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Read\ProductionConnectionTest as ReadProductionTest
;

class ProductionConnectionTest extends ReadProductionTest {

  public function getDriverName(): string {
    return 'Read\GDPR';
  }

}
