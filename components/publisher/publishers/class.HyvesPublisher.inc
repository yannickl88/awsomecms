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
import('libs.OAuthUtil');
/**
 * Publisher for hyves.nl
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class HyvesPublisher implements Publisher
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
        $xml = $this->makeRequest("GET", "auth.accesstoken", array(), array($request['oauth_token'], $_SESSION['oauth_token_secret']));
        
        //check if there went something wrong or the user canceled
        if($xml->getName() == "error_result")
        {
            return false;
        }
        
        $oauth_token = $xml->oauth_token->__toString();
        $oauth_token_secret = $xml->oauth_token_secret->__toString();
        
        //NOW.. finally we can publish something *sigh*
        if($item->type == PublishItem::IMAGE || $item->type == PublishItem::STATUS)
        {
            $sVar = array(
                "emotion" => $item->message,
                "visibility" => "default",
                "where" => Config::get("baseurl", "http://www.google.com/")."/",
            );
            $xml = $this->makeRequest("POST", "wwws.create", $sVar, array($oauth_token, $oauth_token_secret));
        }
        elseif($item->type == PublishItem::NEWS)
        {
            $sVar = array(
                "title" => $item->title,
                "body" => $item->message,
                "visibility" => "default"
            );
            $xml = $this->makeRequest("POST", "blogs.create", $sVar, array($oauth_token, $oauth_token_secret));
        }
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
        $sVar = array(
            "methods" => $this->actionMap($idInfo[0])
        );
        
        $_SESSION['oauth_token_secret'] = "";
        $xml = $this->makeRequest("GET", "auth.requesttoken", $sVar);
        
        $oauth_token = $xml->oauth_token->__toString();
        $_SESSION['oauth_token_secret'] = $xml->oauth_token_secret->__toString();
        $callback = "http://yannickl88.dev/?component=publisher&action=publish&to=hyves&id=".rawurlencode($id);
        
        return "http://www.hyves.nl/api/authorize/?oauth_token=".$oauth_token."&oauth_callback=".rawurlencode($callback);
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
    private function makeRequest($method, $action, $data, $oauth_token = array("", ""))
    {
        $key = Config::get("hyves_apiKey", false, "publisher");;
        $secret = Config::get("hyves_apiSecret", false, "publisher");
        $uri="http://data.hyves-api.nl/";
        
        $oauthData = array(
            "ha_fancylayout" => "false",
            "ha_fancylayout_format" => "",
            "ha_format" => "xml",
            "ha_method" => $action,
            "ha_version" => "2.0",
            "oauth_consumer_key" => $key,
            "oauth_signature_method" => "HMAC-SHA1",
            "oauth_nonce" => "".rand(0, 1000),
            "oauth_timestamp" => time(),
            "oauth_token" => $oauth_token[0],
            "oauth_version" => "1.0",
        );

        $sVar = array_merge($data, $oauthData);
        $sVar["oauth_signature"] = OAuthUtil::calculateOAuthSignature($method, $uri, $sVar, $secret, $oauth_token[1]);
        
        $url = $uri;
        $params = OAuthUtil::normalizeParameters(OAuthUtil::prepData($sVar));
        
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        
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
        
        try
        {
            $xml = new SimpleXMLElement($result);
        }
        catch (Exception $e)
        {
            return false;
            
            //header("Content-type: text/plain");
            //var_dump($url, $result, $sVar);
            //die();
        }
        
        return $xml;
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
                    return "wwws.create";
                    break;
                case PublishItem::NEWS:
                    return "blogs.create";
                    break;
                case PublishItem::STATUS:
                    return "wwws.create";
                    break;
                default:
                    return "";
                    break;
            }
        }
    }
}