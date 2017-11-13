<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\V1\Interfaces;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

/**
 * Do you want to create your own collection?
 * Do you want to also support serializing/deserializing storables?
 * Go ahead and implement this interface! Note that on purpose we do not
 * use the provided collection interfaces from HH\*. The reason is because
 * they are not strict mode safe on top of the language designers not intending them
 * to be extended for anything other than intrinsics.
 * @see https://github.com/facebook/hhvm/issues/8042#issuecomment-342976327
 */
interface StorableCollection<Tv as StorableObjectInterface>
  extends StorableObjectInterface {
  /**
   * Removes all items from the collection.
   */
  public function clear(): void;

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
  public function items(): Iterable<Tv>;

  /**
   * Add a value to the collection and return the collection itself.
   *
   * It returns a shallow copy of the current collection, meaning changes
   * made to the current collection will be reflected in the returned
   * collection.
   *
   * @param $e - The value to add.
   *
   * @return - A shallow copy of the updated current collection itself.
   */
  public function add(Tv $e): this;
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
  public function addAll(?Traversable<Tv> $traversable): this;

  /**
   * @throws OutOfBoundsException
   */
  public function at(int $key): Tv;
}
