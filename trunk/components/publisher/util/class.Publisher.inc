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
 * Publisher interface
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
interface Publisher
{
    /**
     * Publish an item
     * 
     * @param array $request
     * @param PublishItem $item
     * @return bool                 succes
     */
    function publish($request, $item);
    /**
     * Check if an type can be published by this publisher
     * 
     * @param String $type
     * @return bool
     */
    function canPublish($type);
    /**
     * Authenticated if needed to post the request
     * 
     * @param String $id
     * @return String       redirect url to auth page
     */
    function auth($id);
}