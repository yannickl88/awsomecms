<?php
/**
 * @param array $params
 * @param Smarty $smarty
 */
function smarty_function_updates($params, &$smarty)
{
    $updates = SQL::getInstance()->query("
    	SELECT m.model_id AS id, m.model_date AS date, m.model_name AS name, m.model_cat_url AS url, 'c' AS type
    	FROM models AS m
    	UNION
    	SELECT n.news_id AS id, n.news_date AS date, n.news_title AS name, n.news_titleurl AS url, 'n' AS type
    	FROM news AS n
    	WHERE n.news_hidden = 0
    	ORDER BY date DESC
    	LIMIT 0, 5
    ");

    $smarty->assign("updates", $updates->getRows());
    return $smarty->fetch("_updates.tpl");
}