<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.news
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");
importField("DateField");
importField("TextField");
importField("BooleanField");
importField("users.UsersField");

/**
 * News table, this handels all interaction with the news table in the database
 * 
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
        $this->addField(new Field("news_title"), 1);
        $this->addField(new UsersField("news_user"), 2);
        $this->addField(new DateField("news_date"), 3);
        $this->addField(new Field("news_tag", Field::OPTIONAL), 4);
        $this->addField(new TextField("news_text"), 5);
        $this->setTitle("[news_title]");
        
        $extField = new IntField("news_external", Field::HIDE|Field::OPTIONAL);
        $extField->setDefaultValue(0);
        $this->addField($extField);
        $this->addField(new BooleanField("news_extlink", Field::HIDE|Field::OPTIONAL));
        
        $this->setURLs(
            Config::get('newsadmin', 'newsadmin', 'admin'),
            Config::get('newsadd', 'newsadd', 'admin'),
            Config::get('newsedit', 'newsedit', 'admin'),
            Config::get('newsdelete', 'newsdelete', 'admin')
        );
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null) 
    {
        $query->orderby('news_date', "DESC");
        
        if($data)
        {
            if($data->max)
            {
                $query->limit($data->max);
            }
            $query->whereWhenSet("news_title", $data->news_title);
            $query->whereWhenSet("news_tag", $data->news_tag);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#update($query, $data)
     */
    protected function update($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#delete($query, $data)
     */
    protected function delete($query, $data = null) {}
}