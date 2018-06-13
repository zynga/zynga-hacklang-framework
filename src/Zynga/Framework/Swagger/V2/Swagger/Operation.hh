<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\MimeTypeBox;

use Zynga\Framework\Swagger\V2\Swagger\Parameter as SwaggerParameter;
use Zynga\Framework\Swagger\V2\Swagger\Responses as SwaggerResponses;
use Zynga\Framework\Swagger\V2\Swagger\Tag as SwaggerTag;

class Operation extends StorableObjectBase {

  public StorableVector<StringBox> $tags;
  public StringBox $summary;
  public StringBox $description;
  // TODO: ExternalDocumentation $externalDocs
  public StringBox $operationId;
  public StorableVector<MimeTypeBox> $consumes;
  public StorableVector<MimeTypeBox> $produces;
  public StorableVector<SwaggerParameter> $parameters;
  public SwaggerResponses $responses;
  // TODO: $schemes
  public BoolBox $deprecated;
  // TODO: SecurityType $security

  public function __construct() {

    parent::__construct();

    $this->tags = new StorableVector(StringBox::class);
    $this->summary = new StringBox();

    $this->description = new StringBox();
    $this->description->setIsRequired(true);

    $this->operationId = new StringBox();
    $this->consumes = new StorableVector(MimeTypeBox::class);
    $this->produces = new StorableVector(MimeTypeBox::class);
    $this->parameters = new StorableVector(SwaggerParameter::class);

    $this->responses = new SwaggerResponses();
    $this->responses->setIsRequired(true);

    $this->deprecated = new BoolBox();

  }

}
