<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.contact.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Contact for tag, this addes a form to a page
 *
 * params: form
 *
 * @param array $params
 * @param Smarty $smarty
 */
function smarty_function_contactform($params, &$smarty)
{
    $html = '';

    $components = Config::getInstance()->getComponenets();
    
    $form = Table::init("contact.forms")
        ->setRequest((object) array("form_name" => $params['form']))
        ->doSelect()
        ->getRow();
        
    $smarty->assign("contactform", $form);
    
    return $smarty->fetch("file:".$components['contact']->component_path.'/forms/form.contact.tpl');
}