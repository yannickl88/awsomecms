<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Authentication class that is a facade for loggin in and out
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Authentication
{
    /**
     * Login, and setting the user session. This calls the component set in the config
     *
     * @param String $user_name
     * @param String $user_pass
     * @return Boolean
     */
    public static function login($user_name, $user_pass)
    {
        return;
    }
    /**
     * Logout, and clearing the user session. This calls the component set in the config
     *
     * @return Boolean
     */
    public static function logout()
    {
        return;
    }
}
