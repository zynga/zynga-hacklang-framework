<?hh // strict

namespace Zynga\Framework\Service\V2\Handler;

use Zynga\Framework\Environment\HTTP\HeaderContainer\V2\HeaderContainer as HttpHeaderContainer;
use Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Interfaces\HeaderContainerInterface as HttpHeaderContainerInterface;
use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Logging\V1\StaticLogger;
use Zynga\Framework\Service\V2\Handler\Base as BaseHandler;
use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;
use Zynga\Framework\Service\V2\Response\Failure as ResponseFailure;
use Zynga\Framework\StorableObject\V1\Exceptions\MissingDataFromExportDataException;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\StringBox;

class Http extends BaseHandler {
  private HttpHeaderContainerInterface $_httpHeaders;
  private bool $_sendHttpCode;
  private bool $_sendHeaders;
  private bool $_sendJson;

  public function __construct() {
    parent::__construct();
    $this->_httpHeaders = new HttpHeaderContainer();
    $this->_sendHttpCode = true;
    $this->_sendHeaders = true;
    $this->_sendJson = true;
  }

  public function getHttpHeaderContainer(): HttpHeaderContainerInterface {
    return $this->_httpHeaders;
  }

  public function getSendHttpCode(): bool {
    return $this->_sendHttpCode;
  }

  public function setSendHttpCode(bool $send): bool {
    $this->_sendHttpCode = $send;
    return true;
  }

  public function getSendHeaders(): bool {
    return $this->_sendHeaders;
  }

  public function setSendHeaders(bool $send): bool {
    $this->_sendHeaders = $send;
    return true;
  }

  public function getSendJson(): bool {
    return $this->_sendJson;
  }

  public function setSendJson(bool $send): bool {
    $this->_sendJson = $send;
    return true;
  }

  public function parseRequest(): bool {
    try {
      $service = $this->getService();
      if ($service instanceof ServiceInterface) {
        // pull the request parameters.
        $data = SuperGlobals::getRequestAsMap();
        // pull the form post off the php://input handler (by default)
        $body = SuperGlobals::getRequestBodyAsMap();
        // --
        // JEO: Need to dig a little deeper into why $data->addAll(). doesn't
        // work as intended this is a temporary hack, as it will double the
        // memory impact for larger requests.
        // --
        foreach ($body as $k => $v) {
          $data[$k] = $v;
        }

        // var_dump($data);
        // Bring the input into the dataset.
        $service->request()->import()->fromMap($data);
        return true;
      }

      return false;

    } catch (NoFieldsFoundException $e) {
      // We actually do allow requests to not have any configured fields.
      return true;
    } catch (Exception $e) {
      throw $e;
    }

  }

  public function handleGenericSuccess(): bool {

    $service = $this->getService();

    if ($service instanceof ServiceInterface) {

      parent::handleGenericSuccess();

      $service->response()->code()->set(HttpResponseCodeBox::OK);

      return true;
    }

    return false;

  }

  public function handleGenericFailure(): bool {

    $service = $this->getService();

    if ($service instanceof ServiceInterface) {

      parent::handleGenericFailure();

      $service->response()->code()->set(HttpResponseCodeBox::FAILURE_BAD_REQUEST);

      return true;

    }

    return false;

  }

  public function createHttpCodeResponseFailure(
    ServiceInterface $service,
  ): ResponseFailure {

    $failure = new ResponseFailure();
    $failure->success()->set(false);
    if ( $service->response()->code()->get() >= HttpResponseCodeBox::FAILURE_RANGE_START &&
        $service->response()->code()->get() <= HttpResponseCodeBox::FAILURE_RANGE_END ) {
      $failure->code()->set($service->response()->code()->get());
    } else {
      $failure->code()->set(HttpResponseCodeBox::FAILURE_BAD_REQUEST);
    }

    $values = $service->response()->message()->toArray();

    foreach ($values as $value) {
      $failure->message()->add($value);
    }

    return $failure;

  }

  public function createMissingDataFailure(
    ServiceInterface $service,
    MissingDataFromExportDataException $e,
  ): ResponseFailure {
    $failure = new ResponseFailure();
    $failure->success()->set(false);
    $failure->code()->set(HttpResponseCodeBox::FAILURE_BAD_REQUEST);
    $errorMessage = new StringBox();
    $errorMessage->set(
      $this->createShortExceptionName($e).': '.$e->getMessage(),
    );
    $failure->message()->add($errorMessage);
    return $failure;
  }

  public function createJsonForResponse(): string {

    $service = $this->getService();

    if (!$service instanceof ServiceInterface) {
      return '{"message":"invalid-service-for-export"}';
    }

    try {

      // --
      // Mask down the return, in the case of failure to only the failure object.
      //
      // We are attempting to prevent disclosure of data that the end users are
      //  not needing.
      //
      // --
      if ($service->response()->code()->get() !== HttpResponseCodeBox::OK) {

        $failure = $this->createHttpCodeResponseFailure($service);
        return $failure->export()->asJSON();

      }

      // Create the json for the successful response
      return $service->response()->export()->asJSON();

    } catch (MissingDataFromExportDataException $e) {

      // Create a response that is appropriate for the missing data issue.
      $failure = $this->createMissingDataFailure($service, $e);
      return $failure->export()->asJSON();

    }

  }

  public function handleResponse(): bool {

    // --
    // Stand up the headers needed for the response.
    // --
    try {

      $service = $this->getService();

      if (!$service instanceof ServiceInterface) {
        return false;
      }

      $json = $this->createJsonForResponse();

      // Send out the appropriate http response code.
      if ($this->_sendHttpCode === true) {
        http_response_code($service->response()->code()->get());
      }

      // send out the http headers for the request.
      if ($this->_sendHeaders === true) {
        $headers = $this->getHttpHeaderContainer();
        $headers->noCaching();
        $headers->contentIsJSON();
        $headers->send();
      }

      // Give them back some JSON to consume.
      if ($this->_sendJson === true) {
        echo $json;
      }

      return true;

    } catch (Exception $e) {
      // this is a hard failure situation, we can't use our normal methods.
      echo '{"message":"uncaught-exception"}';
      StaticLogger::exception('responseErrorCaught', Map {}, $e, true);
      return false;
    }
  }
}
