<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.linker.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

function escapeParts($url) {
    $parts = parse_url($url);

    $path = explode("/", $parts["path"]);
    $pathEsc = "";
    $first = true;

    foreach($path as $part) {
        if(!$first) {
            $pathEsc .= "/";
        }
        $pathEsc .= rawurlencode($part);
        $first = false;
    }

    return $parts["host"].
        $pathEsc.
        (isset($parts["query"]) ? "?" . $parts["query"] : "") .
        (isset($parts["fragment"]) ? "#" . $parts["fragment"] : "");
}

/**
 * @param array $params
 * @param Smarty $smarty
 */
function smarty_function_facebooktag($params, &$smarty)
{
    $data = Config::get("facebookData", (object) array());
    $appID = Config::get("facebookID", "");
    $html = "";

    if (isset($data->image)) {
        $img = explode("/", $data->image);

        $html .= "<meta property=\"fb:app_id\" content=\"{$appID}\" />\n";
        $html .= "<meta property=\"og:type\" content=\"website\" />\n";
        $html .= "<meta property=\"og:title\" content=\"".htmlentities($data->title). "\" />\n";
        $html .= "<meta property=\"og:image\" content=\"".htmlentities($data->image). "\" />\n";
        $html .= "<meta property=\"og:description\" content=\"".htmlentities($data->description). "\" />\n";
        $html .= "<meta property=\"og:url\" content=\"".htmlentities($data->url). "\" />\n";
    }

    return $html;
}