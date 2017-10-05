<?hh // strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\StorableObject\V1\Fields;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\StorableVector\Exporter;
use Zynga\Framework\StorableObject\V1\StorableVector\Importer;
use Zynga\Type\V1\Interfaces\TypeInterface;

class StorableVector<Tv> implements StorableObjectInterface {
    private Vector<Tv> $_vector;
    private bool $_isRequired;
    private ?bool $_isDefaultValue;

    public function __construct() {
        $this->_isRequired = false;
        $this->_isDefaultValue = null;
        $this->_vector = Vector {};
    }

    public function reset(): bool {
        $this->_vector->clear();
        $this->_isDefaultValue = null;
        return true;
    }

    public function count(): int {
        return $this->_vector->count();
    }

    public function getVector(): Vector<Tv> {
        return $this->_vector;
    }

    public function add(Tv $value): bool {
        $this->_vector[] = $value;
        return true;
    }

    public function get(int $key): ?Tv {
        if ($this->_vector->containsKey($key) === true) {
            return $this->_vector[$key];
        }

        return null;
    }

    public function set(int $key, Tv $value): bool {
        $this->_vector[$key] = $value;
        return true;
    }

    public function setIsDefaultValue(bool $tf): bool {
        $this->_isDefaultValue = $tf;
        return true;
    }

    public function isDefaultValue(): (bool, Vector<string>) {
        $defaultFields = Vector {};

        if ($this->_isDefaultValue !== null) {
            return tuple($this->_isDefaultValue, $defaultFields);
        }

        $offset = 0;
        $hadNonDefault = false;

        foreach ($this->_vector as $value) {
            list($f_isRequired, $f_isDefaultValue) =
                FieldsGeneric::getIsRequiredAndIsDefaultValue($value);

            if ($f_isDefaultValue === true) {
                $defaultFields[] = '['.strval($offset).']';
            } else {
                $hadNonDefault = true;
            }

            $offset++;
        }

        $isDefaultValue = true;

        // echo json_encode(array(get_class($this), $this->_vector)) . "\n";

        if ($hadNonDefault === true) {
            $isDefaultValue = false;
        }

        return tuple($isDefaultValue, $defaultFields);
    }

    public function fields(): FieldsInterface {
        $fields = new Fields($this);
        return $fields;
    }

    public function import(): ImportInterface {
        // TODO: This needs a real vector importer (booo), not done yet.
        $importer = new Importer($this);
        return $importer;
    }

    public function export(): ExportInterface {
        $exporter = new Exporter($this);
        return $exporter;
    }

    public function setIsRequired(bool $isRequired): bool {
        $this->_isRequired = $isRequired;
        return true;
    }

    public function getIsRequired(): bool {
        return $this->_isRequired;
    }
}
