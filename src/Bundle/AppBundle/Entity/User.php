<?php
namespace Bundle\AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;
use Symfony\Component\Security\Core\User\UserInterface;

/**
 * @ORM\Entity(repositoryClass="Bundle\AppBundle\Entity\Repository\UserRepository")
 * @ORM\Table(name="users")
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class User implements UserInterface, \Serializable
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer", name="user_id")
     * @var int
     */
    private $id;

    /**
     * @ORM\Column(type="string", name="user_name", length=100)
     * @var string
     */
    private $username;

    /**
     * @ORM\Column(type="string", name="user_pass", length=150)
     * @var string
     */
    private $legacy_password;

    private $password;
    private $salt;

    /**
     * @return int
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * @see \Symfony\Component\Security\Core\User\UserInterface::getRoles()
     */
    public function getRoles()
    {
        return ['ROLE_ADMIN'];
    }

    /**
     * @see \Symfony\Component\Security\Core\User\UserInterface::getPassword()
     */
    public function getPassword()
    {
        $this->load();
        return $this->password;
    }

    /**
     * @param string $password
     * @return $this
     */
    public function setPassword($password)
    {
        $this->password = $password;
        $this->save();

        return $this;
    }

    /**
     * @see \Symfony\Component\Security\Core\User\UserInterface::getSalt()
     */
    public function getSalt()
    {
        $this->load();
        return $this->salt;
    }

    /**
     * @param string $salt
     * @return $this
     */
    public function setSalt($salt)
    {
        $this->salt = $salt;
        $this->save();

        return $this;
    }

    /**
     * @see \Symfony\Component\Security\Core\User\UserInterface::getUsername()
     */
    public function getUsername()
    {
        return $this->username;
    }

    /**
     * @param string $username
     * @return $this
     */
    public function setUsername($username)
    {
        $this->username = $username;

        return $this;
    }

    /**
     * @see \Symfony\Component\Security\Core\User\UserInterface::eraseCredentials()
     */
    public function eraseCredentials()
    {

    }

    private function load()
    {
        $data = unserialize($this->legacy_password);

        $this->password = $data['password'];
        $this->salt     = $data['salt'];
    }

    private function save()
    {
        $this->legacy_password = serialize(['password' => $this->password, 'salt' => $this->salt]);
    }

    /**
     * @see \Serializable::serialize()
     */
    public function serialize()
    {
        return serialize(array(
            $this->id,
            $this->username,
            $this->password,
            $this->salt,
        ));
    }

    /**
     * @see \Serializable::unserialize()
     */
    public function unserialize($serialized)
    {
        list (
            $this->id,
            $this->username,
            $this->password,
            $this->salt
        ) = unserialize($serialized);
    }
}
