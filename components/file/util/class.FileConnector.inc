<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.file.util
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
/**
 * Connector class so files can be imported from external sources
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
interface FileConnector
{
    /**
     * Fetch the new images and add them to the database
     * 
     * @return array    List of new elements as inserted into the db
     */
    function fetch();
    /**
     * Modify a record for the tree, this usally includes adding tree premissions
     * 
     * @param object $row
     */
    function walk($row);
    /**
     * Check if the connector is active so we can use it
     * 
     * @return bool
     */
    function isActive();
}