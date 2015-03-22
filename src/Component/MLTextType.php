<?php
namespace Component;

use Doctrine\DBAL\Types\Type;
use Doctrine\DBAL\Platforms\AbstractPlatform;

class MLTextType extends Type
{
    const MLTEXT_TYPE = 'mltext';

    public function getSQLDeclaration(array $fieldDeclaration, AbstractPlatform $platform)
    {
        return $platform->getClobTypeDeclarationSQL($fieldDeclaration);
    }

    public function convertToPHPValue($value, AbstractPlatform $platform)
    {
        if (false === ($text = unserialize($value))) {
            return $this->recover($value);
        }

        return $text;
    }

    public function convertToDatabaseValue($value, AbstractPlatform $platform)
    {
        return serialize($value);
    }

    public function getName()
    {
        return self::MLTEXT_TYPE;
    }

    private function recover($raw)
    {
        return ['en' => '', 'nl' => ''];
    }
}
