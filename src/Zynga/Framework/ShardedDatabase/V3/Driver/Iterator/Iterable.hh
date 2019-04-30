<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;

/**
 * Interface for iteration across user shards drivers.
 **/
interface Iterable {
    /**
     * Returns the next driver in the iteration, if available.
     *
     * @return The next driver in the iteration.
     */
    public function next(): DriverInterface;

    /**
     * See @next()
     *
     * Equivalent to a call to next(), but must be called at least once before a call to next().
     */
    public function begin(): DriverInterface;

    /**
     * Returns a DriverInterface to be used for the sole purpose of comparing the value returned from
     * a call to next(). If the value returned from next() strictly equals a call to this method, there
     * are no more drivers on which to iterate.
     *
     * @return DriverInterface to be used for the sole purpose of comparing the value returned from
     *         a call to next().
     */
    public function end(): DriverInterface;

    /**
     * This is a convenience method. It need only be called if, in the process of iterating across drivers,
     * the iteration is terminated before all drivers have been iterated on. In that case, this method should
     * be called to clean up any resources and close any open connections resulting from the use of this
     * iterator.
     *
     * @return True if resources successfully cleaned up, otherwise false.
     */
    public function finish(): bool;
}
