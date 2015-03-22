<?php
namespace Component\Security;

use Symfony\Component\Security\Core\Util\StringUtils;
use Symfony\Component\Security\Core\Encoder\BasePasswordEncoder;

class UserEncoder extends BasePasswordEncoder
{
    /**
     * @see \Symfony\Component\Security\Core\Encoder\PasswordEncoderInterface::encodePassword()
     */
    public function encodePassword($raw, $salt)
    {
        if ($this->isPasswordTooLong($raw)) {
            throw new BadCredentialsException('Invalid password.');
        }

        return sha1($salt . $raw);
    }

    /**
     * @see \Symfony\Component\Security\Core\Encoder\PasswordEncoderInterface::isPasswordValid()
     */
    public function isPasswordValid($encoded, $raw, $salt)
    {
        if ($this->isPasswordTooLong($raw)) {
            return false;
        }

        return StringUtils::equals($encoded, $this->encodePassword($raw, $salt));
    }
}