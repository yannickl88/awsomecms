<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.publish.publishers
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('publisher.util.Publisher');
/**
 * Publisher for facebook.com
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class FacebookPublisher implements Publisher
{
    /**
     * (non-PHPdoc)
     * @see components/publisher/util/Publisher::publish()
     *
     * @param array $request
     * @param PublishItem $item
     */
    public function publish($request, $item)
    {
    }
    /**
     * (non-PHPdoc)
     * @see components/publisher/util/Publisher::auth()
     *
     * @param String $id
     * @return String       redirect url to auth page
     */
    public function auth($id)
    {
    }
    /**
     * (non-PHPdoc)
     * @see components/publisher/util/Publisher::canPublish()
     *
     * @param String $type
     * @return bool
     */
    public function canPublish($type)
    {
        switch($type)
        {
            case PublishItem::IMAGE:
            case PublishItem::NEWS:
            case PublishItem::STATUS:
                return true;
                break;
            default:
                return false;
                break;
        }
    }
}