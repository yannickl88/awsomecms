<?php
import('/core/shared/class.RequestComponent.inc');

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
    
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('component_auth', $data['component_auth'])
            ->updateWhenSet('component_version', $data['component_version'])
            ->updateWhenSet('component_patchlevel', $data['component_patchlevel'])
            ->updateWhenSet('component_updateurl', $data['component_updateurl'])
            ->where('component_name', $data['component_name'])
            ->table('components')
            ->exec();
    }
}
