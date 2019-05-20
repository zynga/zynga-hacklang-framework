#!/usr/bin/env hhvm
<?hh

require_once dirname(dirname(dirname(__FILE__))).'/bootstrap.hh';

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use
  Zynga\Framework\Cache\V2\Interfaces\DriverInterface as CacheDriverInterface
;

use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableCacheDriverInterface
;
use Zynga\Framework\Lockable\Cache\V1\Test\Mock\SimpleStorable;

use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\ItemType;

echo "Gp\n";

// // JEO - Testing that caching is working, with mock stuff.
// $mockCache =
//   LockableCacheFactory::factory(LockableCacheDriverInterface::class, 'Mock');

// $mockId = time() + mt_rand();
// $mockTextValue = 'this-is-a-mock-text-value-'.mt_rand();

// $mockObj = new SimpleStorable();
// $mockObj->example_uint64->set($mockId);
// $mockObj->example_string->set($mockTextValue);

// echo "::get\n";
// $initialFetch = $mockCache->get($mockObj);
// var_dump($initialFetch);

// echo "::set\n";
// var_dump($mockCache->set($mockObj));

// echo "::get\n";
// $secondFetch = $mockCache->get($mockObj);
// var_dump($secondFetch);

// echo "DONE!\n";
// exit();

// // JEO - Testing that caching is working correctly still.
// $test1 = new ItemType();
// $test1->id->set(12);

// var_dump($test1);

// $cache = CacheFactory::factory(
//   CacheDriverInterface::class,
//   'LocalMemcache\PgDataTest',
// );

// var_dump($cache->set($test1));

// $obj = $cache->get($test1);

// var_dump($obj);

// exit();

// $inventory = new InventoryModel();

// $all = $inventory->get(ItemType::class);

// echo "All items\n";
// foreach ($all->toArray() as $item) {
//   var_dump($item->export()->asJSON());
// }

// --
// Fetching one object
// --
$inventory = new InventoryModel();

// --
// Read a simple item off the db.
// --
$id = 12387451;
$name = 'this-is-a-test-valueset-1';

echo "Gp\n";

$oneType = $inventory->getByPk(ItemType::class, $id);

echo "One Type\n";
var_dump($oneType->export()->asJSON());
//var_dump($inventory->stats());

$ids = array(12387451, 12387452, 12387453, 12387454, 12387455);

foreach ($ids as $id) {

  echo "loading id=".$id."\n";
  $oneType = $inventory->getByPk(ItemType::class, $id);
  var_dump($oneType->export()->asJSON());
  //var_dump($inventory->stats());

}
