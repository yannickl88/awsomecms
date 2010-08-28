<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.publish.publishers
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('publisher.util.Publisher');
/**
 * Publisher for facebook.com
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class FacebookPublisher implements Publisher
{
    /**
     * (non-PHPdoc)
     * @see components/publisher/util/Publisher::publish()
     *
     * @param array $request
     * @param PublishItem $item
     */
    public function publish($request, $item)
    {
        $id = $request['id'];
        $callback = "http://yannickl88.dev/?component=publisher&action=publish&to=facebook&id=".rawurlencode($id);
        
        $data = array(
                "code" => $request['code'],
                "redirect_uri" => $callback
        );
        
        $result = $this->makeRequest("GET", "oauth/access_token", $data, array("", ""), "url");
        
        $access_token = $result['access_token'];
        //NOW.. finally we can publish something *sigh*
        if($item->type == PublishItem::IMAGE || $item->type == PublishItem::STATUS)
        {
            $data = array(
                "message" => $item->message
            );
        }
        elseif($item->type == PublishItem::NEWS)
        {
            $data = array(
                "message" => "", 
                "link" => $item->linkback, 
                "name" => $item->title, 
                "caption" => $item->linkback, 
                "description" => $item->message
            );
            if(!empty($item->picture)) $data['picture'] = $item->picture; //set picture if avalible
        }
        
        $data['access_token'] = $access_token; //add the access token
        $result = $this->makeRequest("POST", "me/feed", $data, array("", ""), "json");
        
        return true;
    }
    /**
     * (non-PHPdoc)
     * @see components/publisher/util/Publisher::auth()
     *
     * @param Array $request
     * @return String       redirect url to auth page
     */
    public function auth($request)
    {
        $id = $request['id'];
        $idInfo = explode("::", $id);
        $callback = "http://yannickl88.dev/?component=publisher&action=publish&to=facebook&id=".rawurlencode($id);
        
        return "https://graph.facebook.com/oauth/authorize?".
            "client_id=".Config::get("facebook_apiID", -1, "publisher")."&".
            "scope=".rawurlencode($this->actionMap($idInfo[0]))."&".
            "display=popup&".
            "redirect_uri=".rawurlencode($callback);
    }
    /**
     * (non-PHPdoc)
     * @see components/publisher/util/Publisher::canPublish()
     *
     * @param String $type
     * @return bool
     */
    public function canPublish($type)
    {
        switch($type)
        {
            case PublishItem::IMAGE:
            case PublishItem::NEWS:
            case PublishItem::STATUS:
                return true;
                break;
            default:
                return false;
                break;
        }
    }
    /**
     * Make an oAuth request
     * 
     * @param String $method            [GET|POST]
     * @param String $action
     * @param array $data               key value pairs
     * @param array $oauth_token        [oauth_token, oauth_token_secret]
     * @return SimpleXMLElement         return xml
     */
    private function makeRequest($method, $action, $data, $oauth_token = array("", ""), $return = "xml")
    {
        $id = Config::get("facebook_apiID", false, "publisher");;
        $key = Config::get("facebook_apiKey", false, "publisher");;
        $secret = Config::get("facebook_apiSecret", false, "publisher");
        $uri="https://graph.facebook.com/";
        
        $oauthData = array(
            "client_id" => $id,
            "client_secret" => Config::get("facebook_apiSecret", false, "publisher"),
        );

        $sVar = array_merge($data, $oauthData);
        
        $url = $uri.$action;
        $params = OAuthUtil::normalizeParameters(OAuthUtil::prepData($sVar));
        
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
        
        if(strtolower($method) == "get")
        {
            curl_setopt($ch, CURLOPT_URL, $url."?".$params);
        }
        else if(strtolower($method) == "post")
        {
            curl_setopt($ch, CURLOPT_URL, $url);
            curl_setopt($ch, CURLOPT_POST, count($sVar));
            curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
        }
        $result = curl_exec($ch);
        curl_close($ch);
        
        if($return == "xml")
        {
            try
            {
                return new SimpleXMLElement($result);
            }
            catch (Exception $e)
            {
                return false;
            }
        }
        elseif($return == "url") 
        {
            $op = array(); 
            $pairs = explode("&", $result); 
            foreach ($pairs as $pair)
            { 
                list($k, $v) = array_map("rawurldecode", explode("=", $pair)); 
                $op[$k] = $v; 
            } 
            return $op; 
        }
        elseif($return == "json")
        {
            return json_decode($result);
        }
        else 
        {
            return $result;
        }
    }
    /**
     * Map types to actions
     * 
     * @param String $type
     * @return String
     */
    public function actionMap($type)
    {
        if(is_array($type))
        {
            $return = array();
            
            foreach($type as $act)
            {
                $return[$act] = $this->actionMap($act);
            }
            
            return implode(",", $return);
        }
        else
        {
            switch ($type)
            {
                case PublishItem::IMAGE:
                case PublishItem::NEWS:
                case PublishItem::STATUS:
                    return "publish_stream";
                    break;
                default:
                    return "";
                    break;
            }
        }
    }
}