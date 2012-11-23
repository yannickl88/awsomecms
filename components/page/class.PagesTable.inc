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
importField("Field");
importField("IntField");
importField("TextField");
importField("DateField");
importField("BooleanField");
importField("MLangTextField");
importField("page.LocationField");

/**
 * Pages table, this handels all interaction with the pages table in the database
 * 
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
            
            $indexer->index(
                $row->page_template, 
                $row->page_location.$row->page_name, 
                $title,
                "page.{$id}",
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
            foreach ($data[0]->page_id as $d)
                $indexer->clearByHook("page.{$d}");
        }
    }
}