<?php
/**
 * Show images with title and link in a grid
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class ImagegridComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('imagegrid');
    }

    /**
     * @see Component::register()
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("imagegrid.action_add", "admin");
        $registerManager->registerAuth("imagegrid.action_edit", "admin");
        $registerManager->registerAuth("imagegrid.action_delete", "admin");

        $registerManager->registerAuth("imagegrid.action_addblock", "admin");
        $registerManager->registerAuth("imagegrid.action_editblock", "admin");
        $registerManager->registerAuth("imagegrid.action_deleteblock", "admin");

        $registerManager->registerPage(array($this, 'add'), '{form table="imagegrid.imagegrid" form="add"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="imagegrid.imagegrid" form="edit"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="imagegrid.imagegrid" form="delete"}');
        $registerManager->registerPage(array($this, 'admin'), '{form table="imagegrid.imagegrid" form="admin"}');

        $registerManager->registerPage(array($this, 'addblock'), '{form table="imagegrid.block" form="add"}');
        $registerManager->registerPage(array($this, 'editblock'), '{form table="imagegrid.block" form="edit"}');
        $registerManager->registerPage(array($this, 'deleteblock'), '{form table="imagegrid.block" form="delete"}');
        $registerManager->registerPage(array($this, 'adminblock'), '{form table="imagegrid.block" form="admin"}');
    }

    /**
     * @see Component::registerMenuItems()
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("imagegrid", array($this, 'admin')));
        $menu->addChild(new MenuItem("grids", array($this, 'admin'), "imagegrid"));
        $menu->addChild(new MenuItem("blocks", array($this, 'adminblock'), "imagegrid"));
    }

    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init("imagegrid.imagegrid")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "admin"));
        }
    }
    /**
     * Action for editing a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("imagegrid.imagegrid")
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "admin"));
        }
        else
        {

            if(!empty($request['grid_id']))
            {
                Table::init('imagegrid.imagegrid')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Action for deleting a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("imagegrid.imagegrid")
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['grid_id'])
            {
                Table::init('imagegrid.imagegrid')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }

    public function action_addblock($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init("imagegrid.block")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "adminblock"));
        }
    }
    /**
     * Action for editing a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_editblock($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("imagegrid.block")
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "adminblock"));
        }
        else
        {
            if(!empty($request['block_id']))
            {
                Table::init('imagegrid.block')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "adminblock"));
            }
        }
    }
    /**
     * Action for deleting a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_deleteblock($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("imagegrid.block")
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(array($this, "adminblock"));
        }
        else
        {
            if($request['block_id'])
            {
                Table::init('imagegrid.block')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "adminblock"));
            }
        }
    }
}