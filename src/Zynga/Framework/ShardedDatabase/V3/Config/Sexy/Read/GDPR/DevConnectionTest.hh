<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read\DevConnectionTest as ReadDevTest
;

class DevConnectionTest extends ReadDevTest {

  public function getDriverName(): string {
    return 'Sexy\Read\GDPR';
  }

}
