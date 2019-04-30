#!/bin/bash

set -e

output=$(./$1 || exit 3)
diff -u "${0%/*}/${1#*/}.expect" - <<<"$output"
