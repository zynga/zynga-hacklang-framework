<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Read\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Read\DevConnectionTest as ReadDevTest
;

class DevConnectionTest extends ReadDevTest {

  public function getDriverName(): string {
    return 'Read\GDPR';
  }

}
