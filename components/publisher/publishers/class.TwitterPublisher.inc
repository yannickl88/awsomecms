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
 * Publisher for twitter.com
 * 
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class TwitterPublisher implements Publisher
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
        $result = $this->makeRequest("GET", "oauth/access_token", array("oauth_verifier" => $request['oauth_verifier']), array($request['oauth_token'], $_SESSION['oauth_token_secret']), "auto");
        
        if(isset($result['error'])) return false;
        
        $oauth_token = $result['oauth_token'];
        $oauth_token_secret = $result['oauth_token_secret'];
        
        //NOW.. finally we can publish something *sigh*
        if($item->type == PublishItem::IMAGE || $item->type == PublishItem::STATUS)
        {
            $sVar = array(
                "status" => $item->message
            );
        }
        elseif($item->type == PublishItem::NEWS)
        {
            $sVar = array(
                "status" => $item->title." ".$item->linkback
            );
        }
        
        $result = $this->makeRequest("POST", "statuses/update.json", $sVar, array($oauth_token, $oauth_token_secret), "auto");
        
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
        $callback = "http://yannickl88.dev/?component=publisher&action=publish&to=twitter&id=".rawurlencode($id);
        
        $idInfo = explode("::", $id);
        $sVar = array(
            "methods" => $this->actionMap($idInfo[0]),
            "oauth_callback" => $callback
        );
        
        $_SESSION['oauth_token_secret'] = "";
        $result = $this->makeRequest("GET", "oauth/request_token", $sVar, array("", ""), "url");
        
        $oauth_token = $result['oauth_token'];
        $_SESSION['oauth_token_secret'] = $result['oauth_token_secret'];
        
        return "http://api.twitter.com/oauth/authorize/?oauth_token=".$oauth_token."&oauth_callback=".rawurlencode($callback);
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
        $key = Config::get("twitter_apiKey", false, "publisher");;
        $secret = Config::get("twitter_apiSecret", false, "publisher");
        $uri="http://api.twitter.com/".$action;
        
        $oauthData = array(
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
        
        if($return == "xml" || ($return == "auto" && strpos($result, "<?xml") === 0))
        {
            try
            {
                $xml = new SimpleXMLElement($result);
                return OAuthUtil::xml2array($xml);
            }
            catch (Exception $e)
            {
                return false;
            }
        }
        elseif($return == "json" || ($return == "auto" && in_array(substr($return, 0, 1), array("{", "["))))
        {
            return json_decode($result);
        }
        elseif($return == "url" || ($return == "auto" && strpos($result, "="))) 
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