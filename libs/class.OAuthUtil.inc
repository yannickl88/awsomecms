<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package libs
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
/**
 * OAuth helper class
 * 
 * @author http://www.hyves-developers.nl/documentation/data-api/oauth-example-code
 * NOTE: most of the code comes from the developer pages of hyves
 */
class OAuthUtil
{
    /**
     * URLEncode a string RFC3986
     * 
     * @param String $string
     * @return String
     */
    public static function urlencodeRFC3986($string)
    {
        return str_replace('%7E', '~', rawurlencode($string));
    }
    /**
     * URLEncode a string RFC3986 and UTF8
     * 
     * @param String $string
     * @return String
     */
    public static function urlencodeRFC3986_UTF8($string)
    {
        return self::urlencodeRFC3986(iconv("ISO-8859-1", "UTF-8", $string));
    }
    /**
     * URLDecode a string RFC3986
     * 
     * @param String $string
     * @return String
     */
    public static function urldecodeRFC3986($string)
    {
        return rawurldecode($string); // no exta stuff needed for ~, goes correctly automatically
    }
    /**
     * URLDecode a string RFC3986 and UTF8
     * 
     * @param String $string
     * @return String
     */
    public static function urldecodeRFC3986_UTF8($string)
    {
        return iconv("UTF-8", "ISO-8859-1", self::urldecodeRFC3986($string));
    }
    /**
     * Prep the data into a multidementional array
     * 
     * @param array $sVar
     * @return array
     */
    public static function prepData($sVar)
    {
        $aParam = array();
        foreach($sVar as $key=>$value)
        {
            $aParam[] = array("key"=>$key, "value"=>$value);
        }
        
        return $aParam;
    }
    /**
     * Normalize data and sort in alphabatic order
     * 
     * @param array $aParam
     * @return String
     */
    public static function normalizeParameters($aParam)
    {
        $sortfunc = create_function('$a, $b', '
                                $cmp = strcmp($a["key"], $b["key"]);
                                if ($cmp == 0) {
                                        return strcmp($a["value"], $b["value"]);
                                }
                                return $cmp;
                        ');
        usort($aParam, $sortfunc);
        
        $aEncodedVars = array();
        foreach($aParam as $param)
        {
            $aEncodedVars[] = self::urlencodeRFC3986_UTF8($param["key"])."=".self::urlencodeRFC3986_UTF8($param["value"]);
        }
        return implode("&", $aEncodedVars);
    }
    /**
     * Generate Base String for signature
     * 
     * @param String $http_method       [GET|POST]
     * @param String $uri
     * @param String $params
     */
    public static function generateBaseString($http_method, $uri, $params)
    {
        $aBasestring = array(
            self::urlencodeRFC3986_UTF8($http_method),
            self::urlencodeRFC3986_UTF8($uri),
            self::urlencodeRFC3986_UTF8($params),
        );
        return implode("&", $aBasestring);
    }
    /**
     * Calculate the signature HASH
     * 
     * @param String $basestring
     * @param String $consumersecret
     * @param String $tokensecret
     * @return String
     */
    public static function calculateHMACSHA1Signature($basestring, $consumersecret, $tokensecret) {
        $aKey = array(
            self::urlencodeRFC3986_UTF8($consumersecret),
            self::urlencodeRFC3986_UTF8($tokensecret),
        );
        $key = implode("&", $aKey);
        if (function_exists("hash_hmac"))
        {
            $signature = base64_encode(hash_hmac("sha1", $basestring, $key, true));
        }
        else
        {
            $signature = base64_encode(self::hmacsha1($key, $basestring));
        }
        return $signature;
    }
    /**
     * HMAC-SHA1 not dependent on php compile flags
     * 
     * @param String $key
     * @param String $data
     * @return String
     **/
    public static function hmacsha1($key, $data)
    {
        $blocksize = 64;
        $hashfunc = 'sha1';
        
        if (strlen($key) > $blocksize)
            $key = pack('H*', $hashfunc($key));
            
        $key = str_pad($key,$blocksize,chr(0x00));
        $ipad = str_repeat(chr(0x36),$blocksize);
        $opad = str_repeat(chr(0x5c),$blocksize);
        $hmac = pack(
            'H*',$hashfunc(
                ($key^$opad).pack(
                    'H*',$hashfunc(
                        ($key^$ipad).$data
                    )
                )
            )
        );
        return $hmac;
    }
    /**
     * Calculate the OAuth Signature Hash
     * 
     * @param String $http_method       [GET|POST]
     * @param String $uri
     * @param array $sVar
     * @param String $consumersecret
     * @param String $oauth_token_secret
     * @return String
     */
    public static function calculateOAuthSignature($http_method, $uri, $sVar, $consumersecret, $oauth_token_secret)
    {
        $params = self::normalizeParameters(self::prepData($sVar));
        $basestring = self::generateBaseString($http_method, $uri, $params);
        $signature = self::calculateHMACSHA1Signature($basestring, $consumersecret, $oauth_token_secret);
        return $signature;
    }
    /**
     * Convert an XML to an array, this will lose attribute info
     * 
     * @param SimpleXMLElement $xml
     * @return *        Array or a String
     */
    public static function xml2array($xml)
    {
        if($xml->count() > 0)
        {
            $arr = array();
            
            $children = $xml->children();
            foreach ($children as $elementName => $node)
            {
                $arr[$elementName] = self::xml2array($node);
            }
            
            return $arr;
        }
        else
        {
            return $xml->__toString();
        }
    }
}