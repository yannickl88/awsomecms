<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.comments
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

/**
 * Comments table, this handels all interaction with the comments table in the database
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class CommentsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("comments");
        $this->assign("comments");
        
        $this->addField(new IntField("comment_id", "", Field::AUTOKEY|Field::INDEX));
        $this->addField(new TextField("comment_text", "Reply"));
        $this->addField(new Field("comment_username", "Name/E-mail"));
        $this->addField(new Field("comment_hook", "Hook", Field::OPTIONAL|Field::HIDE));
        $this->addField(new DateField("comment_time", "", Field::HIDE));
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null) 
    {
        $query->orderBy("comment_time", "desc");
        
        if($data)
        {
            $query->whereWhenSet('comment_hook', $data->comment_hook);
        }
        
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null)
    {
        $query->insert('comment_time', date('c'));
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#update($query, $data)
     */
    protected function update($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#delete($query, $data)
     */
    protected function delete($query, $data = null) {}

}