<?hh // strict

namespace Zynga\Framework\PgData\V1\PgFk;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

class FkCache<TModel as PgModelInterface, TRow as PgRowInterface> {
  private static Map<string, ?PgRowInterface> $_cache = Map {};

  public static function getFk(
    classname<TModel> $fkModel,
    classname<TRow> $fkRow,
    mixed $fkId,
  ): ?PgRowInterface {

    $cacheKey = self::createCacheKey($fkModel, $fkRow, $fkId);

    // have we have done a resolve before? if so return the cached results.
    if (self::$_cache->at($cacheKey)) {
      return self::$_cache->get($cacheKey);
    }

    // do a resolve on
    try {

      $model = DynamicClassCreation::createClassByName($fkModel, Vector {});

      if ($model instanceof PgModelInterface) {
        $actualFkRow = $model->getByPk($fkRow, $fkId, false);
        self::$_cache->set($cacheKey, $actualFkRow);
        return $actualFkRow;
      }

      return null;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public static function createCacheKey(
    classname<TModel> $fkModel,
    classname<TRow> $fkRow,
    mixed $fkId,
  ): string {
    return $fkModel.'|'.$fkRow.'|'.strval($fkId);
  }

}
