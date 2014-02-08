--TEST--
Ternary operator test (constants)
--FILE--
<?php

/**
 * A test function
 * 
 * @engine	qb
 * @param	int64	$a
 * @local	int64	$b
 * 
 * @return	void
 * 
 */
function test_function($a) {
	$b = ($a > 10) ? 123 : 456;
	echo "$b\n";
}

test_function(12);
test_function(5);

?>
--EXPECT--
123
456
