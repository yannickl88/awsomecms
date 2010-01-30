<?php
function smarty_function_layout($params, &$smarty)
{
    if(!empty($params['set']))
    {
        $smarty->assign('LAYOUT', $params['set']);
    }

    return;
}