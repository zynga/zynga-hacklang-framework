<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Swagger\V2\Swagger\Contact as SwaggerContact;
use Zynga\Framework\Swagger\V2\Swagger\License as SwaggerLicense;

class Info extends StorableObjectBase {
  public StringBox $title;
  public StringBox $description;
  public StringBox $termsOfService;
  public SwaggerContact $contact;
  public SwaggerLicense $license;
  public StringBox $version;

  public function __construct() {

    parent::__construct();

    $this->title = new StringBox();
    $this->title->setIsRequired(true);

    $this->version = new StringBox();
    $this->version->setIsRequired(true);

    $this->description = new StringBox();

    $this->termsOfService = new StringBox();

    $this->contact = new SwaggerContact();

    $this->license = new SwaggerLicense();

  }

}
