<?php
/**
* Modifier to binary check if a flag is in a value
* 
* @param Int $int
* @param Int $flag
* @return Boolean
*/
function smarty_modifier_hasflag($int, $flag)
{
    return Config::hasFlag((int) $int, (int) $flag);
}
