<?php
/**
 * @author Yannick <yannick.l.88@gmail.com>
 */
class NewsTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("news");
        $this->assign("news");

        $this->addField(new IntField("news_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new MLangField("news_title"), 1);
        $this->addField(new Field("news_titleurl", Field::HIDE|Field::OPTIONAL));
        $this->addField(new UsersField("news_user"), 2);
        $this->addField(new DateField("news_date"), 3);
        $this->addField(new TagField("news_tag", Field::OPTIONAL|Field::HIDE), 4);
        $this->addField(new MLangTextField("news_text"), 5);
        $this->addField(new BooleanField("news_hidden"), 6);
        $this->setTitle("[news_title]");

        $extField = new IntField("news_external", Field::HIDE|Field::OPTIONAL);
        $extField->setDefaultValue(0);
        $this->addField($extField);
        $this->addField(new Field("news_extsite", Field::HIDE|Field::OPTIONAL));
        $this->addField(new BooleanField("news_extlink", Field::HIDE|Field::OPTIONAL));
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('news_date', "DESC");
        $query->orderby('news_id', "DESC");

        if($data)
        {
            if($data->max)
            {
                $offset = 0;
                if($data->offset) $offset = $data->offset;

                $query->limit($data->max, $offset);
            }

            if(isset($data->hideHidden) && $data->hideHidden)
                $query->where("news_hidden", 0, "=", "AND", true);

            $query->whereWhenSet("news_id", $data->news_id);
            $query->whereWhenSet("news_title", $data->news_title);
            $query->whereWhenSet("news_date", $data->news_date);
            $query->whereWhenSet("news_titleurl", rawurlencode($data->news_titleurl));
            $query->whereWhenSet("news_tag", $data->news_tag);
            $query->whereWhenSet("news_external", $data->news_external);
            $query->whereWhenSet("news_extsite", $data->news_extsite);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#update($query, $data)
     */
    protected function update($query, $data = null)
    {
        $query->update("news_titleurl", urlEscape($data['news_title']["en"]));
        Cache::clear("newslist");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null)
    {
        $query->insert("news_titleurl", urlEscape($data['news_title']["en"]));
        Cache::clear("newslist");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#delete($query, $data)
     */
    protected function delete($query, $data = null) {
        Cache::clear("newslist");
    }

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
            $summary["en"] = substr(BBCodeParser::parse($row->news_text['en'], false, false), 0, 100)."...";
            $summary["nl"] = substr(BBCodeParser::parse($row->news_text['nl'], false, false), 0, 100)."...";

            if($query->getAction() == "insert")
                $id = $record->insertID();
            else
            {
                $data = $this->getRecord();
                $id = $data[0]->news_id;
            }

            $indexer = new TextSpider();
            $indexer->index(
                $row->news_text,
                Component::init("news")->makeURL($row, true),
                $row->news_title,
                "news.{$id}",
                $summary,
                strtotime($row->news_date)
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
            foreach ($data[0]->news_id as $d)
                $indexer->clearByHook("news.{$d}");
        }
    }
}