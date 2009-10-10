<?php
include_once Config::get('websiteroot', '.').'/../core/shared/class.RequestComponent.inc';

class CoreRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        if($data['get'] == "components")
        {
            return Config::getInstance()->getComponenets();
        }
        if($data['get'] == "requests")
        {
            return Config::getInstance()->getRequests();
        }
        if($data['get'] == "config")
        {
            return Config::getInstance()->get($data['key'], '', $data['section']);
        }
    }
}
