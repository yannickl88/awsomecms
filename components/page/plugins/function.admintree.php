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
import('page.util.Tree');
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
    
    $tree = Tree::getInstance();
    $types = array();
    $filetypes = array();
    $javascript = false;
    $noCookie = false;
    $checkboxes = array(Tree::NONE, array());
    
    if(!empty($params['javascript']))
    {
        $javascript = ''.$params['javascript'];
    }
    if(!empty($params['checkboxes']))
    {
        $checkboxes = array($params['checkboxes'], $params['value']);
    }
    if(!empty($params['types']))
    {
        $types = explode(",", $params['types']);
        
        foreach($types as $key => $value)
        {
            $types[$key] = (int) trim($value);
        }
    }
    if(!empty($params['filetypes']))
    {
        $filetypes = $params['filetypes'];
    }
    if(!empty($params['noCookie']) && $params['noCookie'] == true)
    {
        $noCookie = true;
    }
    $onlyShowFolders = (isset($params['folderonly']) && $params['folderonly'] === true);
    
    if(!empty($params['hideadmin']))
    {
        $tree->hideAdmin = ($params['hideadmin'] === true);
    }
    
    if(!empty($params['systemFileTree']))
    {
        $tree->systemFileTree = ($params['systemFileTree'] === true);
    }
    
    return $tree->toJavascript("tree", null, $types, $onlyShowFolders, $javascript, $checkboxes, $filetypes, $noCookie);
}