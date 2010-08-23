<?php
interface FileConnector
{
    function get($id);
    function getAll();
    function add($data);
    function update($data);
    function delete($data);
}