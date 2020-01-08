<?hh // strict

namespace Zynga\Framework\Swagger\V2;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Swagger\V2\Swagger\Info as SwaggerInfo;
use Zynga\Framework\Swagger\V2\Swagger\Path as SwaggerPath;
use Zynga\Framework\Swagger\V2\Swagger\Schema as SwaggerSchema;
use Zynga\Framework\Swagger\V2\Swagger\Tag as SwaggerTag;

use Zynga\Framework\StorableObject\V1\StorableMap;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;

class Swagger extends StorableObjectBase {
  const string SWAGGER_VERSION = "2.0";

  public StringBox $swagger;
  public SwaggerInfo $info;
  public StringBox $host;
  public StringBox $basePath;
  public StorableMap<SwaggerPath> $paths;
  public StorableMap<SwaggerSchema> $definitions;
  public StorableVector<SwaggerTag> $tags;

  public function __construct() {

    parent::__construct();

    $this->swagger = new StringBox();
    $this->swagger->setIsRequired(true);
    $this->swagger->set(self::SWAGGER_VERSION);

    $this->info = new SwaggerInfo();
    $this->info->setIsRequired(true);

    $this->host = new StringBox();
    $this->basePath = new StringBox();

    $this->paths = new StorableMap();
    $this->paths->setIsRequired(true);

    $this->definitions = new StorableMap();
    $this->tags = new StorableVector(SwaggerTag::class);

  }

}
