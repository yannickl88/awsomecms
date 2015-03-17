<?php
/**
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PagesTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("pages");
        $this->assign("page");

        $this->addField(new IntField("page_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("page_name"), 1);
        $this->addField(new LocationField("page_location", 0, false, 1, true), 2);
        $this->addField(new MLangTextField("page_template"), 3);
        $this->addField(new DateField("page_timestamp", Field::HIDE));
        $this->addField(new BooleanField("page_html", Field::HIDE|Field::OPTIONAL));
        $this->addField(new UploadField("page_bg", Config::get("uploaddir", "/", "file").'bg/', Field::OPTIONAL), 4);
        $this->addField(new LocationField("page_slideshow", Field::OPTIONAL|Field::FORCEUPDATE, false, 2), 4);
        $this->addField(new BooleanField("page_hide"), 5);
        $this->addField(new BooleanField("page_noindex", Field::HIDE|Field::OPTIONAL|Field::DUMMY|Field::FORCESELECT), 5);
        $this->addField(new BooleanField("page_highlight", Field::HIDE_VIEW), 6);
        $this->setTitle("[page_name]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('page_name');

        if($data)
        {
            $query->whereWhenSet('page_name', $data->page_name);
            $query->whereWhenSet('page_location', $data->page_location);
            $query->whereWhenSet($data->where[0], $data->where[1], $data->where[2]);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#update($query, $data)
     */
    protected function update($query, $data = null)
    {
        if($data && isset($data->page_timestamp))
        {
            $query->update('page_timestamp', $data->page_timestamp);
        }
        else
        {
            $query->update('page_timestamp', date('c'));
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null)
    {
        $query->insert('page_timestamp', date('c'));
        $query->insert('page_html', false);
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#delete($query, $data)
     */
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

            if($dbRow->page_hide === "0" && $dbRow->page_noindex === "0")
            {
                $summary = array("en" => "", "nl" => "");
                $summary["en"] = substr(BBCodeParser::parse($row->page_template['en'], false, false), 0, 100)."...";
                $summary["nl"] = substr(BBCodeParser::parse($row->page_template['nl'], false, false), 0, 100)."...";

                if($query->getAction() == "insert")
                    $id = $record->insertID();
                else
                {
                    $data = $this->getRecord();
                    $id = $data[0]->page_id;
                }

                $indexer = new TextSpider();
                $title = array("en" => $row->page_name, "nl" => $row->page_name);
                if(Language::has($row->page_name, "en"))
                    $title['en'] = Language::get($row->page_name, "en");
                if(Language::has($row->page_name, "nl"))
                    $title['nl'] = Language::get($row->page_name, "nl");

                $priority = $row->page_highlight == 0 ? 0 : 1;

                $indexer->index(
                    $row->page_template,
                    $row->page_location.$row->page_name,
                    $title,
                    "page.{$id}",
                    $summary,
                    $priority,
                    strtotime($row->page_timestamp)
                );
            }
            else
            {
                $data = $this->getRecord();

                $indexer = new TextSpider();
                $indexer->clearByHook("page.{$data[0]->page_id}");
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
            foreach ($data[0]->page_id as $d)
                $indexer->clearByHook("page.{$d}");
        }
    }
}