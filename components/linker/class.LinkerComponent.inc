<?php
/**
 * The News component provides a simple news system that allows for new messages.
 * News items can be grouped using a tag if needed.
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class LinkerComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('linker');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("linker.action_add", "admin");
        $registerManager->registerAuth("linker.action_edit", "admin");
        $registerManager->registerAuth("linker.action_delete", "admin");
        $registerManager->registerAuth("linker.action_lcadd", "admin");
        $registerManager->registerAuth("linker.action_lcedit", "admin");
        $registerManager->registerAuth("linker.action_lcdelete", "admin");

        $registerManager->registerPage(array($this, 'admin'), '{form table="linker.links" form="admin"}');
        $registerManager->registerPage(array($this, 'add'), '{form table="linker.links" form="add"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="linker.links" form="delete"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="linker.links" form="edit"}');

        $registerManager->registerPage(array($this, 'lcadmin'), '{form table="linker.lcats" form="admin"}');
        $registerManager->registerPage(array($this, 'lcadd'), '{form table="linker.lcats" form="add"}');
        $registerManager->registerPage(array($this, 'lcdelete'), '{form table="linker.lcats" form="delete"}');
        $registerManager->registerPage(array($this, 'lcedit'), '{form table="linker.lcats" form="edit"}');

        $registerManager->registerURLMap('\/links(\/[a-zA-Z\-\_0-9]*)?$', 'linker.index', 10);
    }
    /**
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_index($smarty, $smartyLoader, $request)
    {
        $smarty->registerResource("scf", $smartyLoader);

        if(isset($request["urlmapmatch"]) && isset($request["urlmapmatch"][1]))
        {
            $lcat = Table::init('linker.lcats')
                ->setRequest((object) array("lcat_urlname" => substr($request["urlmapmatch"][1], 1)))
                ->doSelect()
                ->getRow();

            if($lcat)
            {
                if(!empty($lcat->lcat_bg))
                    $smarty->assign("BGIMG", $lcat->lcat_bg->url);

                $links = Table::init('linker.links')
                    ->setRequest((object) array("link_cat" => $lcat->lcat_id, "hideHidden" => true ))
                    ->doSelect()
                    ->getRows();

                foreach($links as &$link)
                {
                    $link->link_description = $link->link_description[getLang()];

                    $link->lcat_name = unserialize($link->lcat_name);
                    $link->lcat_name = $link->lcat_name[getLang()];
                    $link->lcat_description = unserialize($link->lcat_description);
                    $link->lcat_description = $link->lcat_description[getLang()];
                }
                uasort($links, array($this, "linkSort"));

                $smarty->assign("links", $links);
                $smarty->assign("TITLE", $lcat->lcat_name);
                Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_linkcat.tpl"), $_GET['url'], $smarty));
                return;
            }
        }
        else
        {
            $links = Table::init('linker.lcats')
                ->setRequest((object) array("hideHidden" => true ))
                ->doSelect()
                ->getRows();

            foreach($links as &$link)
            {
                $link->lcat_name = $link->lcat_name[getLang()];
                $link->lcat_description = $link->lcat_description[getLang()];
            }
            uasort($links, array($this, "lcatSort"));

            $smarty->assign("links", $links);
            $smarty->assign("TITLE", "links");
            $smarty->assign("content", $smarty->fetch("scf:{$_GET['url']}"));
            Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_links.tpl"), $_GET['url'], $smarty));
            return;
        }

        Controller::getInstance()->setOutput($smartyLoader->getPage($_GET['url'], $smarty));
    }

    private function lcatSort($a, $b)
    {
        return strcmp($a->lcat_name, $b->lcat_name);
    }

    private function linkSort($a, $b)
    {
        return strcmp($a->link_title, $b->link_title);
    }

    /**
     * Action for adding a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init("linker.links")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            Table::init('linker.links')
                ->setRecord((object) array("link_code" => makeRandomString(10)));
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
            Table::init("linker.links")
                ->setRecord((object) $request)
                ->doUpdate();

            Cache::clear("link_".$request["link_code"]);

            $this->redirect(array($this, "admin"));
        }
        else
        {

            if(!empty($request['link_id']))
            {
                Table::init('linker.links')
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
            Table::init("linker.links")
                ->setRecord((object) $request)
                ->doDelete();

            Cache::clear("link_".$request["link_code"]);

            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['link_id'])
            {
                Table::init('linker.links')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Action for adding a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_lcadd($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init("linker.lcats")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "lcadmin"));
        }
    }
    /**
     * Action for editing a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_lcedit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("linker.lcats")
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "lcadmin"));
        }
        else
        {

            if(!empty($request['lcat_id']))
            {
                Table::init('linker.lcats')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "lcadmin"));
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
    public function action_lcdelete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("linker.lcats")
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(array($this, "lcadmin"));
        }
        else
        {
            if($request['lcat_id'])
            {
                Table::init('linker.lcats')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "lcadmin"));
            }
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("links", array($this, 'admin')));
        $menu->addChild(new MenuItem("links", array($this, 'admin'), "links"));
        $menu->addChild(new MenuItem("categories", array($this, 'lcadmin'), "links"));
        $menu->addChild(new MenuItem("addlink", array($this, 'add'), "links"));
    }
    /**
    * (non-PHPdoc)
    * @see Component::getAllPages()
    */
    public function getAllPages()
    {
        $result = Table::init('linker.lcats')
            ->setRequest((object) array("hideHidden" => true ))
            ->doSelect()
            ->getRows();

        $pages = array();
        foreach($result as $row)
        {
            $pages[md5('/links/'.$row->lcat_urlname)] = (object) array(
                "loc" => '/links/'.$row->lcat_urlname,
                "images" => array(),
                "lastmod" => "",
                "changefreq" => "",
                "priority" => 0,
            );
        }

        return $pages;
    }
}