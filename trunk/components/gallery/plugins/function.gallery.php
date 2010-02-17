<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.gallery.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Show the gallery on the page
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_gallery($params, &$smarty)
{
    $data = array();

    if(isset($params['max']))
    {
        $data['max'] = $params['max'];
    }
    if(isset($params['tag']))
    {
        $data['image_tag'] = $params['tag'];
    }

    $smarty->assign("gallery", Table::init('gallery.images')->setRequest((object) $data)->doSelect()->getRows());

    return $smarty->fetch("gallery/gallery.tpl");
}