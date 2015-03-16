<?php
/**
 * Captcha field to prevent bots.
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class CaptchaField extends Field
{
    /**
     * Constructor
     *
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($name, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW|Field::DUMMY);
    }

    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $html = "";
        $html .= "<div style=\"height: 80px;\">";
        $html .= "<img id=\"siimage\" src=\"/securimage_show.php?sid=".md5(time())."\" style=\"float: left;\"/>";
        $html .= "<a tabindex=\"-1\" style=\"border-style: none\" href=\"#\" title=\"Refresh Image\" onclick=\"$('#siimage').attr('src', '/securimage_show.php?sid=' + Math.random()); return false\"><img src=\"/img/refresh.gif\" alt=\"Reload Image\" border=\"0\" onclick=\"this.blur()\" align=\"bottom\" /></a><br />";
        $html .= "</div>";

        $html .= ucfirst(Language::get("code")).":<br/>";
        $html .= "<input type='text' class='cmsfield' name='{$this->getName()}' id='{$this->getName()}' class='empty'/>";

        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return $this->toHTML_add();
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#validate($mode)
     */
    public function validate($mode)
    {
        $value = $this->value2db($this->getValue());

        $img = new Securimage();

        if(($mode == Field::ADD || $mode == Field::EDIT) && !$img->check($value))
        {
            throw new FormException($this, Language::get("error_wrongcaptcha"));
        }
    }
}