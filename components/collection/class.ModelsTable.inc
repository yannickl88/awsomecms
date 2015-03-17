<?php
/**
 * Links table, this handels all interaction with the links table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class ModelsTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("models");
        $this->assign("collection");
        $this->lock("model_locked");

        $this->addField(new IntField("model_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new MLangField("model_name"), 1);
        $this->addField(new CategoryField("model_cat"), 2);
        $this->addField(new Field("model_nmr"), 3);
        $this->addField(new IntField("model_year", Field::HIDE_VIEW), 4);
        $this->addField(new DateField("model_date"), 5);
        $scale = new Field("model_scale", Field::HIDE_VIEW);
        $scale->setDefaultValue("1:50");
        $this->addField($scale, 6);
        $this->addField(new MLangField("model_update", Field::OPTIONAL|Field::HIDE_VIEW), 7);
        $this->addField(new UploadField("model_bg", Config::get("uploaddir", "/", "file").'bg/', Field::OPTIONAL), 8);

        $this->addField(new UploadField("model_image", Config::get("uploaddir", "/", "file").'headers/', Field::OPTIONAL), 9);
        $this->addField(new MLangTextField("model_text"), 10);
        $this->addField(new FileSelectField("model_images", array("jpg", "jpeg", "png", "gif"), Field::OPTIONAL|Field::FORCEUPDATE), 11);
        $this->addField(new BooleanField("model_text2_show", Field::HIDE_VIEW), 12);
        $this->addField(new MLangTextField("model_text2", Field::OPTIONAL), 13);
        $this->addField(new FileSelectField("model_images2", array("jpg", "jpeg", "png", "gif"), Field::OPTIONAL|Field::FORCEUPDATE), 14);
        $this->addField(new BooleanField("model_text3_show", Field::HIDE_VIEW), 15);
        $this->addField(new MLangTextField("model_text3", Field::OPTIONAL), 16);
        $this->addField(new FileSelectField("model_images3", array("jpg", "jpeg", "png", "gif"), Field::OPTIONAL|Field::FORCEUPDATE), 17);

        $this->addField(new BooleanField("model_locked", Field::FORCEUPDATE), 18);
        $this->setTitle("[model_name]");

        $this->setOrder('model_name');
        $this->setGrouping('model_locked');
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        if($data)
        {
            $query->whereWhenSet("model_id", $data->model_id);
            $query->whereWhenSet("model_cat1", $data->model_cat1);
            $query->whereWhenSet("model_cat2", $data->model_cat2);
            $query->whereWhenSet("model_cat3", $data->model_cat3);
            $query->whereWhenSet("model_cat4", $data->model_cat4);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table::insert()
     */
    protected function insert($query, $data = null)
    {
        $r = $this->getRecord();
    }
    protected function update($query, $data = null)
    {
        $r = $this->getRecord();
    }
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

            $summary = array("en" => "", "nl" => "");
            $summary["en"] = substr(BBCodeParser::parse($row->model_text['en'], false, false), 0, 100)."...";
            $summary["nl"] = substr(BBCodeParser::parse($row->model_text['nl'], false, false), 0, 100)."...";

            if($query->getAction() == "insert")
                $id = $record->insertID();
            else
            {
                $data = $this->getRecord();
                $id = $data[0]->model_id;
            }

            $totalText = array(
                "en" => $row->model_text['en']. " " . $row->model_text2['en'] . " " . $row->model_text3['en'],
                "nl" => $row->model_text['nl']. " " . $row->model_text2['nl'] . " " . $row->model_text3['nl'],
            );

            $url = Component::init("collection")->makeURL($row);
            $indexer = new TextSpider();
            $indexer->index(
                $totalText,
                $url,
                $row->model_name,
                "model.{$id}",
                $summary
            );
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
            foreach ($data[0]->model_id as $d)
            {
                $indexer->clearByHook("model.{$d}");
            }
        }
        if($query->getAction() == "select") {
            $query->select("model_cat_url");
        }
    }
}