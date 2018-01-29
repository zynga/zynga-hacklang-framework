<?hh // decl /* -*- php -*- */

const int XHPROF_FLAGS_NO_BUILTINS = 0x1;
const int XHPROF_FLAGS_CPU = 0x2;
const int XHPROF_FLAGS_MEMORY = 0x4;

function xhprof_disable(): array;
function xhprof_enable(int $flags = 0, array $options = array()): void;
