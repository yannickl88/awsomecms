<?php
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