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
        $this->addField(new LocationField("page_location"), 2);
        $this->addField(new TextField("page_template"), 3);
        $this->addField(new DateField("page_timestamp", Field::HIDE));
        $this->addField(new BooleanField("page_html", Field::HIDE|Field::OPTIONAL));
        $this->setTitle("[page_name]");
        
        $this->setURLs(
            Config::get('pageadmin', 'pageadmin', 'admin'),
            Config::get('pageadd', 'pageadd', 'admin'),
            Config::get('pageedit', 'pageedit', 'admin'),
            Config::get('pagedelete', 'pagedelete', 'admin')
        );
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
}