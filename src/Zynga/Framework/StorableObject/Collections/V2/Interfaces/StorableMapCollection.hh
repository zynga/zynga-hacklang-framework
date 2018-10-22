<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\V2\Interfaces;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

/**
 * Do you want to create your own collection?
 * Do you want to also support serializing/deserializing storables/boxes?
 * Go ahead and implement this interface! Note that on purpose we do not
 * use the provided collection interfaces from HH\*. The reason is because
 * they are not strict mode safe on top of the language designers not intending them
 * to be extended for anything other than intrinsics.
 * @see https://github.com/facebook/hhvm/issues/8042#issuecomment-342976327
 */
interface StorableMapCollection<Tv> extends StorableObjectInterface {
  /**
   * Removes all items from the collection.
   */
  public function clear(): bool;

  /**
   * Is the collection empty?
   *
   * @return - Returns `true` if the collection is empty; `false`
   *           otherswise.
   */
  public function isEmpty(): bool;

  /**
   * Get the number of items in the collection. Cannot be negative.
   *
   * @return - Returns the number of items in the collection
   */
  public function count(): int;

  /**
   * Get access to the items in the collection. Can be empty.
   *
   * @return - Returns an `Iterable` with access to all of the items in
   *   the collection.
   */
  public function items(): Map<string, Tv>;

  /**
   * Add a key/value pair to the end of the collection and return itself.
   * If the key in the Pair exists, the value associated with it is overwritten.
   *
   * It returns a shallow copy of the current collection, meaning changes
   * made to the current collection will be reflected in the returned
   * collection.
   *
   * @param Pair<string, Tv> $p - The pair to add.
   *
   * @return - A shallow copy of the updated current collection itself.
   */
  public function add(Pair<string, Tv> $p): this;

  /**
   * Set a value for specified key overwrting the previous value
   * associated with the key and return the collection itself.
   * If the key does not exist, it is created.
   *
   * It returns a shallow copy of the current collection, meaning changes
   * made to the current collection will be reflected in the returned
   * collection.
   *
   * @param string $k - The key to add value to.
   * @param Tv $v - The value to add.
   *
   * @return - A shallow copy of the updated current collection itself.
   */
  public function set(string $k, Tv $v): this;

  /**
   * For every element in the provided `Traversable`, append a value into the
   * current collection.
   *
   * It returns a shallow copy of the current collection, meaning changes
   * made to the current collection will be reflected in the returned
   * collection.
   *
   * @param $traversable - The `Traversable` with the new values to set. If
   *                       `null` is provided, no changes are made.
   *
   * @return - A shallow copy of the current collection with the added the
   *           values.
   */
  public function addAll(?Traversable<Pair<string, Tv>> $iterable): this;

  /**
   * Remove a key and its associated value and return the collection itself.
   *
   * It returns a shallow copy of the current collection, meaning changes
   * made to the current collection will be reflected in the returned
   * collection.
   *
   * @param string $k - The to be removed.
   *
   * @return - A shallow copy of the updated current collection itself.
   */
  public function remove(string $k): this;

  /**
   * If the key is not present, an exception is thrown.
   */
  public function at(string $key): Tv;

  /**
   * Converts collection to raw array
   * Similar to @see https://docs.hhvm.com/hack/reference/class/HH.Map/toArray/
   */
  public function toArray(): array<string, Tv>;

}
