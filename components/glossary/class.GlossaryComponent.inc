<?php
/**
 * The News component provides a simple news system that allows for new messages.
 * News items can be grouped using a tag if needed.
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GlossaryComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('glossary');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("glossary.action_add", "admin");
        $registerManager->registerAuth("glossary.action_edit", "admin");
        $registerManager->registerAuth("glossary.action_delete", "admin");

        $registerManager->registerPage(array($this, 'admin'), '{form table="glossary.glossary" form="admin"}');
        $registerManager->registerPage(array($this, 'add'), '{form table="glossary.glossary" form="add"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="glossary.glossary" form="delete"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="glossary.glossary" form="edit"}');

        $registerManager->registerURLMap('\/abc$', 'glossary.index', 10);
    }
    /**
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_index($smarty, $smartyLoader, $request)
    {
        $smarty->registerResource("scf", $smartyLoader);

        $glossary1 = array();
        $glossary2 = array();
        $items = Table::init('glossary.glossary')
            ->doSelect()
            ->getRows();

        foreach($items as $item)
        {
            $firstLetter = strtolower(substr($item->glossary_title[getLang()], 0, 1));

            if(strcmp($firstLetter, "m") < 1)
            {
                if(!isset($glossary1[$firstLetter]))
                    $glossary1[$firstLetter] = array();

                $glossary1[$firstLetter][] = (object) array(
                    "code" => $item->glossary_code,
                    "title" => $item->glossary_title[getLang()],
                    "description" => $item->glossary_description[getLang()]
                );
            }
            else
            {
                if(!isset($glossary2[$firstLetter]))
                    $glossary2[$firstLetter] = array();

                $glossary2[$firstLetter][] = (object) array(
                    "code" => $item->glossary_code,
                    "title" => $item->glossary_title[getLang()],
                    "description" => $item->glossary_description[getLang()]
                );
            }
        }

        foreach($glossary1 as &$gitem)
        {
            uasort($gitem, array($this, "glossarySort"));
        }

        foreach($glossary2 as &$gitem)
        {
            uasort($gitem, array($this, "glossarySort"));
        }

        ksort($glossary1);
        ksort($glossary2);

        $smarty->assign("glossary1", $glossary1);
        $smarty->assign("glossary2", $glossary2);
        $smarty->assign("TITLE", "abc");
        $smarty->assign("content", $smarty->fetch("scf:/abc"));

        Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_abc.tpl"), '/abc', $smarty));
    }

    private function glossarySort($a, $b)
    {
        return strcmp($a->title, $b->title);
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
            $id = Table::init("glossary.glossary")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            Table::init('glossary.glossary')
            ->setRecord((object) array("glossary_code" => makeRandomString(10)));
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
            Table::init("glossary.glossary")
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "admin"));
        }
        else
        {

            if(!empty($request['glossary_id']))
            {
                Table::init('glossary.glossary')
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
            Table::init("glossary.glossary")
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['glossary_id'])
            {
                Table::init('glossary.glossary')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("glossary", array($this, 'admin')));
        $menu->addChild(new MenuItem("glossary", array($this, 'admin'), "glossary"));
        $menu->addChild(new MenuItem("addGlossary", array($this, 'add'), "glossary"));
    }
}