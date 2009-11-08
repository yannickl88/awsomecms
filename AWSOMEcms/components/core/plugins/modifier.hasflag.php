<?php
function smarty_modifier_hasflag($int, $flag)
{
    return Config::hasFlag($int, $flag);
}
