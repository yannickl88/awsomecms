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
    $javascript = ''.$params['javascript'];
    $folderOnly = ($params['folderonly'] === true);
    
    if(isset($params['hideadmin']))
    {
        $hideAdmin = ($params['hideadmin'] === true);
    }
    else
    {
        $hideAdmin = Config::get("hideadmintree", true, "admin") === "1";
    }

    
    $tree = array();

    $currentFolder = '/';
    $table = Table::init("page.pages", "admintree");

    if($hideAdmin)
    {
        $table->setRequest((object) array("where" => array('page_location', "/admin/%", "NOT LIKE")));
    }

    $result = $table->doSelect()->getRows();

    foreach($result as $row)
    {
        pages_addToTree(substr($row->page_location, 1), $row, $tree['items'], '/');
    }

    include_once Config::get('websiteroot', '.').'/../components/page/util/class.TreeSorter.inc';

    $sorter = new TreeSorter($tree);
    $tree = $sorter->sort();

    $html = '<div class="treeNode" style="padding: 0;">';
    $html .= pages_renderTree($tree, $theme, true, $javascript, $folderOnly);
    $html .= '</div>';

    return $html;
}
/**
 * Add an item to the tree
 * 
 * @param string $location
 * @param Object $item
 * @param array $tree
 * @param string $parentLoc
 */
function pages_addToTree($location, $item, &$tree, $parentLoc)
{
    if($location == '')
    {
        if($item->page_isfolder == 1)
        {
            $tree[$item->page_name]['items'] = array();
            $tree[$item->page_name]['location'] = $parentLoc.$item->page_name.'/';
        }
        else
        {
            $tree[] = $item;
        }
    }
    else
    {
        $locationArray = explode('/', $location);
        $folder = array_shift($locationArray);

        $tree[$folder]['location'] = $parentLoc.$folder.'/';

        pages_addToTree(implode('/', $locationArray), $item, &$tree[$folder]['items'], $tree[$folder]['location']);
    }
}
/**
 * Display the tree
 * 
 * @param Array $tree
 * @param string $theme
 * @param boolean $hidden
 * @param string $javascript
 * @param boolean $folderOnly
 * @return string
 */
function pages_renderTree($tree, $theme, $hidden, $javascript = '', $folderOnly = false)
{
    $html = '';

    if($tree['items'] && is_array($tree['items']))
    {
        foreach($tree['items'] as $key => $node)
        {
            $html .= '<div class="hideIcons" onMouseOver="$(this).removeClass(\'hideIcons\');" onMouseOut="$(this).addClass(\'hideIcons\');">';

            if(!is_numeric($key))
            {
                $id = md5($node['location']);

                if($hidden && !isset($_COOKIE['menu'.$id]))
                {
                    $html .= '<a id="link'.$id.'" class="folder link'.$id.'" href="#" onClick="return admin_toggleTree(\''.$id.'\');" onMouseOver="$(this).addClass(\'focus\')" onMouseOut="$(this).removeClass(\'focus\')">';
                }
                else
                {
                    $html .= '<a id="link'.$id.'" class="folder open link'.$id.'" href="#" onClick="return admin_toggleTree(\''.$id.'\');" onMouseOver="$(this).addClass(\'focus\')"  onMouseOut="$(this).removeClass(\'focus\')">';
                }
                $html .= '</a>';

                if($javascript != '')
                {
                    $html .= '<a href="#" onClick="$(\'#'.$javascript.'\').val(\''.$node['location'].'\'); return false;" style="padding: 0;">';
                }
                else
                {
                    $html .= '<a href="#" onClick="return admin_toggleTree(\''.$id.'\');" style="padding: 0;">';
                }
                $html .= '<img src="/img/icons/page_folder.png" />';
                $html .= $key;
                $html .= '</a>';
                if($hidden && !isset($_COOKIE['menu'.$id]))
                {
                    $html .= '<div class="treeNode hidden sub'.$id.'">';
                }
                else
                {
                    $html .= '<div class="treeNode sub'.$id.'">';
                }
                $html .= pages_renderTree($node, $theme, $hidden, $javascript, $folderOnly);
                $html .= '</div>';
            }
            else if(!$folderOnly)
            {
                if($javascript != '')
                {
                    $html .= '<a href="#" onClick="$(\'#'.$javascript.'\').val(\''.$node->page_location.$node->page_name.'\'); return false;">';
                }
                else
                {
                    $html .= '<a class="deleteIcon" href="/'.Config::get('pagedelete', 'pagedelete', 'admin').'&amp;page_id='.$node->page_id.'"></a>';
                    $html .= '<a class="viewIcon" href="'.$node->page_location.$node->page_name.'"></a>';
                    $html .= '<a href="/'.Config::get('pageedit', 'pageedit', 'admin').'&amp;page_id='.$node->page_id.'" >';
                }
                if($node->page_name == 'index')
                {
                    $html .= '<img src="/img/icons/page_home.png" />';
                }
                else
                {
                    $html .= '<img src="/img/icons/page_page.png" />';
                }
                $html .= $node->page_name;
                $html .= '</a>';
            }
            $html .= '</div>';
        }
    }

    return $html;
}