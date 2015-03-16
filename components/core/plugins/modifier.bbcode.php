<?php
/**
 * Modifier to count the number of elements
 *
 * @param Array $array
 * @return Boolean
 */
function smarty_modifier_bbcode($string, $formatting = null)
{
    return BBCodeParser::parse($string, true, $formatting !== false);
}
