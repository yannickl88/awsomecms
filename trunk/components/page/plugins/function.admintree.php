<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Show the admin tree
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_admintree($params, &$smarty)
{
    import('components/page/util/class.Tree.inc');
    
    $tree = new Tree();
    $types = 0;
    
    if(isset($params['javascript']))
    {
        $tree->javascript = ''.$params['javascript'];
    }
    if(isset($params['types']))
    {
        $types = (int) $params['types'];
    }
    $tree->onlyShowFolders = (isset($params['folderonly']) && $params['folderonly'] === true);
    
    if(isset($params['hideadmin']))
    {
        $tree->hideAdmin = ($params['hideadmin'] === true);
    }
    
    return $tree->populate()
        ->toHTML($types);
}