<?hh // strict

namespace Zynga\Framework\Service\V1\Authenticated\Request;

use Zynga\Framework\StorableObject\V1\Base as BaseStorableObject;
use Zynga\Framework\Type\V1\UtcOffsetBox;

use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Poker\Type\AdvertisementId\V1\Box as AdvertisementIdBox;
use Zynga\Poker\Type\App\Version\V1\Box as AppVersionBox;
use Zynga\Poker\Type\Carrier\Id\V1\Box as CarrierIdBox;
use Zynga\Poker\Type\Client\Id\V1\Box as ClientIdBox;
use Zynga\Poker\Type\Device\Id\V1\Box as DeviceIdBox;
use Zynga\Poker\Type\Device\Model\V1\Box as DeviceModelBox;
use Zynga\Poker\Type\Device\Resolution\V1\Box as ResolutionBox;
use Zynga\Poker\Type\Device\SystemMemory\V1\Box as SystemMemoryBox;
use Zynga\Poker\Type\Locale\V1\Box as LocaleBox;
use Zynga\Poker\Type\OperatingSystem\Version\V1\Box as OperatingSystemVersionBox;

/**
 * Encapsulation of parameters sent up with potentially every service request
 * for the legacy purpose of allowing features to access these values without
 * having cached them.
 *
 * DO NOT ADD TO THESE
 *
 * The goal is to migrate to a pattern of sending these up once on auth and
 * storing them for a session. At that point, this class will be deleted.
 */
class LegacyParameters extends BaseStorableObject {

    /**
     * See BaseStorableObject
     */
    private bool $isRequired;

    /**
     * See BaseStorableObject
     */
    private bool $isDefaultValue;

    /**
     * Unique identifier per device. Used for tracking and correlating related
     * users across multiple accounts and devices.
     */
    public UidBox $anonymousId;

    /**
     * ID of the platform of the client from which these legacy parameters were
     * sent
     */
    public ClientIdBox $platformId;

    /**
     * ID of the carrier of the client from which these legacy parameters were
     * sent
     */
    public CarrierIdBox $carrierId;

    /**
     * The application version of the client from which these legacy parameters
     * were sent
     */
    public AppVersionBox $appVersion;

    /**
     * The resolution of the client from which these legacy parameters were sent
     */
    public ResolutionBox $resolution;

    /**
     * The utc offset of the client from which these legacy parameters were sent
     */
    public UtcOffsetBox $utcOffset;

    /**
     * The device model of the client from which these legacy parameters were
     * sent
     */
    public DeviceModelBox $deviceModel;

    /**
     * The device identifier of the client from which these legacy parameters
     * were sent
     */
    public DeviceIdBox $deviceId;

    /**
     * The advertisement id of the client from which these legacy parameters
     * were sent
     */
    public AdvertisementIdBox $advertisementId;

    /**
     * The locale of the client from which these legacy parameters were sent
     */
    public LocaleBox $locale;

    /**
     * The version of the operating system from which these legacy parameters
     * were sent
     */
    public OperatingSystemVersionBox $osVersion;

    /**
     * The amount of system memory available on the client from which these
     * legacy parameters were received
     */
    public SystemMemoryBox $systemMemory;

    <<__Override>>
    public function __construct() {
        parent::__construct();

        $this->isRequired = false;
        $this->isDefaultValue = true;

        $this->anonymousId = new UidBox();
        $this->anonymousId->setIsRequired(false);

        $this->platformId = new ClientIdBox();
        $this->platformId->setIsRequired(false);

        $this->carrierId = new CarrierIdBox();
        $this->carrierId->setIsRequired(false);

        $this->appVersion = new AppVersionBox();
        $this->appVersion->setIsRequired(false);

        $this->resolution = new ResolutionBox();
        $this->resolution->setIsRequired(false);

        $this->utcOffset = new UtcOffsetBox();
        $this->utcOffset->setIsRequired(false);

        $this->deviceModel = new DeviceModelBox();
        $this->deviceModel->setIsRequired(false);

        $this->deviceId = new DeviceIdBox();
        $this->deviceId->setIsRequired(false);

        $this->advertisementId = new AdvertisementIdBox();
        $this->advertisementId->setIsRequired(false);

        $this->locale = new LocaleBox();
        $this->locale->setIsRequired(false);

        $this->osVersion = new OperatingSystemVersionBox();
        $this->osVersion->setIsRequired(false);

        $this->systemMemory = new SystemMemoryBox();
        $this->systemMemory->setIsRequired(true);
    }
}
