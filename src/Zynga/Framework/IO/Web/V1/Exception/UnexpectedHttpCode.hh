<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Exception;

use Zynga\Framework\IO\Web\V1\Exception\WebIO as WebIOException;

/**
 * Represents that an unexpected http code was encountered in the
 * course of Web input or output
 */
class UnexpectedHttpCode extends WebIOException {}
