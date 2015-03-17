<?php
function smarty_function_ref($params, &$smarty)
{
    $code = strtolower($params['code']);
    return "<a href=\"/abc#{$code}\" class=\"glossaryref\">[abc]</a>";
}