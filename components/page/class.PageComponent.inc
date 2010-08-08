<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/libs/class.TextSpider.inc');
import('/core/class.Component.inc');

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
        $registerManager->registerPage('edit', '/admin/page/', '{form table="page.pages" form="edit"}');
        $registerManager->registerPage('add', '/admin/page/', '{form table="page.pages" form="add"}');
        $registerManager->registerPage('delete', '/admin/page/', '{form table="page.pages" form="delete"}');
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
            
            $indexer = new TextSpider();
            $indexer->index($request['page_template'], $request['page_location'].$request['page_name'], $request['page_name'], "page.{$id}");
            
            $this->redirect("/".Config::get('pageedit', 'pageedit', 'admin'), array("page_id" => $id));
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
            
            $indexer = new TextSpider();
            $indexer->index($request['page_template'], $request['page_location'].$request['page_name'], $request['page_name'], "page.{$request['page_id']}");
            
            $this->redirect("/".Config::get('pageedit', 'pageedit', 'admin'), array("page_id" => $request['page_id']));
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
                $this->redirect("/".Config::get('pageadd', 'pageadd', 'admin'));
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
            
            $indexer = new TextSpider();
            $indexer->clearByHook("page.{$request['page_id']}");

            $this->redirect("/admin/");
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
                $this->redirect("/".Config::get('pageadd', 'pageadd', 'admin'));
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
        $menu->addChild(new MenuItem("addpage", Config::get('pageadd', 'pageadd', 'admin'), "page"));
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
}