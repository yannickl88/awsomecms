<?php
/**
 * Links table, this handels all interaction with the links table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GlossaryTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("glossary");
        $this->assign("glossary");

        $this->addField(new IntField("glossary_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("glossary_code"), 1);
        $this->addField(new MLangField("glossary_title"), 2);
        $this->addField(new MLangTextField("glossary_description", Field::OPTIONAL|Field::FORCEUPDATE), 3);
        $this->setTitle("[glossary_title]");

        $this->setOrder('glossary_code');
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     *
     * @param SQLQuery $query
     */
    protected function select($query, $data = null)
    {
        $query->orderby('glossary_title', "ASC");

        if($data)
        {
            $query->whereWhenSet("glossary_id", $data->glossary_id);
            $query->whereWhenSet("glossary_code", $data->glossary_code);
            $query->whereWhenSet("glossary_title", $data->glossary_title);
        }
    }

    protected function insert($query, $data = null) {}
    protected function update($query, $data = null) {}
    protected function delete($query, $data = null) {}

    /**
     * (non-PHPdoc)
     * @see Table::postSQL()
     *
     * @param SQLRecord $record
     * @param SQLQuery $query
     */
    protected function postSQL(&$record, $query)
    {
        if($query->getAction() == "update" || $query->getAction() == "insert" || $query->getAction() == "delete")
        {
            $page = Table::init("page.pages", "index")
                ->setRequest((object) array("page_name" => "abc", "page_location" => "/"))
                ->doSelect()
                ->getRow();

            $summary = array("en" => "", "nl" => "");
            $summary["en"] = substr(BBCodeParser::parse($page->page_template['en'], false, false), 0, 100)."...";
            $summary["nl"] = substr(BBCodeParser::parse($page->page_template['nl'], false, false), 0, 100)."...";

            $abc = Table::init("glossary.glossary", "index")
                ->doSelect();

            $text = array("en" => $page->page_template['en'], "nl" => $page->page_template['nl']);
            while($item = $abc->getRow())
            {
                $text['en'] .= " ".$item->glossary_description['en'];
                $text['nl'] .= " ".$item->glossary_description['nl'];
            }

            $indexer = new TextSpider();
            $indexer->index(
                $text,
                "/abc",
                array("nl" => Language::get("abc", "nl"), "en" => Language::get("abc", "en")),
                "abcpage",
                $summary
            );
        }
    }
}