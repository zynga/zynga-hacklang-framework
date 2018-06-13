<?hh // strict

namespace Zynga\Framework\Service\V1\Authenticated\Request;

use Zynga\Framework\Service\V1\Authenticated\Request\Importer;
use Zynga\Framework\Service\V1\Authenticated\Request\ServiceData;
use Zynga\Framework\Service\V1\Interfaces\Authenticated\RequestInterface;
use Zynga\Framework\Service\V1\Request\Base as RequestBase;
use Zynga\Framework\Type\V1\ChecksumBox;
use Zynga\Poker\Type\Passcode\V1\Box as PasscodeBox;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Framework\Service\V1\Authenticated\Request\LegacyParameters;
use Zynga\Framework\StorableObject\V1\StorableMap;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\Service\V1\Authenticated\Request\Interfaces\ServiceDataInterface;
use Zynga\Framework\Service\V1\Authenticated\Request\ServiceData\EmptyData;

/**
 * Base authenticated request class. This is the starting point for all
 * authenticated services
 */
abstract class Base extends RequestBase implements RequestInterface {

    /**
     * Randomly generated string with which to salt the checksum hash
     */
    private static string $hashSalt = "uDSUJJUnLJP8lYQfoCW5";

    /**
     * Social network id associated with this request
     */
    public SnidBox $snid;

    /**
     * User id associated with this request
     */
    public UidBox $uid;

    /**
     * Passcode used to authenticate this request
     */
    public PasscodeBox $passcode;

    /**
     * A checksum over the entire request. Used to ensure that whoever sent
     * this message used the hashing algorithm and salt on our client
     */
    public ChecksumBox $checksum;

    /**
     * A collection of parameters sent for all legacy feature requests to use
     * Will eventually be removed
     */
    public LegacyParameters $legacyParameters;

    /**
     * PHP magic function
     */
    public function __construct() {
        parent::__construct();

        $this->snid = new SnidBox();
        $this->snid->setIsRequired(true);

        $this->uid = new UidBox();
        $this->uid->setIsRequired(true);

        $this->passcode = new PasscodeBox();
        $this->passcode->setIsRequired(true);

        $this->checksum = new ChecksumBox();
        $this->checksum->setIsRequired(true);

        $this->legacyParameters = new LegacyParameters();
        $this->legacyParameters->setIsRequired(true);
    }

    <<__Override>>
    public function import() : Importer {
        return new Importer($this);
    }

    /**
     * Retrieves the checksum for this request.
     *
     * @return A checksum over the entire request. Used to ensure that whoever
     *         sent this message used the hashing algorithm and salt on our
     *         client
     */
    public function checksum(): ChecksumBox {
        return $this->checksum;
    }

    /**
     * Retrieves the social network ID associated with this request
     *
     * @return Social network id associated with this request
     */
    public function snid(): SnidBox {
        return $this->snid;
    }

    /**
     * Retrieves the user id associated with this request
     *
     * @return User id associated with this request
     */
    public function uid(): UidBox {
        return $this->uid;
    }

    /**
     * Retrieves the passcode used to authenticate this request
     *
     * @return Passcode used to authenticate this request
     */
    public function passcode(): PasscodeBox {
        return $this->passcode;
    }

    /**
     * Retrieves the collection of parameters sent for all legacy feature
     * requests to use. Will eventually be removed.
     *
     * @return The legacy parameters associated with this request
     */
    public function legacyParameters(): LegacyParameters {
        return $this->legacyParameters;
    }

    /**
     * Retrieves the service-specific values sent as part of this request
     *
     * @return The service-specific values associated with this request
     */
    public function serviceData(): ServiceDataInterface {
      return new EmptyData();
    }

    /**
     * Reconstructs the checksum of this request that should match the same
     * checksum passed from the client
     */
    public function reconstructChecksum(): ChecksumBox {
        $toHash = $this->passcode()->get();
        $toHash .= $this->serviceData()->export()->asJSON(null, true);
        $toHash .= $this->legacyParameters()->export()->asJSON(null, true);
        $toHash .= self::$hashSalt;
        $result = new ChecksumBox();
        $result->set(md5($toHash));
        return $result;
    }

}
