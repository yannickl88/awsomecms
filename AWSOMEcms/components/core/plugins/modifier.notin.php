<?php
function smarty_modifier_notin($element, $array)
{
    return !in_array($element, $array);
}
