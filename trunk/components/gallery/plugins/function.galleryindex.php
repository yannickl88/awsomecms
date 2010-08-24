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
 * Show a list of galleries
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_galleryindex($params, &$smarty)
{
    $data = array();

    $galleries = Table::init('gallery.gallery')
        ->doSelect()
        ->getRows();
    
    if(isset($params['widthImages']) && $params['widthImages'] == true)
    {
        foreach($galleries as &$gallery)
        {
            $images = Table::init("file.files")
                ->setRequest((object) array_merge($data, array("file_id" => $gallery->gallery_images, "file_tag" => array($gallery->gallery_tags, "OR"))))
                ->doSelect()
                ->getRows();
            
            $gallery->images = $images;
        }
    }
        
    $smarty->assign("galleries", $galleries);
    return $smarty->fetch("gallery/gallery_index.tpl");
}