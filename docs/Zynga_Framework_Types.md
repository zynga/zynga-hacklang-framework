# Types

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [Types](#types)
	- [What are boxed types?](#intro)
	- [Historical](#historical)
		- [Basic validator contract](#basic_contract)
		- [A major downside to that system](#downside)
	- [How do I use a boxed type?](#usage)
	- [How do I add new types?](#add)
- [Converted](#converted)
- [Dead](#dead)
- [TODO](#todo)

<!-- /TOC -->

<a name="intro"></a>
## What are boxed types?

PHP does not natively support type ranges or type bounding. While hacklang improves the situation it doesn't solve the problem as it only provides primitive types.

Wiki: [https://en.wikipedia.org/wiki/Object_type_(object-oriented_programming)]

<a name="historical"></a>
## Historical

Zynga poker had a security infrastructure called RV (short for request validator). The validator was built to try to prevent script and sql injection, and XSS attack prevention. This system was later generalized to be reused to validate all data fields within the game. We carried the concept forward into hacklang by creating type boxes that encapsulate the same logic. 

<a name="basic_contract"></a>
### Basic validator contract

1. Attempt to validate request parameter
1. If not validated return null

This code eventually was reused and improved to create the casino app parameter validators. The validators were generalized to allow the developer to validate any variable or content with the same paranoia about XSS, and security.

<a name="downside"></a>
### A major downside to that system

We created a JSON validator, this led to highly complex checking code that layered on top of the loose check for valid JSON. We didn't enforce shapes for the data within a structure consistently. We are attempting to fix that problem set by splitting up the 'type' enforcement from the 'shape'. For more on shape enforcement look at storable objects.

<a name="usage"></a>
## How do I use a boxed type?

```php
use Zynga\Type\V1\UInt64Box;

$box = new UInt64Box();

$box->set(mt_rand(0, 100));
echo "Example uint64: " . $box->get() . "\n";

$box->set(-1); // Throws a exception as this is below bounds of the box.

```
<a name="add"></a>
## How do I add new types?

New types should be added here if they span application domains. Integration of new types should include Swagger updates to make sure that contracts can be properly displayed.

* Add any new type here. Changing type contracts should make a new version. See poker-server\README.md for more info on versioning.
~~~
// Add a new type: Zynga\Type\V*\
namespace Zynga\Type\V1;

class MyNewBox extends SInt64Box {

}
~~~
* Update SwaggerType\Base to allow conversion of the Zynga Type to Swagger Type.

~~~
// Under latest Swagger: /Zynga/Swagger/V*/Swagger/SwaggerType/Base.hh

// Look for the convertZynga*TypeToSwaggerType function that lines up with your type

public function convertZyngaIntegerTypeToSwaggerType(string $type): bool {
 /**
 * etc
 */

 // Look at the class implementation for more details to what types are available.
 if ($type === 'MyNewBox') {
   $this->type->set(self::INTEGER_TYPE);
   $this->format->set(self::INT64FORMAT);
   return true;
 }
}
~~~

* Update Unit Tests for SwaggerType to include conversion of your new type.

~~~
// Zynga/Swagger/V2/Swagger/SwaggerTypeTest.hh

public function testTypeConversion(): void {
  /**
  * etc
  */
  $this->assertTrue($obj->convertZyngaTypeToSwaggerType('MyNewBox'));
}
~~~

* Run the test and make sure it passes

~~~
// In bash, under slice/poker-server
./bin/run-single-test.hh include/Zynga/Swagger/V2/Swagger/SwaggerTypeTest.hh
~~~

* Create or update a Noop request to make sure that your type can be viewed in Swagger.

~~~
// Here we are updating the 'Internal' Noop to include our new type Zynga/Internal/Services/Noop/V1/Request.hh

use Zynga\Type\V1\MyNewBox;

class Request extends RequestBase {
  public MyNewBox $foo;

  public function __construct() {
    // etc
    $this->foo = new MyNewBox();
  }
}
~~~

* Load up the Swagger docroot in your browser and make sure it all works. See the poker-server\docroots\README.md for more info.


# Converted
1. BoolBox
1. FloatBox
1. HttpResponseCodeBox
1. Int32Box
1. Int64Box
1. SInt32Box
1. SInt64Box
1. SnidBox
1. StringBox - Generic string value, this has no shape for the string enforcement. Use sparingly.
1. UInt8Box
1. UInt16Box
1. UInt32Box
1. UInt64Box
1. EmailBox
1. ClientIdBox

# Dead
1. BoolStr - Does not make sense within the box context as we have a native bool type that handles conversion from strings.
1. Array - No shape enforcement for what each element is. If you want to do this in hacklang you should be looking at doing Vector<T> or Map<string, <T>>
1. JSON - No shape enforcement for what element is. We will not be carrying this forward.
1. Regex - No generic regex builder. Whomp whomp.

# TODO
1. AsciiToUnicodeTransform.php
1. AsciiUnicodeEntEscapeTransform.php
1. Base64.php
1. ByteArray.php
1. Checkbox.php
1. DateTime.php
1. DomainName.php
1. EmailSecure.php
1. Enum.php
1. EnumObject.php
1. FilePath.php
1. FlashLocKey.php
1. FloatString.php
1. Int16.php
1. Int24.php
1. Int64Range.php
1. Ip4.php
1. Ip6.php
1. LocKey.php
1. Locale.php
1. MobilePurchaseItemCode.php
1. NoScriptTags.php
1. Object.php
1. PHPLocKey.php
1. PassArrayValidator.php
1. Range.php
1. RangeObject.php
1. SrcTrackingString.php
1. TimeZone.php
1. UInt24.php
1. UnifiedLocKey.php
1. UnixFilename.php
1. UnixPathname.php
1. UnixTimestamp.php
1. Uri.php
1. UriPath.php
1. UserAgent.php
1. WebPurchaseItemCode.php
1. ZTrackCreativeTag.php
1. ZYSendKey.php
1. Zid.php
