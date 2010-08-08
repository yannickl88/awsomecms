<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.gallery
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 * @TODO refactor this component into a more all purpose file component
 */
import('/core/class.Component.inc');

/**
 * A simple component to manage and show a series of images on a page. Using a tag, images can be grouped together
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GalleryComponent extends Component
{
    /**
     * Constructor
     * 
     * @return unknown_type
     */
    public function __construct()
    {
        parent::__construct('gallery');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerPage('add', '/admin/gallery/', '{form table="gallery.gallery" form="add"}');
        $registerManager->registerPage('edit', '/admin/gallery/', '{form table="gallery.gallery" form="edit"}');
        $registerManager->registerPage('delete', '/admin/gallery/', '{form table="gallery.gallery" form="delete"}');
        $registerManager->registerPage('admin', '/admin/gallery/', '{form table="gallery.gallery" form="admin"}');
    }
    /**
     * Action to add an image
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("gallery.gallery")
                ->setRecord((object) $request)
                ->doInsert();
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
    }
    /**
     * Action to edit an image
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("gallery.gallery")
                ->setRecord((object) $request)
                ->doUpdate();
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['gallery_id'])
            {
                $record = Table::init("gallery.gallery")
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRow();
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    /**
     * Action to delete an image
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("gallery.gallery")
                ->setRequest((object) $request)
                ->doDelete();
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['gallery_id'])
            {
                $record = Table::init("gallery.gallery")
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRows();
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    /**
     * Function for registering menu items to the admin menu
     *
     * @param Menu $menu
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("gallery", Config::get('galleryadmin', 'galleryadmin', 'admin')));
        $menu->addChild(new MenuItem("galleries", Config::get('galleryadmin', 'galleryadmin', 'admin'), "gallery"));
    }
}