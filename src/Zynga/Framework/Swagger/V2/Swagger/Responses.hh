<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Swagger\V2\Swagger\Response as SwaggerResponse;
use Zynga\Framework\Swagger\V2\Swagger\Responses\Fields as SwaggerResponsesFields;
use Zynga\Framework\Swagger\V2\Swagger\Responses\Export as SwaggerResponsesExport;
use Zynga\Framework\Swagger\V2\Swagger\Responses\Import as SwaggerResponsesImport;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

class Responses implements StorableObjectInterface {
  const int HTTP_OK = 200;
  const int HTTP_FAILURE = 400;

  private Map<int, SwaggerResponse> $_responses;
  private bool $_isDefaultValue;

  public function __construct() {

    $this->_responses = Map {};
    $this->_isDefaultValue = true;

  }

  public function setIsDefaultValue(bool $tf): bool {
    $this->_isDefaultValue = $tf;
    return true;
  }

  public function isDefaultValue(): (bool, Vector<string>) {

    // We don't add anything to our map unless it's been verified as good.
    $defaultFields = Vector {};

    return tuple($this->_isDefaultValue, $defaultFields);

  }

  public function getMap(): Map<int, SwaggerResponse> {
    return $this->_responses;
  }

  public function addSuccessResponse(SwaggerResponse $response): bool {
    $defValues =
      $response->fields()
        ->getRequiredFieldsWithDefaultValues('Responses.addSuccessResponse');
    if ($defValues->count() > 0) {
      return false;
    }
    $this->_isDefaultValue = false;
    $this->_responses->set(self::HTTP_OK, $response);
    return true;
  }

  public function addFailureResponse(SwaggerResponse $response): bool {
    $defValues =
      $response->fields()
        ->getRequiredFieldsWithDefaultValues('Responses.addSuccessResponse');
    if ($defValues->count() > 0) {
      return false;
    }
    $this->_isDefaultValue = false;
    $this->_responses->set(self::HTTP_FAILURE, $response);
    return true;
  }

  public function fields(): FieldsInterface {
    $obj = new SwaggerResponsesFields();
    return $obj;
  }

  public function import(): ImportInterface {
    $obj = new SwaggerResponsesImport();
    return $obj;
  }

  public function export(): ExportInterface {
    $obj = new SwaggerResponsesExport($this);
    return $obj;
  }

  public function setIsRequired(bool $isRequired): bool {
    // you can't make this non-required
    return true;
  }

  public function getIsRequired(): bool {
    return true;
  }

}
