<?php
/**
 * Links table, this handels all interaction with the links table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class LinksTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("links");
        $this->assign("linker");

        $this->addField(new IntField("link_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("link_code"), 1);
        $this->addField(new Field("link_title"), 2);
        $this->addField(new LinkcatField("link_cat"), 3);
        $this->addField(new Field("link_url", Field::OPTIONAL|Field::FORCEUPDATE), 4);
        $this->addField(new LocationField("link_page", Field::OPTIONAL|Field::FORCEUPDATE, true), 5);
        $this->addField(new MLangTextField("link_description", Field::OPTIONAL|Field::FORCEUPDATE), 6);

        $iconField = new UploadField("link_logo", Config::get("uploaddir", "/", "file")."logos/");
        $iconField->setResize(array(50, 50));
        $this->addField($iconField, 7);
        $this->addField(new BooleanField("link_hidden"), 8);
        $this->addField(new BooleanField("link_highlight", Field::HIDE_VIEW), 9);
        $this->setTitle("[link_title]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     *
     * @param SQLQuery $query
     */
    protected function select($query, $data = null)
    {
        $query->orderby('link_title', "ASC");

        //join on the categories
        $query->join("lcats", "link_cat", "lcat_id");
        $query->select("lcat_name");
        $query->select("lcat_urlname");
        $query->select("lcat_description");

        if($data)
        {
            if(isset($data->hideHidden) && $data->hideHidden)
                $query->where("link_hidden", 0, "=", "AND", true);

            $query->whereWhenSet("link_cat", $data->link_cat);
            $query->whereWhenSet("link_id", $data->link_id);
            $query->whereWhenSet("link_code", $data->link_code);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table::insert()
     */
    protected function insert($query, $data = null)
    {

    }
    protected function update($query, $data = null) {}
    protected function delete($query, $data = null) {}

    /**
     * (non-PHPdoc)
     * @see core/Table::postSQL()
     *
     * @param SQLRecord $record
     * @param SQLQuery $query
     */
    protected function postSQL(&$record, $query)
    {
        if($query->getAction() == "update" || $query->getAction() == "insert")
        {
            $row = $record->getRow();
            $dbRow = $this->doSelect()->getRow();

            if($dbRow->link_hidden === "0")
            {
                $summary = array("en" => "", "nl" => "");
                $summary["en"] = substr(BBCodeParser::parse($row->link_description['en'], false, false), 0, 100)."...";
                $summary["nl"] = substr(BBCodeParser::parse($row->link_description['nl'], false, false), 0, 100)."...";

                if($query->getAction() == "insert")
                {
                    $id = $record->insertID();
                }
                else
                {
                    $data = $this->getRecord();
                    $id = $data[0]->link_id;
                }

                $indexer = new TextSpider();
                $title = array("en" => $row->link_title, "nl" => $row->link_title);
                $priority = $row->link_highlight == 0 ? 0 : 1;

                $indexer->index(
                        $row->link_description,
                        "/links/" . $dbRow->lcat_urlname . "#". $row->link_code,
                        $title,
                        "link.{$id}",
                        $summary,
                        $priority
                );
            }
            else
            {
                $data = $this->getRecord();

                $indexer = new TextSpider();
                $indexer->clearByHook("link.{$data[0]->link_id}");
            }
        }
    }

    /**
     * (non-PHPdoc)
     * @see core/Table::preSQL()
     *
     * @param SQLRecord $record
     * @param SQLQuery $query
     */
    protected function preSQL(&$record, $query)
    {
        if($query->getAction() == "delete")
        {
            $data = $this->getRecord();

            $indexer = new TextSpider();
            foreach ($data[0]->link_id as $d)
                $indexer->clearByHook("link.{$d}");
        }
    }
}