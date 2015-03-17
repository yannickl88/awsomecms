<?php
/**
 * The News component provides a simple news system that allows for new messages.
 * News items can be grouped using a tag if needed.
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class CollectionComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('collection');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("collection.action_admin", "admin");
        $registerManager->registerAuth("collection.action_add", "admin");
        $registerManager->registerAuth("collection.action_edit", "admin");
        $registerManager->registerAuth("collection.action_delete", "admin");
        $registerManager->registerAuth("collection.action_mcadd", "admin");
        $registerManager->registerAuth("collection.action_mcedit", "admin");
        $registerManager->registerAuth("collection.action_mcdelete", "admin");
        $registerManager->registerAuth("collection.action_mcadmin", "admin");

        $registerManager->registerPage(array($this, 'admin'), '{form table="collection.models" form="admin"}');
        $registerManager->registerPage(array($this, 'add'), '{form table="collection.models" form="add"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="collection.models" form="delete"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="collection.models" form="edit"}');

        $registerManager->registerPage(array($this, 'mcadmin'), '{form table="collection.mcats" form="admin"}');
        $registerManager->registerPage(array($this, 'mcedit'), '{form table="collection.mcats" form="edit"}');
        $registerManager->registerPage(array($this, 'mcdelete'), '{form table="collection.mcats" form="delete"}');

        $registerManager->registerURLMap('^\/collection(\/[a-zA-Z\-\_0-9]*)?(\/[a-zA-Z\-\_0-9]*)?(\/[a-zA-Z\-\_0-9]*)?(\/[a-zA-Z\-\_0-9]*)?$', 'collection.overview', 18);
        $registerManager->registerURLMap('^\/updates(\/([0-9]*))?$', 'collection.updates', 18);
    }

    /**
     * Action for adding a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_overview($smarty, $smartyLoader, $request)
    {
        $smarty->registerResource("scf", $smartyLoader);

        if(!empty($request['urlmapmatch']))
        {
            $count = count($request['urlmapmatch']);
            if($count == 5)
            {
                //fetch parents
                $parents = SQLQuery::doSelect("c1.mcat_id AS mcat1_id")
                    ->select("c2.mcat_id AS mcat2_id")
                    ->select("c3.mcat_id AS mcat3_id")
                    ->select("c4.mcat_id AS mcat4_id")
                    ->select("c1.mcat_name AS mcat1_name")
                    ->select("c2.mcat_name AS mcat2_name")
                    ->select("c3.mcat_name AS mcat3_name")
                    ->select("c4.mcat_name AS mcat4_name")
                    ->select("c1.mcat_urlname AS mcat1_uname")
                    ->select("c2.mcat_urlname AS mcat2_uname")
                    ->select("c3.mcat_urlname AS mcat3_uname")
                    ->select("c4.mcat_urlname AS mcat4_uname")
                    ->table("mcats", "c1")
                    ->table("mcats", "c2")
                    ->table("mcats", "c3")
                    ->table("mcats", "c4")
                    ->where("c1.mcat_urlname", substr($request['urlmapmatch'][4], 1),"=", "AND", 2)
                    ->where("c2.mcat_urlname", substr($request['urlmapmatch'][3], 1),"=", "AND", 2)
                    ->where("c3.mcat_urlname", substr($request['urlmapmatch'][2], 1),"=", "AND", 2)
                    ->where("c4.mcat_urlname", substr($request['urlmapmatch'][1], 1),"=", "AND", 2)
                    ->where("c1.mcat_type", 4)
                    ->where("c2.mcat_type", 3)
                    ->where("c3.mcat_type", 2)
                    ->where("c4.mcat_type", 1)
                    ->exec()
                    ->getRow();
                if(!$parents)
                {
                    Controller::getInstance()->setOutput($smartyLoader->getPage($_GET['url'], $smarty));
                    return "";
                }
                $parents->mcat1_name = unserialize($parents->mcat1_name);
                $parents->mcat2_name = unserialize($parents->mcat2_name);
                $parents->mcat3_name = unserialize($parents->mcat3_name);
                $parents->mcat4_name = unserialize($parents->mcat4_name);

                $item = Table::init("collection.models")
                    ->setRequest((object) array(
                            "model_cat1" => $parents->mcat4_id,
                            "model_cat2" => $parents->mcat3_id,
                            "model_cat3" => $parents->mcat2_id,
                            "model_cat4" => $parents->mcat1_id,
                        ))
                    ->doSelect()
                    ->getRow();
                if(!empty($item->model_images))
                {
                    $images1 = Table::init("file.files")
                        ->setRequest((object) array("file_id" => $item->model_images))
                        ->doSelect()
                        ->getRows();
                    usort($images1, array($this, 'sortImages'));
                }
                else
                    $images1 = false;
                if(!empty($item->model_images2))
                {
                    $images2 = Table::init("file.files")
                        ->setRequest((object) array("file_id" => $item->model_images2))
                        ->doSelect()
                        ->getRows();
                    usort($images2, array($this, 'sortImages'));
                }
                else
                    $images2 = false;
                if(!empty($item->model_images3))
                {
                    $images3 = Table::init("file.files")
                        ->setRequest((object) array("file_id" => $item->model_images3))
                        ->doSelect()
                        ->getRows();
                    usort($images3, array($this, 'sortImages'));
                }
                else
                    $images3 = false;


                $smarty->assign("NOPADDIND", true);
                $smarty->assign("TITLE", $item->model_name);
                $smarty->assign("parents", $parents);
                $smarty->assign("item", $item);
                $smarty->assign("imageHeader", $item->model_image->url);
                $smarty->assign("images1", $images1);
                $smarty->assign("images2", $images2);
                $smarty->assign("images3", $images3);

                $fbData = (object) array(
                    "title" => $item->model_name[getLang()],
                    "image" => 'http://' . $_SERVER['SERVER_NAME'] . $item->model_image->url,
                    "url" => 'http://' . $_SERVER['SERVER_NAME'] . '/' . $item->model_cat_url,
                    "description" => substr($item->model_text[getLang()], 0, 200)."...",
                );

                Config::getInstance()->set("facebookData", $fbData);

                if(!empty($item->model_bg)) {
                    $smarty->assign("BGIMG", $item->model_bg->url);
                }

                Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_cmodel.tpl"), $_GET['url'], $smarty));
            }
            else if($count > 1 && $count <= 4)
            {
                $query = SQLQuery::doSelect("c1.mcat_id AS mcat1_id")
                    ->select("c1.mcat_name AS mcat1_name")
                    ->select("c1.mcat_description AS mcat1_desc")
                    ->select("c1.mcat_image AS mcat1_img")
                    ->select("c1.mcat_urlname AS mcat1_urlname")
                    ->table("mcats", "c1")
                    ->where("c1.mcat_urlname", substr($request['urlmapmatch'][1], 1),"=", "AND", 2)
                    ->where("c1.mcat_type", 1);
                if($count >= 3)
                {
                    $query->select("c2.mcat_id AS mcat2_id")
                        ->select("c2.mcat_name AS mcat2_name")
                        ->select("c2.mcat_description AS mcat2_desc")
                        ->select("c2.mcat_image AS mcat2_img")
                        ->select("c2.mcat_urlname AS mcat2_urlname")
                        ->table("mcats", "c2")
                        ->where("c2.mcat_urlname", substr($request['urlmapmatch'][2], 1),"=", "AND", 2)
                        ->where("c2.mcat_type", 2);

                }
                if($count >= 4)
                {
                    $query->select("c3.mcat_id AS mcat3_id")
                    ->select("c3.mcat_name AS mcat3_name")
                    ->select("c3.mcat_description AS mcat3_desc")
                    ->select("c3.mcat_image AS mcat3_img")
                    ->select("c3.mcat_urlname AS mcat3_urlname")
                    ->table("mcats", "c3")
                    ->where("c3.mcat_urlname", substr($request['urlmapmatch'][3], 1),"=", "AND", 2)
                    ->where("c3.mcat_type", 3);
                }
                $parents = $query->exec()
                    ->getRow();

                if(!$parents)
                {
                    Controller::getInstance()->setOutput($smartyLoader->getPage($_GET['url'], $smarty));
                    return "";
                }

                $parentIDs = array();
                for($i = 1; $i < $count; $i++)
                {
                    $parentIDs[] = $parents->{'mcat'.($i).'_id'};
                }
                $parent = (object) array(
                    'mcat_id' => (int) $parents->{'mcat'.($count - 1).'_id'},
                    'mcat_name' => unserialize($parents->{'mcat'.($count - 1).'_name'}),
                    'mcat_description' => unserialize($parents->{'mcat'.($count - 1).'_desc'}),
                    'mcat_image' => unserialize($parents->{'mcat'.($count - 1).'_img'}),
                );


                if($count == 4)
                {
                    $groups = SQLQuery::doSelect("c.*")
                    ->table(Table::init("collection.models"), "m")
                    ->table(Table::init("collection.mcats"), "c")
                    ->where("m.model_cat1", $parentIDs[0])
                    ->where("m.model_cat2", $parentIDs[1])
                    ->where("m.model_cat3", $parentIDs[2])
                    ->where("m.model_cat4", "c.mcat_id", "=", "AND", true)
                    ->exec()
                    ->getRows();

                    usort($groups, array($this, "sortItems"));
                }
                else if($count == 3)
                {
                    //var_dump($parent, $parents);
                    $items = SQLQuery::doSelect("c1.mcat_urlname AS mcat_parent_urlname")
                        ->select("c1.mcat_name AS mcat_parent_name")
                        ->select("c2.mcat_urlname AS mcat_urlname")
                        ->select("c2.mcat_name AS mcat_name")
                        ->table(Table::init("collection.models"), "m")
                        ->table(Table::init("collection.mcats"), "c1")
                        ->table("mcats", "c2")
                        ->where("m.model_cat1", $parentIDs[0])
                        ->where("m.model_cat2", $parentIDs[1])
                        ->where("m.model_cat3", "c1.mcat_id", "=", "AND", true)
                        ->where("m.model_cat4", "c2.mcat_id", "=", "AND", true)
                        ->exec()
                        ->getRows();

                    $groups = array();
                    $index = array();

                    foreach ($items as $item)
                    {
                        if(!isset($groups[$item->mcat_parent_urlname]))
                        {
                            $groups[$item->mcat_parent_urlname] = (object) array(
                                'mcat_name' => unserialize($item->mcat_parent_name),
                                'mcat_urlname' => $item->mcat_parent_urlname,
                                'children' => array()
                            );
                        }
                        $groups[$item->mcat_parent_urlname]->children[] = $item;
                    }

                    usort($groups, array($this, "sortGroups"));

                    foreach ($groups as &$group)
                    {
                        usort($group->children, array($this, "sortItems"));
                    }
                }
                else
                {
                    $groups = SQLQuery::doSelect("c.*")
                        ->table(Table::init("collection.models"), "m")
                        ->table(Table::init("collection.mcats"), "c")
                        ->where("m.model_cat1", $parentIDs[0])
                        ->where("m.model_cat2", "c.mcat_id", "=", "AND", true)
                        ->groupBy("m.model_cat2")
                        ->exec()
                        ->getRows();

                    usort($groups, array($this, "sortItems"));
                }

                $smarty->assign("items", $groups);
                $smarty->assign("parents", $parents);

                $smarty->assign("type", $count);
                $smarty->assign("parent", $parent);
                $smarty->assign("TITLE", $parent->mcat_name);
                $smarty->assign("BGIMG", $parent->mcat_image->url);

                Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_coverview.tpl"), $_GET['url'], $smarty));
            }
            else
            {
                $items = Table::init("collection.mcats")
                    ->setRequest((object) array("mcat_type" => 1))
                    ->doSelect()
                    ->getRows();

                usort($items, array($this, "sortItems"));

                $smarty->assign("type", $count);
                $smarty->assign("TITLE", 'collection');
                $smarty->assign("content", $smarty->fetch("scf:{$_GET['url']}"));
                $smarty->assign("items", $items);

                Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_cindex.tpl"), $_GET['url'], $smarty));
            }
            return;
        }

        Controller::getInstance()->setOutput($smartyLoader->getPage($_GET['url'], $smarty));
    }
    private function sortGroups($a, $b)
    {
        return strnatcasecmp($a->mcat_name[getLang()], $b->mcat_name[getLang()]);
    }
    private function sortItems($a, $b)
    {
        return strnatcasecmp($a->mcat_name[getLang()], $b->mcat_name[getLang()]);
    }
    private function sortImages($a, $b)
    {
        return strnatcasecmp($a->file_name, $b->file_name);
    }
    /**
     * Action for adding a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_updates($smarty, $smartyLoader, $request)
    {
        $smarty->registerResource("scf", $smartyLoader);

        $ppp = 15;
        $page = (isset($request['urlmapmatch'][2]) && $request['urlmapmatch'][2] > 0 ? (int) $request['urlmapmatch'][2]: 1 ) - 1;

        $updates = array();
        $data = SQLQuery::doSelect("m.*")
            ->select("c.mcat_name")
            ->table(Table::init("collection.models"), "m")
            ->table(Table::init("collection.mcats"), "c")
            ->where("c.mcat_id", "m.model_cat4", "=", "AND", true)
            ->orderBy("m.model_date", "DESC")
            ->limit($ppp, $page * $ppp)
            ->exec();

        $bgimgs = array();

        while($update = $data->getRow())
        {
            if(!isset($updates[$update->model_date]))
                $updates[$update->model_date] = (object) array("date" => $update->model_date, "children" => array());

            //check if update text exists
            $update->hadUpdateText = !empty($update->model_update[getLang()]);

            array_push($updates[$update->model_date]->children, $update);
            if(count($bgimgs) < 5)
                array_push($bgimgs, $update->model_image);
        }

        $count = SQLQuery::doSelect("COUNT(model_id) AS total")
            ->table(Table::init("collection.models"))
            ->exec()
            ->getRow()
            ->total;

        $smarty->assign("TITLE", 'updates');

        $smarty->assign("updates", $updates);
        $smarty->assign("currentPage", $page + 1);
        $smarty->assign("totalPosts", $count);
        $smarty->assign("postsPerPage", $ppp);
        $smarty->assign("content", $smarty->fetch("scf:updates"));

        Controller::getInstance()->setOutput($smartyLoader->getRenderedPage($smarty->fetch("_allupdates.tpl"), $_GET['url'], $smarty));
        return;
    }

    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init("collection.models")
                ->setRecord((object) $request)
                ->doInsert()
                ->insertID();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            Table::init('collection.models')
                ->setRecord((object) array("link_code" => makeRandomString(10)));
        }
    }
    /**
     * Action for unlocking a news item
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_disabled($smarty, $smartyLoader, $request)
    {
        $locked = (isset($request["model_locked"]) && $request["model_locked"] === "1");

        if(!empty($request['model_id']))
        {
            SQLQuery::doUpdate("model_locked", $locked)
                ->table(Table::init("collection.models"))
                ->where("model_id", $request['model_id'])
                ->exec();
        }
        $this->redirect(array($this, "admin"));
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
            Table::init("collection.models")
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            if(!empty($request['model_id']))
            {
                Table::init('collection.models')
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
            Table::init("collection.models")
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['model_id'])
            {
                Table::init('collection.models')
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
    public function action_mcadd($smarty, $smartyLoader, $request)
    {
        $table = Table::init("collection.mcats");
        if($request['method'] == 'post')
        {
            try {
                $id = $table->setRecord((object) $request)
                    ->doInsert()
                    ->insertID();
            }
            catch (InvalidFormException $e)
            {
                var_dump($e);
            }

            $this->ajax(array(
                "id" => $id,
                "name" => $table->getField("mcat_name")->toHTML(Field::VIEW),
                "type" => $request["mcat_type"],
                "parent" => $request["mcat_parent"]
            ));
        }
    }

    public function action_mcedit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("collection.mcats")
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect(array($this, "mcadmin"));
        }
        else
        {

            if(!empty($request['mcat_id']))
            {
                Table::init('collection.mcats')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "mcadmin"));
            }
        }
    }

    public function action_mcdelete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("collection.mcats")
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect(array($this, "mcadmin"));
        }
        else
        {
            if($request['mcat_id'])
            {
                Table::init('collection.mcats')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "mcadmin"));
            }
        }
    }

    public function makeURL($record)
    {
        $parents = Table::init("collection.mcats")
            ->setRecord((object) array("mcat_id" => array($record->model_cat1, $record->model_cat2, $record->model_cat3, $record->model_cat4)))
            ->doSelect();
        $url = "collection";

        while($row = $parents->getRow())
        {
            $url .= "/".$row->mcat_urlname;
        }
        return $url;
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("collection", array($this, 'admin')));
        $menu->addChild(new MenuItem("models", array($this, 'admin'), "collection"));
        $menu->addChild(new MenuItem("overview", array($this, 'mcadmin'), "collection"));
    }
    /**
    * (non-PHPdoc)
    * @see Component::getAllPages()
    */
    public function getAllPages()
    {
        $result = SQLQuery::doSelect()
            ->table(Table::init('collection.models'))
            ->exec()
            ->getRows();

        $pages = array();
        foreach($result as $row)
        {
            //check for images
            $images = array();
            $imagesData = Table::init("file.files")
                ->setRequest((object) array("file_id" => array_merge($row->model_images, $row->model_images2, $row->model_images3)))
                ->doSelect()
                ->getRows();

            foreach($imagesData as $image)
            {
                $images[] = $image->file_data->url;
            }

            $pages[md5('/'.$row->model_cat_url)] = (object) array(
                "loc" => '/'.$row->model_cat_url,
                "images" => $images,
                "lastmod" => "",
                "changefreq" => "",
                "priority" => 0,
            );

            //categories
            $type = substr($row->model_cat_url, 0, strrpos($row->model_cat_url, "/"));
            $model = substr($type, 0, strrpos($type, "/"));
            $machine = substr($model, 0, strrpos($model, "/"));

            $pages[md5('/'.$type)] = (object) array(
                "loc" => '/'.$type,
                "images" => array(),
                "lastmod" => "",
                "changefreq" => "",
                "priority" => 0,
            );
            $pages[md5('/'.$model)] = (object) array(
                "loc" => '/'.$model,
                "images" => array(),
                "lastmod" => "",
                "changefreq" => "",
                "priority" => 0,
            );
            $pages[md5('/'.$machine)] = (object) array(
                "loc" => '/'.$machine,
                "images" => array(),
                "lastmod" => "",
                "changefreq" => "",
                "priority" => 0,
            );
        }

        return $pages;
    }
}