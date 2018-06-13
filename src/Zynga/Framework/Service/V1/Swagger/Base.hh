<?hh // strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\Service\V1\Base as ServiceBase;
use Zynga\Framework\Service\V1\Interfaces\ServiceInterface;
use Zynga\Framework\Service\V1\Interfaces\RequestInterface;
use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;

use Zynga\Framework\Service\V1\Swagger\Request;
use Zynga\Framework\Service\V1\Swagger\Response;
use Zynga\Framework\Service\V1\Swagger\ServiceFinder;
use Zynga\Framework\Service\V1\Swagger\ServiceFile;
use Zynga\Framework\Service\V1\Response\Failure as ResponseFailure;

use Zynga\Framework\Swagger\V2\Swagger\Path as SwaggerPath;
use Zynga\Framework\Swagger\V2\Swagger\Parameter as SwaggerParameter;
use Zynga\Framework\Swagger\V2\Swagger\Response as SwaggerResponse;
use Zynga\Framework\Swagger\V2\Swagger\SwaggerType;
use Zynga\Framework\Swagger\V2\Swagger\Schema as SwaggerSchema;
use Zynga\Framework\Swagger\V2\Swagger\Tag as SwaggerTag;

use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\MimeTypeBox;

abstract class Base extends ServiceBase {

  private ?Request $_request;
  private ?Response $_response;

  private ResponseFailure $_responseFailure;

  public function __construct() {

    parent::__construct();

    $this->_responseFailure = new ResponseFailure();

  }

  abstract public function getServiceTitle(): string;

  abstract public function getServiceDescription(): string;

  // --
  // Expectation: Map of { ClassPath => CodePath }
  // --
  abstract public function getClassConfiguration(): Map <string, string>;

  abstract public function getClassBase(): string;

  abstract public function getClassPathBase(): string;

  public function request(): Request {
    if ($this->_request === null) {
      $this->_request = new Request();
    }
    return $this->_request;
  }

  public function response(): Response {
    if ($this->_response === null) {
      $this->_response = new Response();
    }
    return $this->_response;
  }

  public function handle(): bool {

    $this->response()->info->title->set($this->getServiceTitle());
    $this->response()->info->description->set($this->getServiceDescription());
    $this->response()->info->contact->email->set('jorcutt@zynga.com');

    // TODO: Should parse the version off the classname.
    $this->response()->info->version->set('1.0');

    $uri = SuperGlobals::getScriptUri();
    $urlComponents = parse_url($uri);

    $basePath = $urlComponents['path'];
    $basePath = str_replace('swagger.hh', '', $basePath);

    $this->response()->basePath->set($basePath);

    // TODO: hostname needs to set
    // TODO: schemes needs to be set

    $this->addServicesToResponse();

    return true;

  }

  public function addServiceRequestToPath(
    RequestInterface $request,
    SwaggerPath $path,
  ): bool {

    $requestObjectName = get_class($request);
    $requestObjectName = str_replace('\\', '_', $requestObjectName);

    $requestObject = new SwaggerSchema($this->response());
    $requestObject->convertStorableToSchema($request);

    $this->response()->definitions->set($requestObjectName, $requestObject);

    $param = new SwaggerParameter($this->response());
    $param->name->set('body');
    $param->in->set('body');
    $param->schema->ref->set($requestObjectName);
    $param->required->set($request->getIsRequired());

    $path->post->parameters->add($param);

    return true;

  }

  public function addServiceSuccessResponseToPath(
    ResponseInterface $response,
    SwaggerPath $path,
  ): bool {

    $responseObjectName = get_class($response);
    $responseObjectName = str_replace('\\', '_', $responseObjectName);

    $responseObject = new SwaggerSchema($this->response());
    $responseObject->convertStorableToSchema($response);

    $this->response()->definitions->set($responseObjectName, $responseObject);

    $okResponse = new SwaggerResponse($this->response());
    $okResponse->description->set('Successful Request');
    $okResponse->schema->ref->set($responseObjectName);

    $path->post->responses->addSuccessResponse($okResponse);

    return true;

  }

  public function addConsumesAndProducesToPath(SwaggerPath $path): bool {
    $json = new MimeTypeBox();
    $json->set(MimeTypeBox::JSON);
    $path->post->consumes->add($json);
    $path->post->produces->add($json);
    return true;
  }

  public function addServiceFailureResponseToPath(SwaggerPath $path): bool {

    $responseObjectName = get_class($this->_responseFailure);
    $responseObjectName = str_replace('\\', '_', $responseObjectName);

    $responseObject = new SwaggerSchema($this->response());
    $responseObject->convertStorableToSchema($this->_responseFailure);

    $this->response()->definitions->set($responseObjectName, $responseObject);

    $failureResponse = new SwaggerResponse($this->response());
    $failureResponse->description->set('Failed Request');
    $failureResponse->schema->ref->set($responseObjectName);

    $path->post->responses->addFailureResponse($failureResponse);

    return true;

  }

  public function addTag(string $tagName): bool {

    $currentTags = $this->response()->tags->toArray();

    foreach ($currentTags as $tag) {
      if ($tag->name->get() == $tagName) {
        return true;
      }
    }

    $tag = new SwaggerTag();
    $tag->name->set($tagName);
    $this->response()->tags->add($tag);

    return true;
  }

  public function getServiceClasses(): Vector<ServiceFile> {

    $classConfig = $this->getClassConfiguration();

    $services = Vector {};
    foreach ( $classConfig as $classPath => $codePath ) {
      $serviceFinder = new ServiceFinder($classPath, $codePath);
      $serviceFinder->find();
      $foundServices = $serviceFinder->getServices();
      foreach ( $foundServices as $foundService ) {
        $services[] = $foundService;
      }
    }
    return $services;
  }

  public function addServicesToResponse(): bool {

    $serviceClasses = $this->getServiceClasses();

    foreach ($serviceClasses as $serviceClass) {

      $shortClassName = $serviceClass->shortClassName->get();
      $longClassName = $serviceClass->codePath->get().'\\'.$shortClassName;

      if (!DynamicClassCreation::doesClassImplementInterface(
            $longClassName,
            ServiceInterface::class,
          )) {
        // We check this before instantiating the class because
        // non-services might not have consistent constructors
        continue;
      }
      // Stand up the service in question and ask it about the request
      // and response objects it has.
      $service =
        DynamicClassCreation::createClassByName($longClassName, Vector {});

      if ($service instanceof ServiceInterface) {
        // We check this after instantiating also to signal
        // to the typechecker that this is a service interface

        $tagName = $serviceClass->tagName->get();

        // is this the first time we have seen this tag?
        $this->addTag($tagName);

        $uri = $serviceClass->serviceUri->get();

        $path = new SwaggerPath();

        $tagBox = new StringBox();
        $tagBox->set($tagName);

        $path->post->tags->add($tagBox);

        $path->post->description->set($longClassName);

        // Add the typical mime types to the request.
        $this->addConsumesAndProducesToPath($path);

        // What are the request parameters for this service?
        $request = $service->request();

        $this->addServiceRequestToPath($request, $path);

        // so this is the successful response
        $response = $service->response();

        // --
        // we have two responses for every zynga service, successful one and the
        // failure state one.
        // --
        $this->addServiceSuccessResponseToPath($response, $path);
        $this->addServiceFailureResponseToPath($path);

        $this->response()->paths->set($uri, $path);

      }

    }

    return true;

  }

}
