<?php
import('/core/shared/class.RequestComponent.inc');

class BlogRequest extends RequestComponent
{
    //CORE ACTIONS
    protected function do_SELECT($data = array())
    {
        //select
        return SQLQuery::doSelect()
            ->table('news')
            ->orderby('news_date', "DESC")
            ->where('news_external', 1)
            ->exec()
            ->getRows();
    }
}
?>