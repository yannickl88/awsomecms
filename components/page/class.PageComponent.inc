<?php
/**
 * The Page component, together with the core component, is this used to show pages and manage them
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PageComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('page');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("page.action_add", "admin");
        $registerManager->registerAuth("page.action_edit", "admin");
        $registerManager->registerAuth("page.action_delete", "admin");
        $registerManager->registerAuth("page.action_loadTree", "admin");

        $registerManager->registerPage(array($this, 'edit'), '{form table="page.pages" form="edit"}');
        $registerManager->registerPage(array($this, 'add'), '{form table="page.pages" form="add"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="page.pages" form="delete"}');
        $registerManager->registerPage("search", '{search}');

        $registerManager->registerURLMap('^\/sitemap\.xml$', 'page.sitemap', 18);
    }
    /**
    * Action for displaying the sitemap
    *
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
    public function action_sitemap($smarty, $smartyLoader, $request)
    {
        $xml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"
xmlns:image="http://www.google.com/schemas/sitemap-image/1.1">

XML;
        //get all the pages;
        $pages = array();
        $url = "http://".$_SERVER['SERVER_NAME'];
        $components = RegisterManager::getInstance()->getComponents();

        foreach($components as $component => $componentData)
        {
            $pages = array_merge($pages, Component::init($component)->getAllPages());
        }

        sort($pages);

        foreach($pages as $page)
        {
            $xml .= "  <url>\n";
            $xml .= "    <loc>".htmlentities($url.$page->loc)."</loc>\n";
            if(!empty($page->images))
            {
                foreach($page->images as $img)
                {
                    $xml .= "    <image:image>\n";
                    $xml .= "      <image:loc>".htmlentities($url.$img)."</image:loc>\n";
                    $xml .= "    </image:image>\n";
                }
            }
            if(!empty($page->lastmod))
            {
                $xml .= "    <lastmod>{$page->lastmod}</lastmod>\n";
            }
            if(!empty($page->changefreq))
            {
                $xml .= "    <changefreq>{$page->changefreq}</changefreq>\n";
            }
            if($page->priority > 0)
            {
                $xml .= "    <priority>{$page->priority}</priority>\n";
            }
            $xml .= "  </url>\n";
        }

        $xml .= <<<XML
</urlset>
XML;
        Controller::getInstance()->setOutput($xml);
    }
    /**
     * Action for loading part of the tree
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_loadTree($smarty, $smartyLoader, $request)
    {
        $this->ajax(Tree::getInstance()->getNodesDataUnder($request['path']));
    }
    /**
     * Action for adding a page
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init('page.pages')
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "edit"), array("page_id" => $id));
        }
    }
    /**
     * Action for editing a page
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('page.pages')
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "edit"), array("page_id" => $request['page_id']));
        }
        else
        {
            if($request['page_id'])
            {
                Table::init('page.pages')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "add"));
            }
        }
    }
    /**
     * Action for deleting a page
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('page.pages')
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(Config::get("location", "admin/", "admin"));
        }
        else
        {
            if($request['page_id'])
            {
                $table = Table::init('page.pages')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "add"));
            }
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("page", ""));
        $menu->addChild(new MenuItem("addpage", array($this, 'add'), "page"));
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerTreeItems($tree)
     */
    public function registerTreeItems($tree)
    {
        $table = Table::init("page.pages", "admintree");

        if($tree->hideAdmin == true)
        {
            $table->setRequest((object) array("where" => array('page_location', "/admin/%", "NOT LIKE")));
        }

        $result = $table
            ->doSelect()
            ->getRows();

        foreach($result as $row)
        {
            $tree->addChild(new PageItem($row));
        }
    }
    /**
     * (non-PHPdoc)
     * @see Component::getAllPages()
     */
    public function getAllPages()
    {
        $table = Table::init("page.pages", "sitemap");
        $table->setRequest((object) array("where" => array('page_location', "/admin/%", "NOT LIKE")));

        $pages = array();
        $result = $table
        ->doSelect()
        ->getRows();

        foreach($result as $row)
        {
            $pages[md5($row->page_location.$row->page_name)] = (object) array(
                "loc" => $row->page_location.$row->page_name,
                "images" => array(),
                "lastmod" => date("c", strtotime($row->page_timestamp)),
                "changefreq" => "",
                "priority" => 0,
            );
        }

        return $pages;
    }
}