<?php
namespace Component\Form;

use Bundle\AppBundle\Entity\User;
use Bundle\AppBundle\Entity\Bundle\AppBundle\Entity;

class UserData
{
    private $original;
    private $username;
    private $password;
    private $password2;

    public function __construct(User $user = null)
    {
        if ($user === null) {
            $user = new User();
        }

        $this->username = $user->getUsername();
        $this->original = $user;
    }

    public function getUsername()
    {
        return $this->username;
    }

    public function setUsername($username)
    {
        $this->username = $username;
    }

    public function getPassword()
    {
        return $this->password;
    }

    public function setPassword($password)
    {
        $this->password = $password;
    }

    public function getPassword2()
    {
        return $this->password2;
    }

    public function setPassword2($password2)
    {
        $this->password2 = $password2;
    }

    /**
     * @return User
     */
    public function getOriginal()
    {
        return $this->original;
    }
}