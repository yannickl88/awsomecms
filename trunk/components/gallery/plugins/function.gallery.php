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
    if(isset($params['name']))
    {
        $data['gallery_name'] = $params['name'];
    }
    else
    {
        return;
    }

    $gallery = Table::init('gallery.gallery')
        ->setRequest((object) $data)
        ->doSelect()
        ->getRow();
    
    if($gallery)
    {
        $images = Table::init("file.files")
            ->setRequest((object) array_merge($data, array("file_id" => $gallery->gallery_images)))
            ->doSelect()
            ->getRows();
        
        $gallery->images = $images;
            
        $smarty->assign("gallery", $gallery);
        
        return $smarty->fetch("gallery/gallery.tpl");
    }
}