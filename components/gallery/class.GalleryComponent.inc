<?php
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
        $registerManager->registerAuth("gallery.action_add", "admin");
        $registerManager->registerAuth("gallery.action_edit", "admin");
        $registerManager->registerAuth("gallery.action_delete", "admin");

        $registerManager->registerPage(array($this, 'add'), '{form table="gallery.gallery" form="add"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="gallery.gallery" form="edit"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="gallery.gallery" form="delete"}');
        $registerManager->registerPage(array($this, 'admin'), '{form table="gallery.gallery" form="admin"}');
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
            $this->redirect(array($this, "admin"));
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

            $this->redirect(array($this, "admin"));
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
                $this->redirect(array($this, "admin"));
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

            $this->redirect(array($this, "admin"));
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
                $this->redirect(array($this, "admin"));
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
        $menu->addChild(new MenuItem("gallery", array($this, 'admin')));
        $menu->addChild(new MenuItem("galleries", array($this, 'admin'), "gallery"));
    }
}