<?php
/**
* Modifier to check if value is in an array
* 
* @param mixed $element
* @param Array $array
* @return Boolean
*/
function smarty_modifier_notin($element, $array)
{
    return !in_array($element, $array);
}
