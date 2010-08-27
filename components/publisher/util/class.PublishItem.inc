<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.publish.util
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
/**
 * Item which is used to standardize input for different tables
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class PublishItem
{
    const IMAGE     = "image";
    const STATUS    = "status";
    const NEWS      = "news";
    
    public $id;
    public $title;
    public $message;
    public $linkback;
    public $picture;
    public $type;
    public $date;
    
    public $published = false;
    
    /**
     * Constructor
     * 
     * @param String $id        table::id
     * @param String $title
     * @param String $message
     * @param String $linkback
     * @param int $date
     * @param String $picture
     * @param String $type      [PublishItem::IMAGE|PublishItem::NEWS|PublishItem::STATUS]
     */
    public function __construct($id, $title, $message, $linkback, $date, $picture, $type)
    {
        $this->id = $type."::".$id;
        $this->linkback = $linkback;
        $this->message = $message;
        $this->picture = $picture;
        $this->title = $title;
        $this->type = $type;
        $this->date = $date;
    }
}